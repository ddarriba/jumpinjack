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
    *y = -800;
  }

  DeathScreen::DeathScreen (SDL_Renderer * renderer) :
    AbstractMenu(renderer, GlobalDefs::getResource (
                           RESOURCE_IMAGE, "death-bg.png").c_str())
  {
    reset_y(&menu_y);
  }

  menu_action DeathScreen::poll ()
  {
    menu_action return_val = MENU_NONE;
    SDL_Event event;

    t_point gwindow_size = GlobalDefs::window_size;
    window_pos =
        { (int)round(0.1 * gwindow_size.x),
          (int)round(0.1 * gwindow_size.y) };
    window_size =
        { (int)round(gwindow_size.x - 0.2*gwindow_size.x),
          (int)round(gwindow_size.y - 0.2*gwindow_size.y) };

    if (state == MENU_STATE_FIXED)
      {
        while (SDL_PollEvent (&event))
          {
            switch (event.type)
              {
              case SDL_KEYDOWN:
                state = MENU_STATE_UNLOAD;
                break;
              }
          }
      }
    else if (state == MENU_STATE_DONE)
      {
        state = MENU_STATE_LOAD;
        reset_y(&menu_y);
        return_val = MENU_CONTINUE;
      }
    else
      while (SDL_PollEvent (&event)); /* purge queue */

    return return_val;
  }

  void DeathScreen::renderFixed (t_point point)
  {
      if (state == MENU_STATE_LOAD)
      {
        menu_y += 40;
        if (menu_y >= window_pos.y)
          state = MENU_STATE_FIXED;
      }
      else if (state == MENU_STATE_UNLOAD)
      {
        menu_y -= 100;
        if (menu_y <= -400)
          state = MENU_STATE_DONE;
      }

      bg->render ({window_pos.x, menu_y}, window_size);
  }

  DeathScreen::~DeathScreen ()
  {
  }

} /* namespace jumpinjack */
