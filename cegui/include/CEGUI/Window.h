/***********************************************************************
    created:    21/2/2004
    author:     Paul D Turner

    purpose:    Defines abstract base class for Window objects
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/

#ifndef _CEGUIWindow_h_
#define _CEGUIWindow_h_

#include "CEGUI/NamedElement.h"
#include "CEGUI/RenderedString.h"
#include "CEGUI/InputEvent.h"
#include <unordered_set>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif


namespace CEGUI
{

/*!
\brief
    Enumerated type used for specifying Window::update mode to be used.  Note
    that the setting specified will also have an effect on child window
    content; for WindowUpdateMode::NEVER and WindowUpdateMode::VISIBLE, if the parent's update function is
    not called, then no child window will have it's update function called
    either - even if it specifies WindowUpdateMode::ALWAYS as it's WindowUpdateMode.
*/
enum class WindowUpdateMode : int
{
    //! Always call the Window::update function for this window.
    Always,
    //! Never call the Window::update function for this window.
    Never,
    //! Only call the Window::update function for this window if it is visible.
    Visible
};

template<>
class PropertyHelper<WindowUpdateMode>
{
public:
    typedef WindowUpdateMode return_type;
    typedef return_type safe_method_return_type;
    typedef WindowUpdateMode pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName()
    {
        static String type("WindowUpdateMode");

        return type;
    }

    static return_type fromString(const String& str)
    {

        if (str == "Always")
        {
            return WindowUpdateMode::Always;
        }
        else if (str == "Never")
        {
            return WindowUpdateMode::Never;
        }
        else
        {
            return WindowUpdateMode::Visible;
        }
    }

    static string_return_type toString(pass_type val)
    {
        if (val == WindowUpdateMode::Always)
        {
            return "Always";
        }
        else if (val == WindowUpdateMode::Never)
        {
            return "Never";
        }
        else if (val == WindowUpdateMode::Visible)
        {
            return "Visible";
        }
        else
        {
            assert(false && "Invalid Window Update Mode");
            return "Always";
        }
    }
};


/*!
\brief
    An abstract base class providing common functionality and specifying the
    required interface for derived classes.

    The Window base class is core UI object class that the the system knows
    about; for this reason, every other window, widget, or similar item within
    the system must be derived from Window.

    The base class provides the common functionality required by all UI objects,
    and specifies the minimal interface required to be implemented by derived
    classes.
*/
class CEGUIEXPORT Window :
    public NamedElement
{
public:
    /*************************************************************************
        Property name constants
    *************************************************************************/
    //! Name of property to access for the alpha value of the Window.
    static const String AlphaPropertyName;
    //! Name of property to access for the 'always on top' setting for the Window
    static const String AlwaysOnTopPropertyName;
    //! Name of property to access for the 'clipped by parent' setting for the Window
    static const String ClippedByParentPropertyName;
    //! Name of property to access for the 'destroyed by parent' setting for the Window
    static const String DestroyedByParentPropertyName;
    //! Name of property to access for the 'disabled state' setting for the Window
    static const String DisabledPropertyName;
    //! Name of property to access for the font for the Window.
    static const String FontPropertyName;
    //! Name of property to access for the ID value of the Window.
    static const String IDPropertyName;
    //! Name of property to access for the get/set the 'inherits alpha' setting for the Window.
    static const String InheritsAlphaPropertyName;
    //! Name of property to access for the the mouse cursor image for the Window.
    static const String CursorImagePropertyName;
    //! Name of property to access for the the 'visible state' setting for the Window.
    static const String VisiblePropertyName;
    //! Name of property to access for the the 'active state' setting for the Window.
    static const String ActivePropertyName;
    //! Name of property to access for the 'restore old capture' setting for the Window.
    static const String RestoreOldCapturePropertyName;
    //! Name of property to access for the text / caption for the Window.
    static const String TextPropertyName;
    //! Name of property to access for the 'z-order changing enabled' setting for the Window. 
    static const String ZOrderingEnabledPropertyName;
    //! Name of property to access for whether the window will receive autorepeat mouse button down events.
    static const String CursorAutoRepeatEnabledPropertyName;
    //! Name of property to access for the autorepeat delay.
    static const String AutoRepeatDelayPropertyName;
    //! Name of property to access for the autorepeat rate.
    static const String AutoRepeatRatePropertyName;
    //! Name of property to access for the whether captured inputs are passed to child windows.
    static const String DistributeCapturedInputsPropertyName;
    //! Name of property to access for the custom tooltip for the window. 
    static const String TooltipTypePropertyName;
    //! Name of property to access for the tooltip text for the window.
    static const String TooltipTextPropertyName;
    //! Name of property to access for the window inherits its parents tooltip text when it has none of its own.
    static const String InheritsTooltipTextPropertyName;
    //! Name of property to access for the window will come to the top of the Z-order when clicked.
    static const String RiseOnClickEnabledPropertyName;
    //! Name of property to access for the window ignores mouse events and pass them through to any windows behind it.
    static const String CursorPassThroughEnabledPropertyName;
    //! Name of property to access for the Window will receive drag and drop related notifications.
    static const String DragDropTargetPropertyName;
    //! Name of property to access for the Window whether texture caching should be activated or not. Will only have an effect if the Renderer supports texture caching.
    static const String AutoRenderingSurfacePropertyName;
    //! Name of property to access for the Window whether texture caching should have a stencil buffer attached for stencil operations, as used in SVG and Custom Shape rendering.
    static const String AutoRenderingSurfaceStencilEnabledPropertyName;
    //! Name of property to access for the text parsing setting for the Window.
    static const String TextParsingEnabledPropertyName;
    //! Name of property to access for the margin for the Window.
    static const String MarginPropertyName;
    //! Name of property to access for the window update mode setting.
    static const String UpdateModePropertyName;
    //! Name of property to access whether unhandled mouse inputs should be propagated back to the Window's parent. 
    static const String CursorInputPropagationEnabledPropertyName;
    //! Name of property to access whether the system considers this window to be an automatically created sub-component window.
    static const String AutoWindowPropertyName;
    //! Name of property to access the DrawMode that is set for this Window, which decides in what draw call it will or will not be drawn.
    static const String DrawModeMaskPropertyName;

    /*************************************************************************
        Event name constants
    *************************************************************************/
    //! Namespace for global events
    static const String EventNamespace;

    // generated internally by Window
    /** Event fired as part of the time based update of the window.
     * Handlers are passed a const UpdateEventArgs reference.
     */
    static const String EventUpdated;

    /** Event fired when the text string for the Window has changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Window whose text was changed.
     */
    static const String EventTextChanged;
    /** Event fired when the Font object for the Window has been changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Window whose font was changed.
     */
    static const String EventFontChanged;
    /** Event fired when the Alpha blend value for the Window has changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Window whose alpha value was changed.
     */
    static const String EventAlphaChanged;
    /** Event fired when the client assigned ID for the Window has changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Window whose ID was changed.
     */
    static const String EventIDChanged;
    /** Event fired when the Window has been activated and has input focus.
     * Handlers are passed a const ActivationEventArgs reference with
     * WindowEventArgs::window set to the Window that is gaining activation and
     * ActivationEventArgs::otherWindow set to the Window that is losing
     * activation (may be 0).
     */
    static const String EventActivated;
    /** Event fired when the Window has been deactivated, losing input focus.
     * Handlers are passed a const ActivationEventArgs reference with
     * WindowEventArgs::window set to the Window that is losing activation and
     * ActivationEventArgs::otherWindow set to the Window that is gaining
     * activation (may be 0).
     */
    static const String EventDeactivated;
    /** Event fired when the Window is shown (made visible).
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Window that was shown.
     */
    static const String EventShown;
    /** Event fired when the Window is made hidden.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Window that was hidden.
     */
    static const String EventHidden;
    /** Event fired when the Window is enabled so interaction is possible.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Window that was enabled.
     */
    static const String EventEnabled;
    /** Event fired when the Window is disabled and interaction is no longer
     * possible.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Window that was disabled.
     */
    static const String EventDisabled;
    /** Event fired when the Window clipping mode is modified.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Window whose clipping mode was
     * changed.
     */
    static const String EventClippedByParentChanged;
    /** Event fired when the Window destruction mode is modified.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Window whose destruction mode was
     * changed.
     */
    static const String EventDestroyedByParentChanged;
    /** Event fired when the Window mode controlling inherited alpha is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Window whose alpha inheritence mode
     * was changed.
     */
    static const String EventInheritsAlphaChanged;
    /** Event fired when the always on top setting for the Window is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Window whose always on top setting
     * was changed.
     */
    static const String EventAlwaysOnTopChanged;
    /** Event fired when the Window gains capture of cursor inputs.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Window that has captured cursor inputs.
     */
    static const String EventInputCaptureGained;
    /** Event fired when the Window loses capture of cursor inputs.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to either:
     * - the Window that has lost capture of cursor inputs if that event was
     *   caused by the window itself releasing the capture.
     * - the Window that is @gaining capture of cursor inputs if that is the
     *   cause of the previous window with capture losing that capture.
     */
    static const String EventInputCaptureLost;
    /** Event fired when the Window has been invalidated.
     * When a window is invalidated its cached rendering geometry is cleared,
     * the rendering surface that receives the window's output is invalidated
     * and the window's target GUIContext is marked as dirty; this causes all
     * objects involved in the display of the window to be refreshed the next
     * time that the GUIContext::draw function is called.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Window that has been invalidated.
     */
    static const String EventInvalidated;
    /** Event fired when rendering of the Window has started.  In this context
     * 'rendering' is the population of the GeometryBuffer with geometry for the
     * window, not the actual rendering of that GeometryBuffer content to the
     * display.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Window whose rendering has started.
     */
    static const String EventRenderingStarted;
    /** Event fired when rendering of the Window has ended.  In this context
     * 'rendering' is the population of the GeometryBuffer with geometry for the
     * window, not the actual rendering of that GeometryBuffer content to the
     * display.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Window whose rendering has ended.
     */
    static const String EventRenderingEnded;
    /** Event fired when destruction of the Window is about to begin.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Window that is about to be destroyed.
     */
    static const String EventDestructionStarted;
    /** Event fired when a DragContainer is dragged in to the window's area.
     * Handlers are passed a const DragDropEventArgs reference with
     * WindowEventArgs::window set to the window over which a DragContainer has
     * been dragged (the receiving window) and DragDropEventArgs::dragDropItem
     * set to the DragContainer that was dragged in to the receiving window's
     * area.
     */
    static const String EventDragDropItemEnters;
    /** Event fired when a DragContainer is dragged out of the window's area.
     * Handlers are passed a const DragDropEventArgs reference with
     * WindowEventArgs::window set to the window over which a DragContainer has
     * been dragged out of (the receiving window) and
     * DragDropEventArgs::dragDropItem set to the DragContainer that was dragged
     * out of the receiving window's area.
     */
    static const String EventDragDropItemLeaves;
    /** Event fired when a DragContainer is dropped within the window's area.
     * Handlers are passed a const DragDropEventArgs reference with
     * WindowEventArgs::window set to the window over which a DragContainer was
     * dropped (the receiving window) and DragDropEventArgs::dragDropItem set to
     * the DragContainer that was dropped within the receiving window's area.
     */
    static const String EventDragDropItemDropped;
    /** Event fired when a WindowRenderer object is attached to the window.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the window that had the WindowRenderer
     * attached to it.
     */
    static const String EventWindowRendererAttached;
    /** Event fired when a WindowRenderer object is detached from the window.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the window that had the WindowRenderer
     * detached from it.
     */
    static const String EventWindowRendererDetached;
    /** Event fired when the Window's setting controlling parsing of it's text
     * string is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Window whose text parsing setting was
     * changed.
     */
    static const String EventTextParsingChanged;
    /** Event fired when the Window's margin has changed (any of the four margins)
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Window whose margin was
     * changed.
     */
    static const String EventMarginChanged;

    // generated externally (inputs)
    /** Event fired when the cursor has entered the Window's area.
     * Handlers are passed a const CursorInputEventArgs reference with all fields
     * valid.
     */
    static const String EventCursorEntersArea;
    /** Event fired when the cursor has left the Window's area.
     * Handlers are passed a const CursorInputEventArgs reference with all fields
     * valid.
     */
    static const String EventCursorLeavesArea;
    /** Event fired when the cursor enters the Window's area.
     * Handlers are passed a const CursorInputEventArgs reference with all fields
     * valid.
     *\note This event is fired if - and only if - the cursor is actually
     * over some part of this Window's surface area, and will not fire for
     * example if the location of the cursor is over some child window (even
     * though the cursor is technically also within the area of this Window).
     * For an alternative version of this event see the
     * Window::EventCursorEntersArea event.
     */
    static const String EventCursorEntersSurface;
    /** Event fired when the cursor is no longer over the Window's surface
     * area.
     * Handlers are passed a const CursorInputEventArgs reference with all fields
     * valid.
     *\note This event will fire whenever the cursor is no longer actually over
     * some part of this Window's surface area, for example if the cursor is
     * moved over some child window (even though technically the cursor has not
     * actually 'left' this Window's area).  For an alternative version of this
     * event see the Window::EventCursorLeavesArea event.
     */
    static const String EventCursorLeavesSurface;
    /** Event fired when the cursor moves within the area of the Window.
     * Handlers are passed a const CursorInputEventArgs reference with all fields
     * valid.
     */
    static const String EventCursorMove;
    /** Event fired when there is a scroll event within the Window's area.
     * Handlers are passed a const CursorInputEventArgs reference with all fields
     * valid.
     */
    static const String EventScroll;
    /** Event fired when a cursor is pressed and held down within the Window.
     * Handlers are passed a const CursorInputEventArgs reference with all fields
     * valid.
     */
    static const String EventCursorPressHold;
    /** Event fired when the cursor is activated twice within the Window.
     * Handlers are passed a const CursorInputEventArgs reference with all fields
     * valid.
     */
    static const String EventSelectWord;
    /** Event fired when the cursor is activated three times within the Window.
     * Handlers are passed a const CursorInputEventArgs reference with all fields
     * valid.
     */
    static const String EventSelectAll;
    /** Event fired when the cursor is activated within the Window.
     * Handlers are passed a const CursorInputEventArgs reference with all fields
     * valid.
     */
    static const String EventCursorActivate;
    /** Event fired when the Window receives a character key input event.
     * Handlers are passed a const CursorInputEventArgs reference.
     */
    static const String EventCharacterKey;
    /** Event fired when the Window receives a semantic input event.
     * Handler are passed a const SemanticEventArgs reference with the details
     * of what semantic event was received
     */
    static const String EventSemanticEvent;

    /*************************************************************************
        Child Widget name suffix constants
    *************************************************************************/
    //! Widget name suffix for automatically created tooltip widgets.
    static const String TooltipNameSuffix;

    // XML element and attribute names that relate to Window.
    static const String WindowXMLElementName;
    static const String AutoWindowXMLElementName;
    static const String UserStringXMLElementName;
    static const String WindowTypeXMLAttributeName;
    static const String WindowNameXMLAttributeName;
    static const String AutoWindowNamePathXMLAttributeName;
    static const String UserStringNameXMLAttributeName;
    static const String UserStringValueXMLAttributeName;

    /*!
    \brief
        Constructor for Window base class

    \param type
        String object holding Window type (usually provided by WindowFactory).

    \param name
        String object holding unique name for the Window.
    */
    Window(const String& type, const String& name);

    /*!
    \brief
        Destructor for Window base class
    */
    virtual ~Window() override;

    /*!
    \brief
        return a String object holding the type name for this Window.

    \return
        String object holding the Window type.
    */
    inline const String& getType() const
    {
        return d_falagardType.empty() ? d_type : d_falagardType;
    }

    /*!
    \brief
        return a String object holding the factory type name for this Window without Falagard mapping.
    */
    inline const String& getFactoryType() const
    {
        return d_type;
    }

    /*!
    \brief
        returns whether or not this Window is set to be destroyed when its
        parent window is destroyed.

    \return
        - true if the Window will be destroyed when its parent is destroyed.
        - false if the Window will remain when its parent is destroyed.
    */
    bool isDestroyedByParent() const {return d_destroyedByParent;}

    /*!
    \brief
        returns whether or not this Window is an always on top Window.  Also
        known as a top-most window.

    \return
        - true if this Window is always drawn on top of other normal windows.
        - false if the Window has normal z-order behaviour.
    */
    bool isAlwaysOnTop() const {return d_alwaysOnTop;}

    /*!
    \brief
        return whether the Window is currently disabled

    \note
        Only checks the state set for this window, and does not
        factor in inherited state from ancestor windows.

    \return
        - true if the window is disabled.
        - false if the window is enabled.
    */
    inline bool isDisabled() const
    {
        return !d_enabled;
    }

    /*!
    \brief
        return whether the Window is currently disabled

    \note
        Not only checks the state set for this window, but also
        factors in inherited state from ancestor windows.

    \return
        - true if the window is disabled.
        - false if the window is enabled.
    */
    inline bool isEffectiveDisabled() const
    {
        return !d_enabled || (d_parent && getParent()->isEffectiveDisabled());
    }

    /*!
    \brief
        return true if the Window is currently visible.

        When true is returned from this function does not mean that the window
        is not completely obscured by other windows, just that the window will
        be processed when rendering, and is not explicitly marked as hidden.

    \note
        Only checks the state set for this window, and does not
        factor in inherited state from ancestor windows.

    \return
        - true if the window is set as visible.
        - false if the window is set as hidden.
    */
    inline bool isVisible() const
    {
        return d_visible;
    }

    /*!
    \brief
        return true if the Window is currently visible.

        When true is returned from this function does not mean that the window
        is not completely obscured by other windows, just that the window will
        be processed when rendering, and is not explicitly marked as hidden.

    \note
        Does check the state set for this window, but also
        factors in inherited state from ancestor windows.

    \return
        - true if the window will be drawn.
        - false if the window is hidden and therefore ignored when rendering.
    */
    inline bool isEffectiveVisible() const
    {
        return d_visible && (!d_parent || getParent()->isEffectiveVisible());
    }

    /*!
    \brief
        return true if this is the active Window.  An active window is a window
        that may receive user inputs.

        Cursor events are always sent to the window containing the cursor
        regardless of what this function reports (unless a window has
        captured inputs). The active state mainly determines where send other,
        for example keyboard inputs.

    \return
        - true if the window is active and may be sent inputs by the system.
        - false if the window is inactive and will not be sent inputs.
    */
    inline bool isActive() const
    {
        return d_active && (!d_parent || getParent()->isActive());
    }

    /*!
    \brief
        return true if this Window is clipped so that its rendering will not
        pass outside of its parent Window area.

    \return
        - true if the window will be clipped by its parent Window.
        - false if the windows rendering may pass outside its parents area
    */
    bool isClippedByParent() const { return d_clippedByParent; }

    /*!
    \brief
        return the ID code currently assigned to this Window by client code.

    \return
        unsigned int value equal to the currently assigned ID code for this Window.
    */
    unsigned int getID() const { return d_ID; }

    using NamedElement::isChild;
    /*!
    \brief
        returns whether at least one window with the given ID code is attached
        to this Window as a child.

    \note
        ID codes are client assigned and may or may not be unique, and as such,
        the return from this function will only have meaning to the client code.

    \param ID
        unsigned int ID code to look for.

    \return
        - true if at least one child window was found with the ID code \a ID
        - false if no child window was found with the ID code \a ID.
    */
    bool isChild(unsigned int ID) const;

    using NamedElement::isChildRecursive;
    /*!
    \brief
        returns whether at least one window with the given ID code is attached
        to this Window or any of it's children as a child.

    \note
        ID codes are client assigned and may or may not be unique, and as such,
        the return from this function will only have meaning to the client code.

        WARNING! This function can be very expensive and should only be used
        when you have no other option available. If you decide to use it anyway,
        make sure the window hierarchy from the entry point is small.

    \param ID
        unsigned int ID code to look for.

    \return
        - true if at least one child window was found with the ID code \a ID
        - false if no child window was found with the ID code \a ID.
    */
    bool isChildRecursive(unsigned int ID) const;


    /*!
    \brief
        returns a pointer to the child window at the specified index. Idx is the
        index of the window in the child window list. It is based on the order
        in which the children were added and is stable.

    \param idx
        Index of the child window list position of the window that should be
        returned.This value is not bounds checked, client code should ensure that
        this is less than the value returned by getChildCount().

    \return
        Pointer to the child window currently attached at index position \a idx
    */
    Window* getChildAtIndex(size_t idx) const { return static_cast<Window*>(getChildElementAtIndex(idx)); }

    /*!
    \brief
        return the attached child window that the given name path references.

        A name path is a string that describes a path down the window
        hierarchy using window names and the forward slash '/' as a separator.
        \par
        For example, if this window has a child attached to it named "Panel"
        which has its own children attached named "Okay" and "Cancel",
        you can access the window "Okay" from this window by using the
        name path "Panel/Okay".  To access "Panel", you would simply pass the
        name "Panel".

    \param name_path
        String object holding the name path of the child window to return.

    \return
        the Window object referenced by \a name_path.

    \exception UnknownObjectException
        thrown if \a name_path does not reference a Window attached to this
        Window.
    */
    Window* getChild(const String& name_path) const { return static_cast<Window*>(getChildElement(name_path)); }

    /*!
    \brief
        return the attached child window that the given name path references.

        A name path is a string that describes a path down the window
        hierarchy using window names and the forward slash '/' as a separator.
        \par
        For example, if this window has a child attached to it named "Panel"
        which has its own children attached named "Okay" and "Cancel",
        you can access the window "Okay" from this window by using the
        name path "Panel/Okay".  To access "Panel", you would simply pass the
        name "Panel".

    \param name_path
        String object holding the name path of the child window to return.

    \return
        Pointer to the Window object referenced by \a name_path.
        If no child is found with the name \a name, 0 is returned.
    */
    Window* findChild(const String& name_path) const { return static_cast<Window*>(findChildElement(name_path)); }

    /*!
    \brief
        return a pointer to the first attached child window with the specified
        name. Children are traversed recursively.

        Contrary to the non recursive version of this function, this one will
        not throw an exception, but return 0 in case no child was found.

    \note
        WARNING! This function can be very expensive and should only be used
        when you have no other option available. If you decide to use it anyway,
        make sure the window hierarchy from the entry point is small.

    \param name
        String object holding the name of the window to return a pointer to.

    \return
        Pointer to the (first) Window object attached to this window that has
        the name \a name.
        If no child is found with the name \a name, 0 is returned.
    */
    Window* getChildRecursive(const String& name) const { return static_cast<Window*>(getChildElementRecursive(name)); }

    /*!
    \brief
        return a pointer to the first attached child window with the specified
        ID value.

        This function will throw an exception if no child object with the given
        ID is attached.  This decision was made (over returning NULL if no
        window was found) so that client code can assume that if the call
        returns it has a valid window pointer.  We provide the findChild()
        functions for exception free code.

    \param ID
        unsigned int value specifying the ID code of the window to return a pointer to.

    \return
        Pointer to the (first) Window object attached to this window that has
        the ID code \a id.

    \exception UnknownObjectException
        thrown if no window with the ID code \a id is attached to this Window.
    */
    Window* getChild(unsigned int id) const;

    /*!
    \brief
        return a pointer to the first attached child window with the specified
        ID value.

    \param ID
        unsigned int value specifying the ID code of the window to return a pointer to.

    \return
        Pointer to the (first) Window object attached to this window that has
        the ID code \a ID.
        If no child is found with the ID code \a ID, 0 is returned.
    */
    Window* findChild(unsigned int id) const;

    /*!
    \brief
        return a pointer to the first attached child window with the specified
        ID value. Children are traversed recursively.

        Contrary to the non recursive version of this function, this one will
        not throw an exception, but return 0 in case no child was found.

    \note
        WARNING! This function can be very expensive and should only be used
        when you have no other option available. If you decide to use it anyway,
        make sure the window hierarchy from the entry point is small.

    \param ID
        unsigned int value specifying the ID code of the window to return a pointer to.

    \return
        Pointer to the (first) Window object attached to this window that has
        the ID code \a ID.
        If no child is found with the ID code \a ID, 0 is returned.
    */
    Window* getChildRecursive(unsigned int ID) const;

    /*!
    \brief
        return a pointer to the Window that currently has input focus starting
        with this Window.

    \return
        Pointer to the window that is active (has input focus) starting at this
        window.  The function will return 'this' if this Window is active
        and either no children are attached or if none of the attached children
        are active.  Returns NULL if this Window (and therefore all children)
        are not active.
    */
    Window* getActiveChild();
    const Window* getActiveChild() const;

    using NamedElement::isAncestor;
    /*!
    \brief
        return true if any Window with the given ID is some ancestor of this
        Window.

    \param ID
        unsigned int value specifying the ID to look for.

    \return
        - true if an ancestor (parent, or parent of parent, etc) was found with
          the ID code \a ID.
        - false if no ancestor window has the ID code \a ID.
    */
    bool isAncestor(unsigned int ID) const;

    /*!
    \brief
        return the active Font object for the Window.

    \return
        Pointer to the Font being used by this Window.  If the window has no
        assigned font, and \a useDefault is true, then the default system font
        is returned.
    */
    const Font* getActualFont() const;

    //! Returns the font set for this window, nullptr means that a default font will be used
    const Font* getFont() const { return d_font; }

    /*!
    \brief
        return the current text for the Window

    \return
        The String object that holds the current text for this Window.
    */
    const String& getText() const { return d_textLogical; }

    /*!
    \brief
        Return text string with \e visual ordering of glyphs. This
        only returns meaningful data if using only bidi. Will return
        the regular text String if using raqm or no bidi.
    */
    const String& getTextVisual() const;

    /*!
    \brief
        return true if the Window inherits alpha from its parent(s).

    \return
        - true if the Window inherits alpha from its parent(s)
        - false if the alpha for this Window is independant from its parents.
    */
    bool inheritsAlpha() const  {return d_inheritsAlpha;}

    /*!
    \brief
        return the current alpha value set for this Window

    \note
        The alpha value set for any given window may or may not be the final
        alpha value that is used when rendering.  All window objects, by
        default, inherit alpha from thier parent window(s) - this will blend
        child windows, relatively, down the line of inheritance.  This behaviour
        can be overridden via the setInheritsAlpha() method.  To return the true
        alpha value that will be applied when rendering, use the
        getEffectiveAlpha() method.

    \return
        the currently set alpha value for this Window.  The value returned Will
        be between 0.0f and 1.0f.
    */
    float getAlpha() const { return d_alpha; }

    /*!
    \brief
        return the effective alpha value that will be used when rendering this
        window, taking into account inheritance of parent window(s) alpha.

    \return
        the effective alpha that will be applied to this Window when rendering.
        The value returned Will be between 0.0f and 1.0f.
    */
    float getEffectiveAlpha() const;

    /*!
    \brief
        Return a Rect that describes the rendering clipping rect based upon the
        outer rect area of the window.

    \note
        The area returned by this function gives you the correct clipping rect
        for rendering within the Window's outer rect area.  The area described
        may or may not correspond to the final visual clipping actually seen on
        the display; this is intentional and neccessary due to the way that
        imagery is cached under some configurations.
    */
    const Rectf& getOuterRectClipper() const;

    /*!
    \brief
        Return a Rect that describes the rendering clipping rect based upon the
        inner rect area of the window.

    \note
        The area returned by this function gives you the correct clipping rect
        for rendering within the Window's inner rect area.  The area described
        may or may not correspond to the final visual clipping actually seen on
        the display; this is intentional and neccessary due to the way that
        imagery is cached under some configurations.
    */
    const Rectf& getInnerRectClipper() const;

    /*!
    \brief
        Return a Rect that describes the rendering clipping rect for the Window.

        This function can return the clipping rect for either the inner or outer
        area dependant upon the boolean values passed in.

    \note
        The areas returned by this function gives you the correct clipping rects
        for rendering within the Window's areas.  The area described may or may
        not correspond to the final visual clipping actually seen on the
        display; this is intentional and neccessary due to the way that imagery
        is cached under some configurations.

    \param non_client
        - true to return the non-client clipping area (based on outer rect).
        - false to return the client clipping area (based on inner rect).
    */
    const Rectf& getClipRect(const bool non_client = false) const
    {
        return non_client ? getOuterRectClipper() : getInnerRectClipper();
    }

    //! helper function for calculating a parent clipping rectangle.
    Rectf getParentClipRect() const;

    /*!
    \brief
        Return the Rect that descibes the clipped screen area that is used for
        determining whether this window has been hit by a certain point.

        The area returned by this function may also be useful for certain
        calculations that require the clipped Window area as seen on the display
        as opposed to what is used for rendering (since the actual rendering
        clipper rects should not to be used if reliable results are desired).
    */
    const Rectf& getHitTestRect() const;

    /*!
    \brief
        return the Window that currently has inputs captured.

    \return
        Pointer to the Window object that currently has inputs captured, or NULL
        if no Window has captured input.
    */
    Window* getCaptureWindow() const;

    /*!
    \brief
        return true if this Window has input captured.

    \return
        - true if this Window has captured inputs.
        - false if some other Window, or no Window, has captured inputs.
    */
    bool isCapturedByThis() const { return getCaptureWindow() == this; }

    /*!
    \brief
        return true if an ancestor window has captured inputs.

    \return
        - true if input is captured by a Window that is some ancestor (parent,
          parent of parent, etc) of this Window.
        - false if no ancestor of this window has captured input.
    */
    bool isCapturedByAncestor() const { return isAncestor(getCaptureWindow()); }

    /*!
    \brief
        return true if a child window has captured inputs.

    \return
        - true if input is captured by a Window that is a child of this Window.
        - false if no child of this window has not captured input.
    */
    bool isCapturedByChild() const  {return isChild(getCaptureWindow());}

    /*!
    \brief
        check if the given pixel position would hit this window.

    \param position
        vec2 object describing the position to check.  The position
        describes a pixel offset from the top-left corner of the display.

    \param allow_disabled
        - true specifies that the window may be 'hit' if it is disabled.
        - false specifies that the window may only be hit if it is enabled.

    \return
        - true if \a position hits this Window.
        - false if \a position does not hit this window.
    */
    virtual bool isHit(const glm::vec2& position,
                       const bool allow_disabled = false) const;

    /*!
    \brief
        return the child Window that is hit by the given pixel position

    \param position
        vec2 object describing the position to check.  The position
        describes a pixel offset from the top-left corner of the display.

    \return
        Pointer to the child Window that was hit according to the location
        \a position, or 0 if no child of this window was hit.
    */
    Window* getChildAtPosition(const glm::vec2& position) const;

    /*!
    \brief
        return the child Window that is 'hit' by the given position, and is
        allowed to handle cursor events.

    \param position
        vec2 object describing the position to check.  The position
        describes a pixel offset from the top-left corner of the display.

    \param allow_disabled
        - true specifies that a disabled window may be returned as the target.
        - false specifies that only enabled windows may be returned.

    \param exclude
        if not null, excludes the specified window and its children from the test

    \return
        Pointer to the child Window that was hit according to the location
        \a position, or 0 if no child of this window was hit.
    */
    Window* getTargetChildAtPosition(const glm::vec2& position,
                                     const bool allow_disabled = false,
                                     const Window* const exclude = nullptr) const;

    /*!
    \brief
        return the parent of this Window.

    \return
        Pointer to the Window object that is the parent of this Window.
        This value can be NULL, in which case the Window is a GUI sheet / root.
    */
    inline Window* getParent() const
    {
        return static_cast<Window*>(getParentElement());
    }

    /*!
    \brief
        Return a pointer to the cursor image to use when the cursor
        indicator is within this window's area.

    \return
        Pointer to the cursor image that will be used when the cursor
        enters this window's area. May return NULL indicating no indicator will
        be drawn for this window.
    */
    const Image* getActualCursor() const;

    //! Returns the cursor set for this window, nullptr means that a default cursor will be used
    const Image* getCursor() const { return d_cursor; }

    /*!
    \brief
        Return the user data set for this Window.

        Each Window can have some client assigned data attached to it, this data
        is not used by the GUI system in any way.  Interpretation of the data is
        entirely application specific.

    \return
        pointer to the user data that is currently set for this window.
    */
    void* getUserData() const { return d_userData; }

    /*!
    \brief
        Return whether this window is set to restore old input capture when it
        loses input capture.

        This is only really useful for certain sub-components for widget
        writers.

    \return
        - true if the window will restore the previous capture window when it
          loses input capture.
        - false if the window will set the capture window to NULL when it loses
          input capture (this is the default behaviour).
    */
    bool restoresOldCapture() const { return d_restoreOldCapture; }

    /*!
    \brief
        Return whether z-order changes are enabled or disabled for this Window.

    \note
        This is distinguished from the is/setRiseOnClickEnabled setting in that
        if rise on click is disabled it only affects the users ability to affect
        the z order of the Window by clicking the cursor; is still possible to
        programmatic alter the Window z-order by calling the moveToFront,
        moveToBack, moveInFront and moveBehind member functions.  Whereas if z
        ordering is disabled those functions are also precluded from affecting
        the Window z position.

    \return
        - true if z-order changes are enabled for this window.
          moveToFront, moveToBack, moveInFront and moveBehind work normally.
        - false: z-order changes are disabled for this window.
          moveToFront, moveToBack, moveInFront and moveBehind are ignored.
    */
    inline bool isZOrderingEnabled() const
    {
        return d_zOrderingEnabled;
    }

    /*!
    \brief
        Return whether cursor down event autorepeat is enabled for this
        window.

    \note
        Clicks are not repeated because they require down & up.
    */
    inline bool isCursorAutoRepeatEnabled() const
    {
        return d_autoRepeat;
    }

    /*!
    \brief
        Return the current auto-repeat delay setting for this window.

    \return
        float value indicating the delay, in seconds, before the first repeat
        cursor press event will be triggered when autorepeat is enabled.
    */
    inline float getAutoRepeatDelay() const
    {
        return d_repeatDelay;
    }

    /*!
    \brief
        Return the current auto-repeat rate setting for this window.

    \return
        float value indicating the rate, in seconds, at which repeat cursor press
        events will be generated after the initial delay has expired.
    */
    inline float getAutoRepeatRate() const
    {
        return d_repeatRate;
    }

    /*!
    \brief
        Return whether the window wants inputs passed to its attached
        child windows when the window has inputs captured.

    \return
        - true if System should pass captured input events to child windows.
        - false if System should pass captured input events to this window only.
    */
    inline bool distributesCapturedInputs() const
    {
        return d_distCapturedInputs;
    }

    /*!
    \brief
        Return whether this Window is using the system default Tooltip for its
        Tooltip window.

    \return
        - true if the Window will use the system default tooltip.
        - false if the window has a custom Tooltip object.
    */
    inline bool isUsingDefaultTooltip() const
    {
        return !d_customTip;
    }

    /*!
    \brief
        Return a pointer to the Tooltip object used by this Window.  The value
        returned may point to the system default Tooltip, a custom Window
        specific Tooltip, or be NULL.

    \return
        Pointer to a Tooltip based object, or NULL.
    */
    Tooltip* getTooltip() const;

    /*!
    \brief
        Return the custom tooltip type.

    \return
        String object holding the current custom tooltip window type, or an
        empty string if no custom tooltip is set.
     */
    String getTooltipType() const;

    /*!
    \brief
        Return the current tooltip text set for this Window.

    \return
        String object holding the current tooltip text set for this window.
     */
    inline const String& getTooltipText() const
    {
        return d_tooltipText;
    }

    /*!
    \brief
        Return the current tooltip text set for this Window or the inherited one.
        If the tooltip text of this window is empty then the inherited value will
        be used instead.

    \return
        String object holding the current tooltip text of this window or the 
        tooltip text this window inherited.
     */
    const String& getTooltipTextIncludingInheritance() const;

    /*!
    \brief
        Return whether this window inherits Tooltip text from its parent when
        its own tooltip text is not set.

    \return
        - true if the window inherits tooltip text from its parent when its own
          text is not set.
        - false if the window does not inherit tooltip text from its parent
          (and shows no tooltip when no text is set).
     */
    inline bool inheritsTooltipText() const
    {
        return d_inheritsTipText;
    }

    /*!
    \brief
        Return whether this window will rise to the top of the z-order when
        activated with the left cursor source.

    \note
        This is distinguished from the is/setZOrderingEnabled setting in that
        if rise on click is disabled it only affects the users ability to affect
        the z order of the Window by activating the cursor; is still possible to
        programmatic alter the Window z-order by calling the moveToFront,
        moveToBack, moveInFront and moveBehind member functions.  Whereas if z
        ordering is disabled those functions are also precluded from affecting
        the Window z position.

    \return
        - true if the window will come to the top of other windows when the left
          cursor source is activated within its area.
        - false if the window does not change z-order position when the left
          cursor source is activated within its area.
     */
    bool isRiseOnPointerActivationEnabled() const { return d_riseOnPointerActivation; }

    /*!
    \brief
        Return the list of GeometryBuffer objects for this Window.

    \return
        Reference to the list of GeometryBuffer objects for this Window.
    */
    inline std::vector<GeometryBuffer*>& getGeometryBuffers()
    {
        return d_geometryBuffers;
    }
    
    /*!
    \brief
        Adds GeometryBuffers to the end of the list of GeometryBuffers of this Window.

    \param appendingGeomBuffers
        The GeometryBuffers that will be appended to the window's GeometryBuffers
    */
    void appendGeometryBuffers(std::vector<GeometryBuffer*>& geomBuffers);

    /*!
    \brief
        Get the name of the LookNFeel assigned to this window.

    \return
        String object holding the name of the look assigned to this window.
        Returns the empty string if no look is assigned.
    */
    inline const String& getLookNFeel() const { return d_lookName; }

    /*!
    \brief
        Get whether or not this Window is the modal target.

    \return
        Returns true if this Window is the modal target, otherwise false.
    */
    bool getModalState() const;

    /*!
    \brief
        Returns a named user string.

    \param name
        String object holding the name of the string to be returned.

    \return
        String object holding the data stored for the requested user string.

    \exception UnknownObjectException
        thrown if a user string named \a name does not exist.
    */
    const String& getUserString(const String& name) const;

    /*!
    \brief
        Return whether a user string with the specified name exists.

    \param name
        String object holding the name of the string to be checked.

    \return
        - true if a user string named \a name exists.
        - false if no such user string exists.
    */
    bool isUserStringDefined(const String& name) const;

    /*!
    \brief
        Returns the active sibling window.

        This searches the immediate children of this window's parent, and
        returns a pointer to the active window.  The method will return this if
        we are the immediate child of our parent that is active.  If our parent
        is not active, or if no immediate child of our parent is active then 0
        is returned.  If this window has no parent, and this window is not
        active then 0 is returned, else this is returned.

    \return
        A pointer to the immediate child window attached to our parent that is
        currently active, or 0 if no immediate child of our parent is active.
    */
    Window* getActiveSibling();

    /*!
    \brief
        Returns whether this window should ignore cursor event and pass them
        through to and other windows behind it. In effect making the window
        transparent to the cursor.

    \return
        true if cursor pass through is enabled.
        false if cursor pass through is not enabled.
    */
    bool isCursorPassThroughEnabled() const { return d_cursorPassThroughEnabled; }

    /*!
    \brief
        Returns whether this window is an auto window.

        An auto window is typically a Window object created automatically by
        CEGUI - for example to form part of a multi-element 'compound' widget.
    */
    bool isAutoWindow() const { return d_autoWindow; }

    /*!
    \brief
        Returns whether this window is allowed to write XML.
    */
    bool isWritingXMLAllowed() const { return d_allowWriteXML; }

    /*!
    \brief
        Returns whether this Window object will receive events generated by
        the drag and drop support in the system.

    \return
        - true if the Window is enabled as a drag and drop target.
        - false if the window is not enabled as a drag and drop target.
    */
    inline bool isDragDropTarget() const
    {
        return d_dragDropTarget;
    }

    /*!
    \brief
        Fill in the RenderingContext \a ctx with details of the RenderingSurface
        where this Window object should normally do it's rendering.
    */
    void getRenderingContext(RenderingContext& ctx) const;

    //! implementation of the default getRenderingContext logic.
    virtual void getRenderingContext_impl(RenderingContext& ctx) const;

    /*!
    \brief
        return the RenderingSurface currently set for this window.  May return
        0.
    */
    inline RenderingSurface* getRenderingSurface() const
    {
        return d_surface;
    }

    /*!
    \brief
        return the RenderingSurface that will be used by this window as the
        target for rendering.
    */
    RenderingSurface* getTargetRenderingSurface() const;

    /*!
    \brief
        Returns whether \e automatic use of an imagery caching RenderingSurface
        (i.e. a RenderingWindow) is enabled for this window.  The reason we
        emphasise 'automatic' is because the client may manually set a
        RenderingSurface that does exactly the same job.

    \return
        - true if automatic use of a caching RenderingSurface is enabled.
        - false if automatic use of a caching RenderTarget is not enabled.
    */
    inline bool isUsingAutoRenderingSurface() const
    {
        return d_autoRenderingWindow;
    }

    /*!
    \brief
        Returns whether the Window's texture caching (if activated) will have a stencil buffer
        attached or not. This may be required for proper rendering of SVG images and Custom Shapes.
        For example, polygons with internal overlap in SVG require this in order to be correctly rendered.

    \return
    - true to provide stencil buffer functionality with the texture caching.
    - false to not provide a stencil buffer functionality with the texture caching.
    */
    inline bool isAutoRenderingSurfaceStencilEnabled() const
    {
        return d_autoRenderingSurfaceStencilEnabled;
    }

    /*!
    \brief
        Returns the window at the root of the hierarchy starting at this
        Window.  The root window is defined as the first window back up the
        hierarchy that has no parent window.

    \return
        A pointer to the root window of the hierarchy that this window is
        attached to.
    */
    const Window* getRootWindow() const;
    Window* getRootWindow();

    /*!
    \brief
        Initialises the Window based object ready for use.

    \note
        This must be called for every window created.  Normally this is handled
        automatically by the WindowManager.
    */
    virtual void initialiseComponents() {}

    /*!
    \brief
        Set whether or not this Window will automatically be destroyed when its
        parent Window is destroyed.

    \param setting
        - true to have the Window auto-destroyed when its parent is destroyed
          (default behaviour)
        - false to have the Window remain after its parent is destroyed.

    \return
        Nothing
    */
    void setDestroyedByParent(bool setting);

    /*!
    \brief
        Set whether this window is enabled or disabled.  A disabled window
        normally can not be interacted with, and may have different rendering.

    \param setting
        - true to enable the Window
        - false to disable the Window.

    \return
        Nothing
    */
    virtual void setEnabled(bool enabled);

    /*!
    \brief
        Set whether this window is enabled or disabled.  A disabled window
        normally can not be interacted with, and may have different rendering.

    \param setting
        - true to disable the Window
        - false to enable the Window.
    */
    void setDisabled(bool disabled) { setEnabled(!disabled); }

    /*!
    \brief
        enable the Window to allow interaction.
    */
    void enable()   { setEnabled(true); }

    /*!
    \brief
        disable the Window to prevent interaction.
    */
    void disable()  { setEnabled(false); }

    /*!
    \brief
        Set whether the Window is visible or hidden.

    \param setting
        - true to make the Window visible.
        - false to make the Window hidden.

    \note
        Hiding the active window will cause that window to become deactivated.
        Showing a window does not, however, automatically cause that window to
        become the active window (call Window::activate after making the window
        visible to activate it).

    \return
        Nothing
    */
    void setVisible(bool setting);

    /*!
    \brief
        show the Window.

    \note
        Showing a window does not automatically activate the window.  If you
        want the window to also become active you will need to call the
        Window::activate member also.

    \return
        Nothing
    */
    void show() { setVisible(true); }

    /*!
    \brief
        hide the Window.
    \note
        If the window is the active window, it will become deactivated as a
        result of being hidden.

    \return
        Nothing
    */
    void hide() { setVisible(false); }

    /*!
    \brief
        Set whether the Window is active or inactive.

    \param setting
        - true to make the Window active.
        - false to make the Window inactive (deactivate).

    \note
        Activating the window will call to move the window to the front if the 
        window is not already active.  The window must be already visible in 
        order to activate it otherwise it will have no effect.  
        When deactivating, all active children will also be deactivated.

    \return
        Nothing
    */

    void setActive(bool setting);

    /*!
    \brief
        Activate the Window giving it input focus and bringing it to the top of
        all windows with the same always-on-top settig as this Window.

    \return
        Nothing
    */
    void activate() { activate_impl(false); }

    /*!
    \brief
        Deactivate the window. No further inputs will be received by the window
        until it is re-activated either programmatically or by the user
        interacting with the gui.
    */
    void deactivate();

    /*!
    \brief
        Set whether this Window will be clipped by its parent window(s).

    \param setting
        - true to have the Window clipped so that rendering is constrained to
          within the area of its parent(s).
        - false to have rendering constrained to the screen only.

    \return
        Nothing
    */
    void setClippedByParent(bool setting);

    /*!
    \brief
        Set the current ID for the Window.

    \param ID
        Client assigned ID code for this Window.  The GUI system assigns no
        meaning to any IDs, they are a device purely for client code usage.

    \return
        Nothing
    */
    void setID(unsigned int ID);

    /*!
    \brief
        Set the current text string for the Window.

    \param text
        String object containing the text that is to be set as the Window text.

    \return
        Nothing
    */
    void setText(const String& text);

    /*!
    \brief
        Insert the text string \a text into the current text string for the
        Window object at the position specified by \a position.

    \param text
        String object holding the text that is to be inserted into the Window
        object's current text string.

    \param position
        The characted index position where the string \a text should be
        inserted.
    */
    void insertText(const String& text, const String::size_type position);

    /*!
    \brief
        Append the string \a text to the currect text string for the Window
        object.

    \param text
        String object holding the text that is to be appended to the Window
        object's current text string.
    */
    void appendText(const String& text);

    /*!
    \brief
        Set the font used by this Window.

    \param font
        Pointer to the Font object to be used by this Window.
        If \a font is NULL, the default font will be used.

    \return
        Nothing
    */
    void setFont(const Font* font);

    /*!
    \brief
        Set the font used by this Window.

    \param name
        String object holding the name of the Font object to be used by this
        Window.  If \a name == "", the default font will be used.

    \return
        Nothing

    \exception UnknownObjectException
        thrown if the specified Font is unknown within the system.
    */
    void setFont(const String& name);

    using NamedElement::removeChild;
    /*!
    \brief
        Remove the first child Window with the specified ID.  If there is more
        than one attached Window objects with the specified ID, only the fist
        one encountered will be removed.

    \param ID
        ID number assigned to the Window to be removed.  If no Window with ID
        code \a ID is attached, nothing happens.

    \return
        Nothing.
    */
    void removeChild(unsigned int ID);

    /*!
    \brief
        Creates a child window attached to this window.

    \param type
        String that describes the type of Window to be created.  A valid
        WindowFactory for the specified type must be registered.

    \param name
        String that holds the name that is to be given to the new window.  If
        this string is empty, a name will be generated for the window.

    \return
        Pointer to the newly created child Window object.
    */
    Window* createChild(const String& type, const String& name = "");

    /*!
    \brief
        Destroys a child window of this window

    \param wnd
        The child window to destroy
    */
    void destroyChild(Window* wnd);

    /*!
    \brief
        Destroys a child window of this window

    \param name_path
        Name path that references the window to destroy
    */
    void destroyChild(const String& name_path);

    /*!
    \brief
        Move the Window to the top of the z order.

        - If the Window is a non always-on-top window it is moved the the top of
          all other non always-on-top sibling windows, and the process repeated
          for all ancestors.
        - If the Window is an always-on-top window it is moved to the of of all
          sibling Windows, and the process repeated for all ancestors.

    \return
        Nothing
    */
    void moveToFront();

    /*!
    \brief
        Move the Window to the bottom of the Z order.

        - If the window is non always-on-top the Window is sent to the very
          bottom of its sibling windows and the process repeated for all
          ancestors.
        - If the window is always-on-top, the Window is sent to the bottom of
          all sibling always-on-top windows and the process repeated for all
          ancestors.

    \return
        Nothing
    */
    void moveToBack();

    /*!
    \brief
        Move this window immediately above it's sibling \a window in the z order.

        No action will be taken under the following conditions:
        - \a window is 0.
        - \a window is not a sibling of this window.
        - \a window and this window have different AlwaysOnTop settings.
        - z-ordering is disabled for this window.

    \param window
        The sibling window that this window will be moved in front of.
    */
    void moveInFront(const Window* const window);

    /*!
    \brief
        Move this window immediately behind it's sibling \a window in the z
        order.

        No action will be taken under the following conditions:
        - \a window is 0.
        - \a window is not a sibling of this window.
        - \a window and this window have different AlwaysOnTop settings.
        - z-ordering is disabled for this window.

    \param window
        The sibling window that this window will be moved behind.
    */
    void moveBehind(const Window* const window);

    /*!
    \brief
        Return whether /a this Window is in front of the given window.

    \note
        Here 'in front' just means that one window is drawn after the other, it
        is not meant to imply that the windows are overlapping nor that one
        window is obscured by the other.
    */
    bool isInFront(const Window& wnd) const;

    /*!
    \brief
        Return whether /a this Window is behind the given window.

    \note
        Here 'behind' just means that one window is drawn before the other, it
        is not meant to imply that the windows are overlapping nor that one
        window is obscured by the other.
    */
    inline bool isBehind(const Window& wnd) const
    {
        return !isInFront(wnd);
    }

    /*!
    \brief
        Return the (visual) z index of the window on it's parent.

        The z index is a number that indicates the order that windows will be
        drawn (but is not a 'z co-ordinate', as such).  Higher numbers are in
        front of lower numbers.

        The number returned will not be stable, and generally should be used to
        compare with the z index of sibling windows (and only sibling windows)
        to discover the current z-ordering of those windows.
    */
    size_t getZIndex() const;

    /*!
    \brief
        Set whether this window is always on top, or not.

    \param setting
        - true to have the Window appear on top of all normal (non-topmost) windows.
        - false to allow the window to be covered by other normal windows.
    */
    void setAlwaysOnTop(bool setting);

    /*!
    \brief
        Captures input to this window

    \return
        - true if input was successfully captured to this window.
        - false if input could not be captured to this window
          (maybe because the window is not active).
    */
    bool captureInput();

    /*!
    \brief
        Releases input capture from this Window. If this Window does not have
        inputs captured, nothing happens.
    */
    void releaseInput();

    /*!
    \brief
        Set whether this window will remember and restore the previous window
        that had inputs captured.

    \param setting
        - true: The window will remember and restore the previous capture
          window.  The CaptureLost event is not fired on the previous window
          when this window steals input capture.  When this window releases
          capture, the old capture window is silently restored.

        - false: Input capture works as normal, each window losing capture is
          signalled via CaptureLost, and upon the final release of capture, no
          previous setting is restored (this is the default behaviour).
    */
    void setRestoreOldCapture(bool setting);

    /*!
    \brief
        Set the current alpha value for this window.

    \note
        The alpha value set for any given window may or may not be the final
        alpha value that is used when rendering.  All window objects, by
        default, inherit alpha from thier parent window(s) - this will blend
        child windows, relatively, down the line of inheritance.  This behaviour
        can be overridden via the setInheritsAlpha() method.  To return the true
        alpha value that will be applied when rendering, use the
        getEffectiveAlpha() method.

    \param alpha
        The new alpha value for the window.
        Value should be between 0.0f and 1.0f.
    */
    void setAlpha(const float alpha);

    /*!
    \brief
        Sets whether this Window will inherit alpha from its parent windows.

    \param setting
        - true if the Window should use inherited alpha.
        - false if the Window should have an independant alpha value.
    */
    void setInheritsAlpha(bool setting);

    /*!
    \brief
        Invalidate this window and - dependant upon \a recursive - all child
        content, causing affected windows to be redrawn during the next
        rendering pass.

    \param recursive
        Boolean value indicating whether attached child content should also be
        invalidated.
        - true will cause all child content to be invalidated also.
        - false will just invalidate this single window.
    */
    void invalidate(const bool recursive = false);

    /*!
    \brief
        Set the cursor image to be used when the cursor enters this window.

    \param image
        Pointer to the Image object to use as the cursor image when the
        cursor enters the area for this Window.
    */
    void setCursor(const Image* image);

    /*!
    \brief
        Set the cursor image to be used when the cursor enters this window.

    \param imageset
        String object that contains the name of the Imageset that contains the
        image to be used.

    \param name
        String object that contains the name of the Image to use.

    \exception UnknownObjectException
        thrown if no Image named \a name exists.
    */
    void setCursor(const String& name);

    /*!
    \brief
        Set the user data set for this Window.

        Each Window can have some client assigned data attached to it, this data
        is not used by the GUI system in any way.  Interpretation of the data is
        entirely application specific.

    \param user_data
        pointer to the user data that is to be set for this window.
    */
    void setUserData(void* user_data)   {d_userData = user_data;}

    /*!
    \brief
        Set whether z-order changes are enabled or disabled for this Window.

    \note
        This is distinguished from the is/setRiseOnClickEnabled setting in that
        if rise on click is disabled it only affects the users ability to affect
        the z order of the Window by activating the cursor; is still possible to
        programmatic alter the Window z-order by calling the moveToFront,
        moveToBack, moveInFront and moveBehind member functions.  Whereas if z
        ordering is disabled those functions are also precluded from affecting
        the Window z position.

    \param setting
        - true if z-order changes are enabled for this window.
          moveToFront, moveToBack, moveInFront and moveBehind work normally.
        - false: z-order changes are disabled for this window.
          moveToFront, moveToBack, moveInFront and moveBehind are ignored.
    */
    void    setZOrderingEnabled(bool setting);

    /*!
    \brief
        Set whether cursor down event autorepeat is enabled for this
        window.

    \note
        Clicks are not repeated because they require down & up.
    */
    void setCursorAutoRepeatEnabled(bool setting);

    /*!
    \brief
        Set the current auto-repeat delay setting for this window.

    \param delay
        float value indicating the delay, in seconds, before the first repeat
        cursor press event should be triggered when autorepeat is enabled.
    */
    void setAutoRepeatDelay(float delay);

    /*!
    \brief
        Set the current auto-repeat rate setting for this window.

    \param rate
        float value indicating the rate, in seconds, at which repeat cursor press
        events should be generated after the initial delay has expired.
    */
    void setAutoRepeatRate(float rate);

    /*!
    \brief
        Set whether the window wants inputs passed to its attached
        child windows when the window has inputs captured.

    \param setting
        - true if System should pass captured input events to child windows.
        - false if System should pass captured input events to this window only.
    */
    void setDistributesCapturedInputs(bool setting);

    /*!
    \brief
        Internal support method for drag & drop.  You do not normally call
        this directly from client code.  See the DragContainer class.
    */
    void notifyDragDropItemEnters(DragContainer* item);

    /*!
    \brief
        Internal support method for drag & drop.  You do not normally call
        this directly from client code.  See the DragContainer class.
    */
    void notifyDragDropItemLeaves(DragContainer* item);

    /*!
    \brief
        Internal support method for drag & drop.  You do not normally call
        this directly from client code.  See the DragContainer class.

    \return
        true if dropping is accepted, false otherwise. When false and sticky
        mode is active, dragging will not be terminated.
    */
    bool notifyDragDropItemDropped(DragContainer* item);

    /*!
    \brief
        Internal destroy method which actually just adds the window and any
        parent destructed child windows to the dead pool.

        This is virtual to allow for specialised cleanup which may be required
        in some advanced cases.  If you override this for the above reason, you
        MUST call this base class version.

    \note
        You never have to call this method yourself, use WindowManager to
        destroy your Window objects (which will call this for you).
    */
    virtual void destroy();

    /*!
    \brief
        Set the custom Tooltip object for this Window.  This value may be 0 to
        indicate that the Window should use the system default Tooltip object.

    \param tooltip
        Pointer to a valid Tooltip based object which should be used as the
        tooltip for this Window, or 0 to indicate that the Window should use the
        system default Tooltip object.  Note that when passing a pointer to a
        Tooltip object, ownership of the Tooltip does not pass to this Window
        object.

    \return
        Nothing.
    */
    void setTooltip(Tooltip* tooltip);

    /*!
    \brief
        Set the custom Tooltip to be used by this Window by specifying a Window
        type.

        The Window will internally attempt to create an instance of the
        specified window type (which must be derived from the base Tooltip
        class).  If the Tooltip creation fails, the error is logged and the
        Window will revert to using either the existing custom Tooltip or the
        system default Tooltip.

    \param tooltipType
        String object holding the name of the Tooltip based Window type which
        should be used as the Tooltip for this Window.

    \return
        Nothing.
    */
    void setTooltipType(const String& tooltipType);

    /*!
    \brief
        Set the tooltip text for this window.

    \param tip
        String object holding the text to be displayed in the tooltip for this
        Window.

    \return
        Nothing.
    */
    void setTooltipText(const String& tip);

    void setTooltipEnabled(bool enable) { d_tooltipEnabled = enable; }
    bool isTooltipEnabled() const { return d_tooltipEnabled; }

    /*!
    \brief
        Set whether this window inherits Tooltip text from its parent when its
        own tooltip text is not set.

    \param setting
        - true if the window should inherit tooltip text from its parent when
          its own text is not set.
        - false if the window should not inherit tooltip text from its parent
          (and so show no tooltip when no text is set).

    \return
        Nothing.
     */
    void setInheritsTooltipText(bool setting);

    /*!
    \brief
        Set whether this window will rise to the top of the z-order when clicked
        with the left cursor source.

    \note
        This is distinguished from the is/setZOrderingEnabled setting in that
        if rise on cursor activation is disabled it only affects the users ability to affect
        the z order of the Window by activating the left cursor source; is still
        possible to programmatic alter the Window z-order by calling the moveToFront,
        moveToBack, moveInFront and moveBehind member functions.  Whereas if z
        ordering is disabled those functions are also precluded from affecting
        the Window z position.

    \param setting
        - true if the window should come to the top of other windows when the
          left cursor source is activated within its area.
        - false if the window should not change z-order position when the left
          cursor source is activated within its area.

    \return
        Nothing.
     */
    void setRiseOnClickEnabled(bool setting)    { d_riseOnPointerActivation = setting; }

    /*!
    \brief
        Set the LookNFeel that shoule be used for this window.

    \param look
        String object holding the name of the look to be assigned to the window.

    \exception UnknownObjectException
        thrown if the look'n'feel specified by \a look does not exist.

    \exception NullObjectException
        thrown if the Window does not have a WindowRenderer assigned to it.
        see Window::setWindowRenderer.

    \note
        This is really intended as an internal function. The way that client
        code is supposed to use skins is by defining a Falagard mapping (either
        in a scheme xml file or in code) and then create instances of that
        mapped type via WindowManager.  See
        WindowFactoryManager::addFalagardWindowMapping and \ref xml_scheme.
        With that being said, it is possible for client code to use this
        function so long as you are aware of the implications of doing so:
        - Automatically created child windows (AutoWindows) will be deleted, and
          references or pointers you hold to these will become invalid.
        - Aside from those absolutely required, there is not guarantee that the
          newly assigned look will create the same set of child windows, nor
          that any created windows will be of any given type.
        - Any properties set on automatically created child windows after their
          creation will be lost - even if the new look creates a child of the
          same type with the same name.
    */
    virtual void setLookNFeel(const String& look);

    /*!
    \brief
        Set the modal state for this Window.

    \param state
        Boolean value defining if this Window should be the modal target.
        - true if this Window should be activated and set as the modal target.
        - false if the modal target should be cleared if this Window is
          currently the modal target.

    \return
        Nothing.
    */
    void setModalState(bool state);

    /*!
    \brief
       Sets the value a named user string, creating it as required.

    \param name
        String object holding the name of the string to be returned.

    \param value
        String object holding the value to be assigned to the user string.

    \return
        Nothing.
    */
    void setUserString(const String& name, const String& value);

    /*!
    \brief
        Draws the Window object and all of it's attached
        children to the display.
    \param drawModeMask
        Only if the specified draw mode mask matches any of the bit-flags active
        in the drawModeMask of this Window, the Window will be rendered as part
        of this call.

    */
    void draw(std::uint32_t drawModeMask = DrawModeMaskAll);

    /*!
    \brief
        Cause window to update itself and any attached children.  Client code
        does not need to call this method; to ensure full, and proper updates,
        call the injectTimePulse methodname method provided by the System class.

    \note
        The update order is such that 'this' window is updated prior to any
        child windows, this is so that child windows that access the parent in
        their update code get the correct updated state.

    \param elapsed
        float value indicating the number of seconds passed since the last
        update.

    \return
        Nothing.
    */
    virtual void update(float elapsed);

    /*!
    \brief
        Asks the widget to perform a clipboard copy to the provided clipboard

    \param clipboard
        Target clipboard class

    \return
        true if the copy was successful and allowed, false otherwise
    */
    virtual bool performCopy(Clipboard& clipboard);

    /*!
    \brief
        Asks the widget to perform a clipboard cut to the provided clipboard

    \param clipboard
        Target clipboard class

    \return
        true if the cut was successful and allowed, false otherwise
    */
    virtual bool performCut(Clipboard& clipboard);

    /*!
    \brief
        Asks the widget to perform a clipboard paste from the provided clipboard

    \param clipboard
        Source clipboard class

    \return
        true if the paste was successful and allowed, false otherwise
     */
    virtual bool performPaste(Clipboard& clipboard);

    /*!
    \brief
        Asks the widget to perform a undo operation

    \return
        true if the undo was successful and allowed, false otherwise
     */
    virtual bool performUndo();

    /*!
    \brief
        Asks the widget to perform a redo operation

    \return
        true if the redo was successful and allowed, false otherwise
     */
    virtual bool performRedo();

    /*!
    \brief
        Writes an xml representation of this window object to \a out_stream.

    \param xml_stream
        Stream where xml data should be output.

    \return
        Nothing.
    */
    virtual void writeXMLToStream(XMLSerializer& xml_stream) const;

    /*!
    \brief
        Sets the internal 'initialising' flag to true.
        This can be use to optimize initialisation of some widgets, and is called
        automatically by the layout XML handler when it has created a window.
        That is just after the window has been created, but before any children or
        properties are read.
    */
    virtual void beginInitialisation() { d_initialising = true; }

    /*!
    \brief
        Sets the internal 'initialising' flag to false.
        This is called automatically by the layout XML handler when it is done
        creating a window. That is after all properties and children have been
        loaded and just before the next sibling gets created.
    */
    virtual void endInitialisation();

    bool isInitializing() const { return d_initialising; }

    /*!
    \brief
        Returns an auto window with given name. Auto windows are created by
        the widget itself and typically can be seen as its parts. In the most
        cases auto windows are precreated and fetched by name here, but some
        widgets may create auto windows on demand based on \a name. The name
        of auto window created on demand may differ from the name passed.

    \param name
        String object holding the name of the child auto window to return.

    \return
        the Window object for the auto window referenced by \a name.
    */
    virtual Window* getChildAutoWindow(const String& name);

    /*!
    \brief
        Sets whether this window should ignore cursor events and pass them
        through to any windows behind it. In effect making the window
        transparent to the cursor.

    \param setting
        true if cursor pass through is enabled.
        false if cursor pass through is not enabled.
    */
    void setCursorPassThroughEnabled(bool setting)   {d_cursorPassThroughEnabled = setting;}

    /*!
    \brief
        Assign the WindowRenderer type to be used when rendering this window.

    \param name
        The factory name of the WindowRenderer to use.

    \note
        This is really intended as an internal function. The way that client
        code is supposed to use skins is by defining a Falagard mapping (either
        in a scheme xml file or in code) and then create instances of that
        mapped type via WindowManager.  See
        WindowFactoryManager::addFalagardWindowMapping and \ref xml_scheme.
    */
    void setWindowRenderer(const String& name);

    /*!
    \brief
        Get the currently assigned WindowRenderer. (Look'N'Feel specification).

    \return
        A pointer to the assigned window renderer object.
        0 if no window renderer is assigned.
    */
    inline WindowRenderer* getWindowRenderer() const
    {
        return d_windowRenderer;
    }

    /*!
    \brief
        Get the factory name of the currently assigned WindowRenderer.
        (Look'N'Feel specification).

    \return
        The factory name of the currently assigned WindowRenderer.
        If no WindowRenderer is assigned an empty string is returned.
    */
    const String& getWindowRendererName() const;

    /*!
    \brief
        Sets whether this window is allowed to write XML
    */
    void setWritingXMLAllowed(bool allow)   {d_allowWriteXML = allow;}

    /*!
    \brief
        Changes the widget's falagard type, thus changing its look'n'feel and optionally its
        renderer in the process.

    \param type
        New look'n'feel of the widget

    \param type
        New renderer of the widget
    */
    void setFalagardType(const String& type, const String& rendererType = "");

    /*!
    \brief
        Specifies whether this Window object will receive events generated by
        the drag and drop support in the system.

    \param setting
        - true to enable the Window as a drag and drop target.
        - false to disable the Window as a drag and drop target.
    */
    void setDragDropTarget(bool setting);

    /*!
    \brief
        Set the RenderingSurface to be associated with this Window, or 0 if
        none is required.
    \par
        If this function is called, and the option for automatic use of an
        imagery caching RenderingSurface is enabled, any automatically created
        RenderingSurface will be released, and the affore mentioned option will
        be disabled.
    \par
        If after having set a custom RenderingSurface you then subsequently
        enable the automatic use of an imagery caching RenderingSurface by
        calling setUsingAutoRenderingSurface, the previously set
        RenderingSurface will be disassociated from the Window.  Note that the
        previous RenderingSurface is not destroyed or cleaned up at all - this
        is the job of whoever created that object initially.

    \param target
        Pointer to the RenderingSurface object to be associated with the window.
    */
    void setRenderingSurface(RenderingSurface* surface);

    /*!
    \brief
        Invalidate the chain of rendering surfaces from this window backwards to
        ensure they get properly redrawn - but doing the minimum amount of work
        possibe - next render.
    */
    void invalidateRenderingSurface();

    /*!
    \brief
        Sets whether \e automatic use of an imagery caching RenderingSurface
        (i.e. a RenderingWindow) is enabled for this window.  The reason we
        emphasise 'atutomatic' is because the client may manually set a
        RenderingSurface that does exactlythe same job.
    \par
        Note that this setting really only controls whether the Window
        automatically creates and manages the RenderingSurface, as opposed to
        the \e use of the RenderingSurface. If a RenderingSurface is set for the
        Window it will be used regardless of this setting.
    \par
        Enabling this option will cause the Window to attempt to create a
        suitable RenderingSurface (which will actually be a RenderingWindow).
        If there is an existing RenderingSurface assocated with this Window, it
        will be removed as the Window's RenderingSurface
        <em>but not destroyed</em>; whoever created the RenderingSurface in the
        first place should take care of its destruction.
    \par
        Disabling this option will cause any automatically created
        RenderingSurface to be released.
    \par
        It is possible that the renderer in use may not support facilities for
        RenderingSurfaces that are suitable for full imagery caching.  If this
        is the case, then calling getRenderingSurface after enabling this option
        will return 0.  In these cases this option will still show as being
        'enabled', this is because Window \e settings should not be influenced
        by capabilities the renderer in use; for example, this enables correct
        XML layouts to be written from a Window on a system that does not
        support such RenderingSurfaces, so that the layout will function as
        preferred on systems that do.
    \par
        If this option is enabled, and the client subsequently assigns a
        different RenderingSurface to the Window, the existing automatically
        created RenderingSurface will be released and this setting will be
        disabled.

    \param setting
        - true to enable automatic use of an imagery caching RenderingSurface.
        - false to disable automatic use of an imagery caching RenderingSurface.
    */
    void setUsingAutoRenderingSurface(bool setting);

    /*!
    \brief
        Sets whether the Window's texture caching (if activated) will have a stencil buffer
        attached or not. This may be required for proper rendering of SVG images and Custom Shapes.
        For example, polygons with internal overlap in SVG require this in order to be correctly rendered.

    \param setting
        - true to provide stencil buffer functionality with the texture caching.
        - false to not provide a stencil buffer functionality with the texture caching.
    */
    void setAutoRenderingSurfaceStencilEnabled(bool setting);
  

    //! Return the parsed RenderedString object for this window.
    const RenderedString& getRenderedString() const;
    //! Return a pointer to any custom RenderedStringParser set, or 0 if none.
    inline RenderedStringParser* getCustomRenderedStringParser() const
    {
        return d_customStringParser;
    }
    //! Set a custom RenderedStringParser, or 0 to remove an existing one.
    void setCustomRenderedStringParser(RenderedStringParser* parser);
    //! return the active RenderedStringParser to be used
    virtual RenderedStringParser& getRenderedStringParser() const;
    //! return whether text parsing is enabled for this window.
    inline bool isTextParsingEnabled() const
    {
        return d_textParsingEnabled;
    }
    //! set whether text parsing is enabled for this window.
    void setTextParsingEnabled(const bool setting);

    //! set margin
    virtual void setMargin(const UBox& margin);
    //! retrieves currently set margin
    inline const UBox& getMargin() const
    {
        return d_margin;
    }

    //! return glm::vec2 \a pos after being fully unprojected for this Window.
    glm::vec2 getUnprojectedPosition(const glm::vec2& pos) const;

#ifdef CEGUI_BIDI_SUPPORT
    //! return the pointer to the BidiVisualMapping for this window, if any.
    const BidiVisualMapping* getBidiVisualMapping() const
        {return d_bidiVisualMapping;}
#endif

    /*!
    \brief
        Adds the named property to the XML ban list for this window
        Essentially a property that is banned from XML will never end up being saved to it.
        This is very useful if 2 properties overlap (XPosition and Position for example).

        Please note that properties that are not writable (read-only properties) are
        implicitly/automatically banned from XML, no need to ban them manually.

    \param property_name 
        Name of the property you want to ban
    */
    void banPropertyFromXML(const String& property_name);

    /*!
    \brief
        Adds the named property to the XML ban list for this window and all of its child windows.
        Essentially a property that is banned from XML will never end up being saved to it.
        This is very useful if 2 properties overlap (XPosition and Position for example).

        Please note that properties that are not writable (read-only properties) are
        implicitly/automatically banned from XML, no need to ban them manually.

    \param property_name 
        Name of the property you want to ban
    */
    void banPropertyFromXMLRecursive(const String& property_name);

    /*!
    \brief
        Removes the named property from the XML ban list for this window.
        Essentially a property that is banned from XML will never end up being saved to it.

        Please note that properties that are not writable (read-only properties) are
        implicitly/automatically banned from XML.

    \param property_name 
        Name of the property you want to unban
    */
    void unbanPropertyFromXML(const String& property_name);

    /*!
    \brief
        Removes the named property from the XML ban list for this window and all of its child windows.
        Essentially a property that is banned from XML will never end up being saved to it.

        Please note that properties that are not writable (read-only properties) are
        implicitly/automatically banned from XML.

    \param property_name 
        Name of the property you want to unban
    */
    void unbanPropertyFromXMLRecursive(const String& property_name);
    

    /*!
    \brief
        Return whether the named property is banned from XML

    \note
        Read-only properties and properties that can't write to XML streams
        are implicitly banned. This method will return true for them.
    */
    bool isPropertyBannedFromXML(const String& property_name) const;

    //! Add the given property to the XML ban list for this window.
    void banPropertyFromXML(const Property* property);

    //! Remove the given property from the XML ban list for this window.
    void unbanPropertyFromXML(const Property* property);

    /*!
    \brief
        Return whether given property is banned from XML

    \note
        Read-only properties and properties that can't write to XML streams
        are implicitly banned. This method will return true for them.
    */
    bool isPropertyBannedFromXML(const Property* property) const;

    /*!
    \brief
        Set the window update mode.  This mode controls the behaviour of the
        Window::update member function such that updates are processed for
        this window (and therefore it's child content) according to the set
        mode.

    \note
        Disabling updates can have negative effects on the behaviour of CEGUI
        windows and widgets; updates should be disabled selectively and
        cautiously - if you are unsure of what you are doing, leave the mode
        set to WindowUpdateMode::ALWAYS.

    \param mode
        One of the WindowUpdateMode enumerated values indicating the mode to
        set for this Window.
    */
    void setUpdateMode(const WindowUpdateMode mode);

    /*!
    \brief
        Return the current window update mode that is set for this Window.
        This mode controls the behaviour of the Window::update member function
        such that updates are processed for this window (and therefore it's
        child content) according to the set mode.

    \note
        Disabling updates can have negative effects on the behaviour of CEGUI
        windows and widgets; updates should be disabled selectively and
        cautiously - if you are unsure of what you are doing, leave the mode
        set to WindowUpdateMode::ALWAYS.

    \return
        One of the WindowUpdateMode enumerated values indicating the current
        mode set for this Window.
    */
    inline WindowUpdateMode getUpdateMode() const
    {
        return d_updateMode;
    }

    /*!
    \brief
        Set whether cursor input that is not directly handled by this Window
        (including it's event subscribers) should be propagated back to the
        Window's parent.
    */
    void setCursorInputPropagationEnabled(const bool enabled);

    /*!
    \brief
        Return whether cursor input that is not directly handled by this Window
        (including it's event subscribers) should be propagated back to the
        Window's parent.
    */
    inline bool isCursorInputPropagationEnabled() const
    {
        return d_propagatePointerInputs;
    }

    /*!
    \brief
        Clones this Window and returns the result

    \param
        deepCopy if true, even children are copied

    \return
        the cloned Window
    */
    Window* clone(const bool deepCopy = true) const;

    //! copies this widget's properties to given target widget
    virtual void clonePropertiesTo(Window& target) const;
    //! copies this widget's child widgets to given target widget
    virtual void cloneChildWidgetsTo(Window& target) const;

    //! return the GUIContext this window is associated with.
    GUIContext& getGUIContext() const;
    //! function used internally.  Do not call this from client code.
    void setGUIContext(GUIContext* context);

    /*!
    \brief
        Set whether this window is marked as an auto window.

        An auto window is typically a Window object created automatically by
        CEGUI - for example to form part of a multi-element 'compound' widget.
    */
    void setAutoWindow(bool is_auto);

    /*!
    \brief
        Return whether Window thinks cursor is currently within its area.

    \note
        If the cursor has moved or Window's area has changed since the
        last time the GUIContext updated the window hit information, the value
        returned here may be inaccurate - this is not a bug, but is required
        to ensure correct handling of certain events.
    */
    inline bool isPointerContainedInArea() const
    {
        return d_containsPointer;
    }

    // overridden from Element
    Sizef getRootContainerSize() const override;

    /*!
    \brief
        Return whether this Window is focused or not.

        A window is focused when it is the active Window inside the current
        GUIContext.
    */
    inline bool isFocused() const
    {
        return d_isFocused;
    }

    /*!
    \brief
        Makes this Window be focused.

        Focusing a Window means activating it and setting the focused flag.
        This will also trigger the activated event. Focusing works only on
        non-disabled widgets.
    */
    void focus();

    /*!
    \brief
        Unfocus this Window.

        This will trigger the deactivated event if this was an active window.
    */
    void unfocus();

    /*!
    \brief
        Return whether Window can be focused or not.

        A Window cannot be usually focused when it's disabled. Other widgets
        can override this method based on their own behaviour.
    */
    virtual bool canFocus() const;
    
    /*!
    \brief
        Sets the DrawMode bitmask for this Window.

        The DrawMode of this Window specifies when the Window should be
        drawn. The bitmask of this window is checked against the mask supplied
        in the draw call in that case.
    \param drawMode
        The drawMode bitmask to be set for this Window.
    */
    void setDrawModeMask(std::uint32_t drawMode);

    /*!
    \brief
        Gets the DrawMode bitmask of this Window.

        The DrawMode of this Window specifies when the Window should be
        drawn. The bitmask of this window is checked against the mask supplied
        in the draw call in that case.
    \return
        The drawMode bitmask that is set for this Window.
    */
    inline std::uint32_t getDrawModeMask() const
    {
        return d_drawModeMask;
    }

    /*!
    \brief
        Checks if the "DrawMode" property of this window is compatible with
        the drawMode bitmask that is supplied-

    \param drawMode
        The "DrawMode" bitmask to check this window's bitmask against.

    \return
        True if a bitwise and between the masks return non-zero.
    */
    inline bool checkIfDrawMaskAllowsDrawing(std::uint32_t drawModeMask) const
    {
        return (getDrawModeMask() & drawModeMask) != 0;
    }

    float getContentWidth() const override;
    float getContentHeight() const override;
    UDim getWidthOfAreaReservedForContentLowerBoundAsFuncOfElementWidth() const override;
    UDim getHeightOfAreaReservedForContentLowerBoundAsFuncOfElementHeight() const override;
    void adjustSizeToContent() override;
    bool contentFitsForSpecifiedElementSize(const Sizef& element_size) const override;
    bool contentFits() const override;

    /*!
    \brief
        Layout child windows inside our content areas.

        Laying out of child content includes:
            - ensuring content specified in any assigned WidgetLook has its area
              rectangles sychronised.
            - assigned WindowRenderer given the opportunity to update child
              content areas as needed.
            - Children are then updated recursively depending on flags passed.

        The system may call this at various times (like when a window is resized
        for example), and it may be invoked directly where required.

    \param client
        - true to process client children

    \param nonClient
        - true to process non-client children
    */
    virtual void performChildLayout(bool client, bool nonClient) override;

protected:
    // friend classes for construction / initialisation purposes (for now)
    friend class WindowManager; // FIXME for d_falagardType only
    friend class GUIContext;

    /*************************************************************************
        Event trigger methods
    *************************************************************************/

    //! \copydoc Element::handleAreaChanges
    virtual uint8_t handleAreaChanges(bool moved, bool sized) override;

    /*!
    \brief
        Handler called when the window's position changes.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    void onMoved(ElementEventArgs& e) override;

    void onRotated(ElementEventArgs& e) override;

    /*!
    \brief
        Handler called when the window's text is changed.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onTextChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window's font is changed.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onFontChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window's alpha blend value is changed.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onAlphaChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window's client assigned ID is changed.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onIDChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window is shown (made visible).

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onShown(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window is hidden.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onHidden(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window is enabled.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onEnabled(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window is disabled.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onDisabled(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window's setting for being clipped by it's
        parent is changed.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onClippingChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window's setting for being destroyed
        automatically be it's parent is changed.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onParentDestroyChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window's setting for inheriting alpha-blending
        is changed.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onInheritsAlphaChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window's always-on-top setting is changed.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onAlwaysOnTopChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when this window gains capture of cursor inputs.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onCaptureGained(WindowEventArgs& e);

    /*!
    \brief
        Handler called when this window loses capture of cursor inputs.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onCaptureLost(WindowEventArgs& e);

    /*!
    \brief
        Handler called when this window gets invalidated.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onInvalidated(WindowEventArgs& e);

    /*!
    \brief
        Handler called when rendering for this window has started.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onRenderingStarted(WindowEventArgs& e);

    /*!
    \brief
        Handler called when rendering for this window has ended.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onRenderingEnded(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the z-order position of this window has changed.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onZChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when this window's destruction sequence has begun.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onDestructionStarted(WindowEventArgs& e);

    /*!
    \brief
        Handler called when this window has become the active window.

    \param e
        ActivationEventArgs class whose 'otherWindow' field is set to the window
        that previously was active, or NULL for none.
    */
    virtual void onActivated(ActivationEventArgs& e);

    /*!
    \brief
        Handler called when this window has lost input focus and has been
        deactivated.

    \param e
        ActivationEventArgs object whose 'otherWindow' field is set to the
        window that has now become active, or NULL for none.
    */
    virtual void onDeactivated(ActivationEventArgs& e);

    /*!
    \brief
        Handler called when a child window is added to this window.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that has been added.
    */
    void onChildAdded(ElementEventArgs& e) override;

    /*!
    \brief
        Handler called when a child window is removed from this window.

    \param e
        WindowEventArgs object whose 'window' pointer field is set the window
        that has been removed.
    */
    void onChildRemoved(ElementEventArgs& e) override;

    /*!
    \brief
        Handler called when the cursor has entered this window's area.

    \param e
        CursorInputEventArgs object.  All fields are valid.
    */
    virtual void onCursorEntersArea(CursorInputEventArgs& e);

    /*!
    \brief
        Handler called when the cursor has left this window's area.

    \param e
        CursorInputEventArgs object.  All fields are valid.
    */
    virtual void onCursorLeavesArea(CursorInputEventArgs& e);

    /*!
    \brief
        Handler called when the cursor has entered this window's area and
        is actually over some part of this windows surface and not, for
        instance over a child window - even though technically in those cases
        the cursor is also within this Window's area, the handler will not be
        called.

    \param e
        CursorInputEventArgs object.  All fields are valid.

    \see
        Window::onCursorEntersArea
    */
    virtual void onCursorEnters(CursorInputEventArgs& e);

    /*!
    \brief
        Handler called when the cursor is no longer over this window's
        surface area.  This will be called when the cursor is not over a part
        of this Window's actual surface - even though technically the cursor is
        still within the Window's area, for example if the cursor moves over a
        child window.

    \param e
        CursorInputEventArgs object.  All fields are valid.

    \see
        Window::onCursorLeavesArea
    */
    virtual void onCursorLeaves(CursorInputEventArgs& e);

    /*!
    \brief
        Handler called when the cursor has been moved within this window's area.

    \param e
        CursorInputEventArgs object.  All fields are valid.
    */
    virtual void onCursorMove(CursorInputEventArgs& e);

    /*!
    \brief
        Handler called when the cursor scroll value changes within
        this window's area.

    \param e
        CursorInputEventArgs object.  All fields are valid.
    */
    virtual void onScroll(CursorInputEventArgs& e);

    /*!
    \brief
        Handler called when a cursor is held pressed within this window's area.

    \param e
        CursorInputEventArgs object.  All fields are valid.
    */
    virtual void onCursorPressHold(CursorInputEventArgs& e);
    
    /*!
    \brief
        Handler called when a cursor is activated twice within this window's area.

    \param e
        CursorInputEventArgs object.  All fields are valid.
    */
    virtual void onSelectWord(CursorInputEventArgs& e);

    /*!
    \brief
        Handler called when a cursor is activated three times within this window's area.

    \param e
        CursorInputEventArgs object.  All fields are valid.
    */
    virtual void onSelectAll(CursorInputEventArgs& e);

    /*!
    \brief
        Handler called when a cursor is activated within this window's area.

    \param e
        CursorInputEventArgs object.  All fields are valid.
    */
    virtual void onCursorActivate(CursorInputEventArgs& e);

    /*!
    \brief
        Handler called when a character has been injected while this window
        has input focus.

    \param e
        TextEventArgs object whose 'character' field is set to the Unicode code
        point (encoded as UTF-32) for the character inputted.
    */
    virtual void onCharacter(TextEventArgs& e);

    /*!
    \brief
        Handler called when a semantic input event occurred

    \param e
        The semantic input event
    */
    virtual void onSemanticInputEvent(SemanticEventArgs& e);

    /*!
    \brief
        Handler called when a DragContainer is started being dragged over this window.

    \param e
        DragDropEventArgs object initialised as follows:
        - window field is normaly set to point to 'this' window.
        - dragDropItem is a pointer to a DragContainer window that triggered
          the event.
    */
    virtual void onDragDropItemEnters(DragDropEventArgs& e);

    /*!
    \brief
        Handler called when a DragContainer is ended being dragged over this window.

    \param e
        DragDropEventArgs object initialised as follows:
        - window field is normaly set to point to 'this' window.
        - dragDropItem is a pointer to a DragContainer window that triggered
          the event.
    */
    virtual void onDragDropItemLeaves(DragDropEventArgs& e);

    /*!
    \brief
        Handler called when a DragContainer is dropped on this window.

    \param e
        DragDropEventArgs object initialised as follows:
        - window field is normaly set to point to 'this' window.
        - dragDropItem is a pointer to a DragContainer window that triggered
          the event.
    */
    virtual void onDragDropItemDropped(DragDropEventArgs& e);

    /*!
    \brief
        Handler called when a new window renderer object is attached.

    \param e
        WindowEventArgs object initialised as follows:
        - window field is set to point to the Window object that just got a new
          window renderer attached. (typically 'this').
    */
    virtual void onWindowRendererAttached(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the currently attached window renderer object is detached.

    \param e
        WindowEventArgs object initialised as follows:
        - window field is set to point to the Window object that just got lost its
          window renderer. (typically 'this').
    */
    virtual void onWindowRendererDetached(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window's setting for whether text parsing is
        enabled is changed.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onTextParsingChanged(WindowEventArgs& e);

    virtual void onMarginChanged(WindowEventArgs& e);

    /*************************************************************************
        Implementation Functions
    *************************************************************************/
    /*!
    \brief
        Perform actual update processing for this Window.

    \param elapsed
        float value indicating the number of seconds elapsed since the last
        update call.

    \return
        Nothing.
    */
    virtual void updateSelf(float elapsed);

    /*!
    \brief
        Perform the actual rendering for this Window.

    \param ctx
        RenderingContext holding the details of the RenderingSurface to be
        used for the Window rendering operations.

    \return
        Nothing
    */
    virtual void drawSelf(const RenderingContext& ctx, std::uint32_t drawModeMask);

    /*!
    \brief
        Perform drawing operations concerned with generating and buffering
        window geometry.

    \note
        This function is a sub-function of drawSelf; it is provided to make it
        easier to override drawSelf without needing to duplicate large sections
        of the code from the default implementation.
    */
    void bufferGeometry(const RenderingContext& ctx, std::uint32_t drawModeMask);

    /*!
    \brief
        Perform drawing operations concerned with positioning, clipping and
        queueing of window geometry to RenderingSurfaces.

    \note
        This function is a sub-function of drawSelf and is provided to make it
        easier to override drawSelf without needing to duplicate large sections
        of the code from the default implementation.
    */
    void queueGeometry(const RenderingContext& ctx);

    /*!
    \brief
        Destroys the geometry buffers of this Window.
    */
    void destroyGeometryBuffers();

    /*!
    \brief
        Update the rendering cache.

        Populates the Window's GeometryBuffer ready for rendering.
    */
    virtual void populateGeometryBuffer()  {}

    /*!
    \brief
        Fires off a repeated cursor press event for this window.
    */
    void generateAutoRepeatEvent(CursorInputSource source);

    /*!
    \brief
        Function used in checking if a WindowRenderer is valid for this window.

    \param renderer
        Window renderer that will be checked (it can be null!)

    \return
        Returns true if the given WindowRenderer class name is valid for this window.
        False if not.
    */
    virtual bool validateWindowRenderer(const WindowRenderer* renderer) const;

    /*!
    \brief
        Returns whether a property is at it's default value.
        This function is different from Property::isDefatult as it takes the assigned look'n'feel
        (if the is one) into account.
    */
    bool isPropertyAtDefault(const Property* property) const;

    /*!
    \brief
        Recursively inform all children that the clipping has changed and screen rects
        need to be recached.
    */
    void notifyClippingChanged();

    //! notify windows in a hierarchy when the default font changes
    void notifyDefaultFontChanged();

    /*!
    \brief
        Recursively updates all rendering surfaces and windows to work with a new host surface.
    */
    virtual void onTargetSurfaceChanged(RenderingSurface* newSurface);

    //! return the GUIContext this window is associated with.
    GUIContext* getGUIContextPtr() const;

    //! helper to create and setup the auto RenderingWindow surface
    void allocateRenderingWindow(bool addStencilBuffer);

    //! helper to clean up the auto RenderingWindow surface
    void releaseRenderingWindow();

    /*!
    \brief
        Cleanup child windows
    */
    virtual void cleanupChildren();

    /*!
    \copydoc Element::addChild_impl
    */
    void addChild_impl(Element* element) override;

    /*!
    \copydoc Element::removeChild_impl
    */
    void removeChild_impl(Element* element) override;

    /*!
    \brief
        Notify 'this' and all siblings of a ZOrder change event
    */
    virtual void onZChange_impl();

    /*!
    \brief
        Add standard CEGUI::Window properties.
    */
    void    addWindowProperties();

    /*!
    \brief
        A helper function for internal activation logic.
    */
    bool activate_impl(bool byClick);

    /*!
    \brief
        Implements move to front behavior.

    \return
        Should return true if some action was taken, or false if there was
        nothing to be done.
    */
    virtual bool moveToFront_impl(bool byClick);

    /*!
    \brief
        Add the given window to the drawing list at an appropriate position for
        it's settings and the required direction.  Basically, when \a at_back
        is false, the window will appear in front of all other windows with the
        same 'always on top' setting.  When \a at_back is true, the window will
        appear behind all other windows wih the same 'always on top' setting.

    \param wnd
        Window object to be added to the drawing list.

    \param at_back
        Indicates whether the window should be placed at the back of other
        windows in the same group. If this is false, the window is placed in
        front of other windows in the group.

    \return
        Nothing.
    */
    void addWindowToDrawList(Window& wnd, bool at_back = false);

    /*!
    \brief
        Removes the window from the drawing list.  If the window is not attached
        to the drawing list then nothing happens.

    \param wnd
        Window object to be removed from the drawing list.

    \return
        Nothing.
    */
    void removeWindowFromDrawList(const Window& wnd);

    /*!
    \brief
        Return whether the window is at the top of the Z-Order.  This will
        correctly take into account 'Always on top' windows as needed.

    \return
        - true if the Window is at the top of the z-order in relation to sibling
          windows with the same 'always on top' setting.
        - false if the Window is not at the top of the z-order in relation to
          sibling windows with the same 'always on top' setting.
    */
    bool isTopOfZOrder() const;

    //! transfer RenderingSurfaces to be owned by our target RenderingSurface.
    void transferChildSurfaces();

    //! helper function to invalidate window and optionally child windows.
    void invalidate_impl(const bool recursive);

    /*!
    \brief
        Helper function to return the ancestor Window of /a wnd that is attached
        as a child to a window that is also an ancestor of /a this.  Returns 0
        if /a wnd and /a this are not part of the same hierachy.
     */
    const Window* getWindowAttachedToCommonAncestor(const Window& wnd) const;

    Rectf getUnclippedInnerRect_impl(bool skipAllPixelAlignment) const override;
    //! Default implementation of function to return Window outer clipper area.
    virtual Rectf getOuterRectClipper_impl() const;
    //! Default implementation of function to return Window inner clipper area.
    virtual Rectf getInnerRectClipper_impl() const;
    //! Default implementation of function to return Window hit-test area.
    virtual Rectf getHitTestRect_impl() const;

    virtual int writePropertiesXML(XMLSerializer& xml_stream) const;
    virtual int writeChildWindowsXML(XMLSerializer& xml_stream) const;
    virtual bool writeAutoChildWindowXML(XMLSerializer& xml_stream) const;

    virtual void banPropertiesForAutoWindow();

    //! handler function for when font render size changes.
    virtual bool handleFontRenderSizeChange(const EventArgs& args);

    Window* getChildAtPosition(const glm::vec2& position,
                               bool (Window::*hittestfunc)(const glm::vec2&, bool)
                               #ifndef SWIG
                               const
                               #endif
                               ,
                               bool allow_disabled = false,
                               const Window* const exclude = nullptr) const;

    bool isHitTargetWindow(const glm::vec2& position, bool allow_disabled) const;

    /*************************************************************************
        Properties for Window base class
    *************************************************************************/

    /*!
    \brief
        Property to access/change the assigned window renderer object.

        \par Usage:
            - Name: WindowRenderer
            - Format: "[windowRendererName]"

        \par Where [windowRendererName] is the factory name of the window
             renderer type you wish to assign.
    */
    static
    class WindowRendererProperty : public TplWindowProperty<Window, String>
    {
    public:
        WindowRendererProperty();
        void writeXMLToStream(const PropertyReceiver* receiver,
                              XMLSerializer& xml_stream) const override;
    } d_windowRendererProperty;

    /*!
    \brief
        Property to access/change the assigned look'n'feel.

        \par Usage:
            - Name: LookNFeel
            - Format: "[LookNFeelName]"

        \par Where [LookNFeelName] is the name of the look'n'feel you wish
             to assign.
    */
    static
    class LookNFeelProperty : public TplWindowProperty<Window, String>
    {
    public:
        LookNFeelProperty();
        void writeXMLToStream(const PropertyReceiver* receiver,
                              XMLSerializer& xml_stream) const override;
    } d_lookNFeelProperty;

    /*************************************************************************
        Implementation Data
    *************************************************************************/

    //! GUIContext.  Set when this window is used as a root window.
    GUIContext* d_guiContext;
    //! The WindowRenderer module that implements the Look'N'Feel specification
    WindowRenderer* d_windowRenderer;
    //! RenderingSurface owned by this window (may be 0)
    RenderingSurface* d_surface;
    //! Holds pointer to the Window objects current cursor image.
    const Image* d_cursor;
    //! Possible custom Tooltip for this window.
    Tooltip* d_customTip;
    //! Holds pointer to the Window objects current Font.
    const Font* d_font;
    //! Pointer to a custom (user assigned) RenderedStringParser object.
    RenderedStringParser* d_customStringParser;
    //! The Window that previously had capture (used for restoreOldCapture mode)
    Window* d_oldCapture;
    //! Holds pointer to some user assigned data.
    void* d_userData;

#ifdef CEGUI_BIDI_SUPPORT
    //! pointer to bidirection support object
    BidiVisualMapping* d_bidiVisualMapping;
#endif

#ifdef CEGUI_USE_RAQM
    //! raqm text object
    std::unique_ptr<RaqmTextData> d_raqmTextData;
#endif

    // FIXME: why GUI context doesn't propagate this to its window hierarchy?
    //! connection for event listener for font render size changes.
    Event::ScopedConnection d_fontRenderSizeChangeConnection;

    //! outer area clipping rect in screen pixels
    mutable Rectf d_outerRectClipper;
    //! inner area clipping rect in screen pixels
    mutable Rectf d_innerRectClipper;
    //! area rect used for hit-testing against this window
    mutable Rectf d_hitTestRect;
    //! The clipping region which was set for this window.
    Rectf d_clippingRegion;
    //! Margin, only used when the Window is inside LayoutContainer class
    //!!!FIXME: move to LC? Too much memory wasted.
    UBox d_margin;
    /*!
        Contains the draw mode mask, for this window, specifying
        a the bit flags that determine if the Window will be drawn or not
        in the draw calls, depending on the bitmask passed to the calls.
    */
    std::uint32_t d_drawModeMask;
    //! User ID assigned to this Window
    unsigned int d_ID;
    //! Cursor source we're tracking for auto-repeat purposes.
    CursorInputSource d_repeatPointerSource;
    //! The mode to use for calling Window::update
    WindowUpdateMode d_updateMode;
    //! The translation which was set for this window.
    glm::vec3 d_translation;
    //! Alpha transparency setting for the Window
    float d_alpha;
    //! seconds before first repeat event is fired
    float d_repeatDelay;
    //! seconds between further repeats after delay has expired.
    float d_repeatRate;
    //! implements repeating - tracks time elapsed.
    float d_repeatElapsed;

    //! Holds a collection of named user string values.
    std::unordered_map<String, String> d_userStrings;
    //! collection of properties not to be written to XML for this window.
    std::unordered_set<String> d_bannedXMLProperties;
    //! List of geometry buffers that cache the geometry drawn by this Window.
    std::vector<GeometryBuffer*> d_geometryBuffers;
    //! Child window objects arranged in rendering order.
    std::vector<Window*> d_drawList;

    //! RenderedString representation of text string as ouput from a parser.
    mutable RenderedString d_renderedString;
    //! Shared instance of a parser to be used in most instances.
    static BasicRenderedStringParser d_basicStringParser;
    //! Shared instance of a parser to be used when rendering text verbatim.
    static DefaultRenderedStringParser d_defaultStringParser;

    //! type of Window (also the name of the WindowFactory that created us)
    const String d_type;
    //! Type name of the window as defined in a Falagard mapping.
    String d_falagardType;
    //! Name of the Look assigned to this window (if any).
    String d_lookName;

    //! Holds the text / label / caption for this Window.
    String d_textLogical;
    //! Text string used as tip for this window.
    String d_tooltipText;

    //! true when this window is an auto-window
    bool d_autoWindow : 1;
    //! true when this window is currently being initialised (creating children etc)
    bool d_initialising : 1;
    //! true when this window is being destroyed.
    bool d_destructionStarted : 1;
    //! true when Window is enabled
    bool d_enabled : 1;
    //! is window visible (i.e. it will be rendered, but may still be obscured)
    bool d_visible : 1;
    //! true when Window is the active Window (receiving inputs).
    bool d_active : 1;
    //! true when Window will be auto-destroyed by parent.
    bool d_destroyedByParent : 1;
    //! true when Window will be clipped by parent Window area Rect.
    bool d_clippedByParent : 1;
    //! true if window geometry cache needs to be regenerated.
    bool d_needsRedraw : 1;
    //! holds setting for automatic creation of of surface (RenderingWindow)
    bool d_autoRenderingWindow : 1;
    //! holds setting for stencil buffer usage in texture caching
    bool d_autoRenderingSurfaceStencilEnabled : 1;
    //! true if the Window inherits alpha from the parent Window
    bool d_inheritsAlpha : 1;
    //! Restore capture to the previous capture window when releasing capture.
    bool d_restoreOldCapture : 1;
    //! Whether to distribute captured inputs to child windows.
    bool d_distCapturedInputs : 1;

    //! true if d_renderedString is valid, false if needs re-parse.
    mutable bool d_renderedStringValid : 1;
    //! true if use of parser other than d_defaultStringParser is enabled
    bool d_textParsingEnabled : 1;

    //! true if Window will be drawn on top of all other Windows
    bool d_alwaysOnTop : 1;
    //! whether window should rise in the z order when left cursor source is activated.
    bool d_riseOnPointerActivation : 1;
    //! true if the Window responds to z-order change requests.
    bool d_zOrderingEnabled : 1;

    //! whether (most) cursor events pass through this window
    bool d_cursorPassThroughEnabled : 1;
    //! whether pressed cursor will auto-repeat the down event.
    bool d_autoRepeat : 1;
    //! implements repeating - is true after delay has elapsed,
    bool d_repeating : 1;

    //! true if window will receive drag and drop related notifications
    bool d_dragDropTarget : 1;

    //! true if this Window created the custom Tooltip.
    bool d_weOwnTip : 1;
    //! whether tooltip text may be inherited from parent.
    bool d_inheritsTipText : 1;
    bool d_tooltipEnabled : 1;

    //! true if this window is allowed to write XML, false if not
    bool d_allowWriteXML : 1;

    mutable bool d_outerRectClipperValid : 1;
    mutable bool d_innerRectClipperValid : 1;
    mutable bool d_hitTestRectValid : 1;

    //! specifies whether cursor inputs should be propagated to parent(s)
    bool d_propagatePointerInputs : 1;

    //! true when cursor is contained within this Window's area.
    bool d_containsPointer : 1;

    //! true when this window is focused.
    bool d_isFocused : 1;

#ifdef CEGUI_BIDI_SUPPORT
    //! whether bidi visual mapping has been updated since last text change.
    mutable bool d_bidiDataValid : 1;
#endif

#ifdef CEGUI_USE_RAQM
    /*! Stores whether raqm text is up-to-date or if the logical text has changed since
     the last update
    */
    mutable bool d_raqmTextNeedsUpdate : 1;
#endif

private:

    void updateTransformAndClipping();
    void updatePivot();
};

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIWindow_h_

