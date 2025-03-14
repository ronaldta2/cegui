/***********************************************************************
    created:    Fri Apr 22 2011
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#include "InventoryItem.h"
#include "InventoryReceiver.h"

#include <CEGUI/PropertyHelper.h>
#include <CEGUI/Image.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//------------------------------------------------------------------------------//
const String InventoryItem::WidgetTypeName("InventoryItem");
const String InventoryItem::EventNamespace("InventoryItem");

//------------------------------------------------------------------------------//
InventoryItem::InventoryItem(const String& type, const String& name) :
    DragContainer(type, name),
    d_validDropTarget(false),
    d_receiverLocationX(-1),
    d_receiverLocationY(-1)
{
}

//------------------------------------------------------------------------------//
void InventoryItem::setContentSize(int width, int height)
{
    InventoryBase::setContentSize(width, height);
    d_content.clear(true);
}

//------------------------------------------------------------------------------//
bool InventoryItem::isSolidAtLocation(int x, int y) const
{
    return d_content.elementAtLocation(x, y);
}

//------------------------------------------------------------------------------//
void InventoryItem::setItemLayout(const bool* layout)
{
    for (int y = 0; y < d_content.height(); ++y)
        for (int x = 0; x < d_content.width(); ++x)
            d_content.setElementAtLocation(x, y, *layout++);
}

//------------------------------------------------------------------------------//
int InventoryItem::locationOnReceiverX() const
{
    return d_receiverLocationX;
}

//------------------------------------------------------------------------------//
int InventoryItem::locationOnReceiverY() const
{
    return d_receiverLocationY;
}

//------------------------------------------------------------------------------//
void InventoryItem::setLocationOnReceiver(int x, int y)
{
    d_receiverLocationX = x;
    d_receiverLocationY = y;
}

//------------------------------------------------------------------------------//
bool InventoryItem::isHit(const glm::vec2& position, const bool allow_disabled) const
{
    if (!DragContainer::isHit(position, allow_disabled))
        return false;

    int gx = gridXLocationFromPixelPosition(position.x);
    int gy = gridYLocationFromPixelPosition(position.y);

    if (gx < 0 || gx >= d_content.width() || gy < 0 || gy >= d_content.height())
        return false;

    return d_content.elementAtLocation(gx, gy);
}

//------------------------------------------------------------------------------//
bool InventoryItem::currentDropTargetIsValid() const
{
    return d_validDropTarget;
}

//------------------------------------------------------------------------------//
void InventoryItem::populateGeometryBuffer()
{
    if (!isUserStringDefined("BlockImage"))
        return;

    const Image* img = PropertyHelper<Image*>::fromString(getUserString("BlockImage"));

    if (!img)
        return;

    // If is over a valid receiver, render in a cell size of the receiver
    // TODO: do the same for items rendered by InventoryItemRenderer
    const Sizef square_size = (d_dragging && currentDropTargetIsValid()) ?
        static_cast<InventoryReceiver*>(d_dropTarget)->squarePixelSize() :
        squarePixelSize();

    const argb_t colour = (d_dragging && !currentDropTargetIsValid()) ? 0xFFFF0000 : 0xFF00FF00;

    ImageRenderSettings imgRenderSettings(
        Rectf(), nullptr, false, ColourRect(colour));

    for (int y = 0; y < d_content.height(); ++y)
    {
        for (int x = 0; x < d_content.width(); ++x)
        {
            if (d_content.elementAtLocation(x, y))
            {
                imgRenderSettings.d_destArea = Rectf(
                    glm::vec2(x * square_size.d_width + 1, y * square_size.d_height + 1),
                    Sizef(square_size.d_width - 2, square_size.d_height - 2));

                auto geomBuffers = img->createRenderGeometry(imgRenderSettings);

                appendGeometryBuffers(geomBuffers);
            }
        }
    }
}

//------------------------------------------------------------------------------//
Rectf InventoryItem::gridBasePixelRect() const
{
    return getUnclippedOuterRect().get();
}

//------------------------------------------------------------------------------//
void InventoryItem::onMoved(ElementEventArgs& e)
{
    invalidate();

    DragContainer::onMoved(e);

    if (InventoryReceiver* receiver = dynamic_cast<InventoryReceiver*>(d_dropTarget))
    {
        const Sizef square_size(receiver->squarePixelSize());
        Rectf area(getUnclippedOuterRect().get());
        area.offset(0.5f * glm::vec2(square_size.d_width, square_size.d_height));
        const int x = receiver->gridXLocationFromPixelPosition(area.left());
        const int y = receiver->gridYLocationFromPixelPosition(area.top());

        d_validDropTarget = receiver->itemWillFitAtLocation(*this, x, y);
        return;
    }

    d_validDropTarget = false;
}

//------------------------------------------------------------------------------//
void InventoryItem::onDragDropTargetChanged(DragDropEventArgs& e)
{
    DragContainer::onDragDropTargetChanged(e);
    d_validDropTarget = (dynamic_cast<InventoryReceiver*>(d_dropTarget) != nullptr);
    invalidate();
}

//------------------------------------------------------------------------------//

} // End of  CEGUI namespace section

