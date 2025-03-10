/***********************************************************************
    created:    Sun Jul 3 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
#ifndef _FalSlider_h_
#define _FalSlider_h_

#include "CEGUI/WindowRendererSets/Core/Module.h"
#include "CEGUI/widgets/Slider.h"

namespace CEGUI
{
/*!
\brief
    Slider class for the FalagardBase module.

    This class requires LookNFeel to be assigned.
    The LookNFeel should provide the following:

    States:
        - Enabled
        - EnabledFocused
        - Disabled

    Named Areas:
        - ThumbTrackArea

    Child Widgets:
        Thumb based widget with name suffix "__auto_thumb__"

    Property initialiser definitions:
        - VerticalSlider - boolean property.
            Indicates whether this slider will operate in the vertical or
            horizontal direction. Default is for horizontal. Optional.
*/
class COREWRSET_API FalagardSlider : public SliderWindowRenderer
{
public:

    static const String TypeName;     //! type name for this widget.

    FalagardSlider(const String& type);

    bool isVertical() const { return d_vertical; }
    void setVertical(bool setting) { d_vertical = setting; }

    bool isReversedDirection() const { return d_reversed; }
    void setReversedDirection(bool setting) { d_reversed = setting; }

    void createRenderGeometry() override;
    bool performChildWindowLayout() override;

protected:

    // overridden from Slider base class.
    void updateThumb() override;
    value_type getValueFromThumb() const override;
    float getAdjustDirectionFromPoint(const glm::vec2& pt) const override;

    value_type getValueAtPoint(float pt) const;

    bool d_vertical = false; //!< True if slider operates in vertical direction.
    bool d_reversed = false; //!< True if slider operates in reversed direction to 'normal'.
};

} // End of  CEGUI namespace section

#endif  // end of guard _FalSlider_h_
