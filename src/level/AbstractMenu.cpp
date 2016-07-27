/*
 * AbstractMenu.cpp
 *
 *  Created on: Jul 27, 2016
 *      Author: diego
 */

#include "AbstractMenu.h"
#include "../GlobalDefs.h"

namespace jumpinjack
{

  AbstractMenu::AbstractMenu(SDL_Renderer * renderer, const char *  bg_file) :
          Drawable(renderer, 100, false)
  {
    state = MENU_STATE_LOAD;
    bg = new BackgroundDrawable(renderer, bg_file, 0);
    assert(bg);
  }

  AbstractMenu::~AbstractMenu()
  {
    for (t_option & op : options)
    {
      if (op.texture)
        delete op.texture;
    }
    delete bg;
  }

  void AbstractMenu::_render(t_point & point)
  {
    bg->render (point, { 640, 400 });

    for (t_option & op : options)
    {
      op.texture->render (
        { (point.x + 640 - op.quad.w) / 2, point.y + op.quad.y },
                            { op.quad.w, op.quad.h });
    }
  }
} /* namespace jumpinjack */
