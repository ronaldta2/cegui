/***********************************************************************
    created:    25/4/2004
    author:     Paul D Turner

    purpose:    Implementation of common Titlebar parts.
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/widgets/Titlebar.h"
#include "CEGUI/widgets/FrameWindow.h"
#include "CEGUI/CoordConverter.h"
#include "CEGUI/GUIContext.h"

// Start of CEGUI namespace section
namespace CEGUI
{
const String Titlebar::EventNamespace("Titlebar");
const String Titlebar::WidgetTypeName("CEGUI/Titlebar");

/*************************************************************************
    Constructor
*************************************************************************/
Titlebar::Titlebar(const String& type, const String& name) :
    Window(type, name)
{
    addTitlebarProperties();
    setAlwaysOnTop(true);
    setCursorInputPropagationEnabled(true);

    // basic initialisation
    d_dragging = false;
    d_dragEnabled = true;
}

/*************************************************************************
    Destructor
*************************************************************************/
Titlebar::~Titlebar(void)
{
}


/*************************************************************************
    Return whether this title bar will respond to dragging.
*************************************************************************/
bool Titlebar::isDraggingEnabled(void) const
{
    return d_dragEnabled;
}


/*************************************************************************
    Set whether this title bar widget will respond to dragging.
*************************************************************************/
void Titlebar::setDraggingEnabled(bool setting)
{
    if (d_dragEnabled != setting)
    {
        d_dragEnabled = setting;

        // stop dragging now if the setting has been disabled.
        if ((!d_dragEnabled) && d_dragging)
        {
            releaseInput();
        }

        // call event handler.
        WindowEventArgs args(this);
        onDraggingModeChanged(args);
    }

}

bool Titlebar::isDragged() const
{
    return d_dragging;
}

const glm::vec2& Titlebar::getDragPoint() const
{
    return d_dragPoint;
}

/*************************************************************************
    Handler for cursor movement events
*************************************************************************/
void Titlebar::onCursorMove(CursorInputEventArgs& e)
{
    // Base class processing.
    Window::onCursorMove(e);

    if (d_dragging && (d_parent != nullptr))
    {
        // move the window.  *** Again: Titlebar objects should only be attached to FrameWindow derived classes. ***
        if (auto frameWnd = dynamic_cast<FrameWindow*>(d_parent))
        {
            const glm::vec2 delta(CoordConverter::screenToWindow(*this, e.position) - d_dragPoint);
            frameWnd->setPosition(frameWnd->getPosition() + UVector2(cegui_absdim(delta.x), cegui_absdim(delta.y)));
        }

        ++e.handled;
    }
}


/*************************************************************************
    Handler for cursor press events
*************************************************************************/
void Titlebar::onCursorPressHold(CursorInputEventArgs& e)
{
    Window::onCursorPressHold(e);

    if (e.source == CursorInputSource::Left)
    {
        // Sizing border events are propagated to the owning FrameWindow
        auto frameWnd = dynamic_cast<FrameWindow*>(d_parent);
        if (frameWnd && frameWnd->isSizingEnabled())
        {
            const glm::vec2 localCursorPos(CoordConverter::screenToWindow(*frameWnd, e.position));
            if (frameWnd->getSizingBorderAtPoint(localCursorPos) != FrameWindow::SizingLocation::Invalid)
                return;
        }

        if (d_parent && d_dragEnabled)
        {
            // we want all cursor inputs from now on
            if (captureInput())
            {
                // initialise the dragging state
                d_dragging = true;
                d_dragPoint = CoordConverter::screenToWindow(*this, e.position);

                // store old constraint area
                d_oldCursorArea = getGUIContext().getCursor().getConstraintArea();

                // setup new constraint area to be the intersection of the old area and our grand-parent's clipped inner-area
                Rectf constrainArea;
                if (auto grandParent = static_cast<Window*>(d_parent->getParentElement()))
                {
                    constrainArea = grandParent->getInnerRectClipper().getIntersection(d_oldCursorArea);
                }
                else
                {
                    const Rectf screen(glm::vec2(0, 0), getRootContainerSize());
                    constrainArea = screen.getIntersection(d_oldCursorArea);
                }

                getGUIContext().getCursor().setConstraintArea(&constrainArea);
            }
        }

        ++e.handled;
    }
}


/*************************************************************************
    Handler for cursor activation events
*************************************************************************/
void Titlebar::onCursorActivate(CursorInputEventArgs& e)
{
    // Base class processing
    Window::onCursorActivate(e);

    if (e.source == CursorInputSource::Left)
    {
        releaseInput();
        ++e.handled;
    }
}

void Titlebar::onSemanticInputEvent(SemanticEventArgs& e)
{
    // Base class processing
    Window::onSemanticInputEvent(e);

    if (isDisabled())
        return;

    if (e.d_semanticValue == SemanticValue::SelectWord && e.d_payload.source == CursorInputSource::Left)
    {
        // Our parent must be a FrameWindow or subclass for rolling up to work
        if (auto frameWnd = dynamic_cast<FrameWindow*>(d_parent))
            frameWnd->toggleRollup();

        ++e.handled;
    }
}

/*************************************************************************
    Handler for if the window loses capture of the cursor.
*************************************************************************/
void Titlebar::onCaptureLost(WindowEventArgs& e)
{
    // Base class processing
    Window::onCaptureLost(e);

    // when we lose out hold on the cursor inputs, we are no longer dragging.
    d_dragging = false;

    // restore old constraint area
    getGUIContext().
        getCursor().setConstraintArea(&d_oldCursorArea);
}


/*************************************************************************
    Handler for when the font for this Window is changed
*************************************************************************/
void Titlebar::onFontChanged(WindowEventArgs& e)
{
    Window::onFontChanged(e);

    if (d_parent && !getParent()->isInitializing())
    {
        getParent()->performChildLayout(false, false);
    }
}


/*************************************************************************
    Add title bar specific properties
*************************************************************************/
void Titlebar::addTitlebarProperties(void)
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(Titlebar, bool,
        "DraggingEnabled", "Property to get/set the state of the dragging enabled setting for the Titlebar.  Value is either \"true\" or \"false\".",
        &Titlebar::setDraggingEnabled, &Titlebar::isDraggingEnabled, true
    );
}

} // End of  CEGUI namespace section
