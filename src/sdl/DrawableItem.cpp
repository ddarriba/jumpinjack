/*
 * DrawableItem.cpp
 *
 *  Created on: May 17, 2015
 *      Author: diego
 */

#include "DrawableItem.h"

namespace jumpinjack
{

  DrawableItem::DrawableItem (SDL_Renderer * renderer, int zindex) :
          Drawable (renderer, zindex, true)
  {
    status = (t_status) (STATUS_ALIVE | STATUS_LISTENING);
  }

  DrawableItem::~DrawableItem ()
  {
  }

  void DrawableItem::setStatus (t_status s)
  {
    status = (t_status) (status | s);
  }

  void DrawableItem::unsetStatus (t_status s)
  {
    status = (t_status) (status & ~s);
  }

  bool DrawableItem::getStatus (t_status s)
  {
    return status & s;
  }
} /* namespace jumpinjack */
