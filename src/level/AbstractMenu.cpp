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

  void AbstractMenu::_render( void ) const
  {
    bg->render (window_pos, window_size);

    for (t_option const& op : options)
    {
      op.texture->render (
        { window_pos.x + (window_size.x - op.quad.w) / 2,
           window_pos.y + op.quad.y },
        { op.quad.w, op.quad.h });
    }
  }
} /* namespace jumpinjack */
