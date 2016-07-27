/*
 * InGameMenu.cpp
 *
 *  Created on: May 24, 2015
 *      Author: diego
 */

#include "DeathScreen.h"

#define CHECK_MOUSE_WIDTH 0

namespace jumpinjack
{

  static void reset_y(int * y)
  {
    *y = -400;
  }

  DeathScreen::DeathScreen (SDL_Renderer * renderer) :
    AbstractMenu(renderer, "data/img/menu-bg.png")
  {
    reset_y(&menu_y);
  }

  menu_action DeathScreen::poll ()
  {
    menu_action return_val = MENU_NONE;

    if (state == MENU_STATE_FIXED)
      {
//        int x, y;
        SDL_Event event;

        while (SDL_PollEvent (&event))
          {
            switch (event.type)
              {
              case SDL_KEYDOWN:
                state = MENU_STATE_UNLOAD;
                break;
              }
          }
//
//        for (t_option & op : options)
//          {
//            op.texture->loadFromRenderedText (op.option_text,
//                                              color[op.id == selected_option]);
//          }
      }
    else if (state == MENU_STATE_DONE)
      {
        state = MENU_STATE_LOAD;
        reset_y(&menu_y);
        return_val = MENU_CONTINUE;
      }
    return return_val;
  }

  void DeathScreen::renderFixed (t_point point)
  {
      if (state == MENU_STATE_LOAD)
      {
        menu_y += 40;
        if (menu_y>= 0)
          state = MENU_STATE_FIXED;
      }
      else if (state == MENU_STATE_UNLOAD)
      {
        menu_y -= 40;
        if (menu_y <= -400)
          state = MENU_STATE_DONE;
      }

      t_point menu_point = { (GlobalDefs::window_size.x - 640) / 2, menu_y };
      _render(menu_point);
  }

  DeathScreen::~DeathScreen ()
  {
  }

} /* namespace jumpinjack */
