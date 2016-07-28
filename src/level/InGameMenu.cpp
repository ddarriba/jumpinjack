/*
 * InGameMenu.cpp
 *
 *  Created on: May 24, 2015
 *      Author: diego
 */

#include "InGameMenu.h"

#define CHECK_MOUSE_WIDTH 0

namespace jumpinjack
{

  static void reset_y(int * y)
  {
    *y = -400;
  }

  InGameMenu::InGameMenu (SDL_Renderer * renderer) :
      AbstractMenu(renderer, GlobalDefs::getResource (
                             RESOURCE_IMAGE, "menu-bg.png").c_str())
  {
    selected_option = 0;

    window_pos = { (GlobalDefs::window_size.x - 640) / 2, menu_y };
    window_size = { 600, 400 };

    options.reserve (4);
    options.push_back (
      { 0, "continue", MENU_CONTINUE, 0,
        { 0, 0, 0, 0 } });
    options.push_back (
      { 1, "back to main", MENU_MAIN, 0,
        { 0, 0, 0, 0 } });
    options.push_back (
      { 2, "set controls", MENU_SET_CONTROLS, 0,
        { 0, 0, 0, 0 } });
    options.push_back (
      { 3, "exit game", MENU_EXIT, 0,
        { 0, 0, 0, 0 } });

    int yMenuOffset = 50;
    for (t_option & op : options)
      {
        op.texture = new Drawable (renderer, 0, false);
        op.texture->loadFromRenderedText (op.option_text, color[0]);
        op.quad.x = GlobalDefs::window_size.x / 2 - op.texture->getWidth () / 2;
        op.quad.y = yMenuOffset;
        op.quad.w = op.texture->getWidth ();
        op.quad.h = op.texture->getHeight ();
        yMenuOffset += op.quad.h;
      }

    reset_y(&menu_y);
  }

  menu_action InGameMenu::poll ()
  {
    menu_action return_val = MENU_NONE;

    if (state == MENU_STATE_FIXED)
      {
        int x, y;
        SDL_Event event;

        while (SDL_PollEvent (&event))
          {
            switch (event.type)
              {
              case SDL_QUIT:
                return_val = MENU_EXIT;
                break;
              case SDL_MOUSEMOTION:
                x = event.motion.x;
                y = event.motion.y;
                for (t_option & op : options)
                  {
                    if ((!CHECK_MOUSE_WIDTH ||
                        (x >= op.quad.x && x <= op.quad.x + op.quad.w)) &&  /* check width */
                        y >= op.quad.y && y <= op.quad.y + op.quad.h)
                      {
                        selected_option = op.id;
                      }
                  }
                break;
              case SDL_MOUSEBUTTONDOWN:
                x = event.button.x;
                y = event.button.y;
                for (t_option & op : options)
                  {
                    if ((!CHECK_MOUSE_WIDTH ||
                        (x >= op.quad.x && x <= op.quad.x + op.quad.w)) &&
                        y >= op.quad.y && y <= op.quad.y + op.quad.h)
                      {
                        return_val = op.action;
                      }
                  }
                break;
              case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                  case SDLK_UP:
                    selected_option += options.size() - 1;
                    selected_option %= options.size();
                    break;
                  case SDLK_DOWN:
                    selected_option ++;
                    selected_option %= options.size();
                    break;
                  case SDLK_ESCAPE:
                    return_val = MENU_CONTINUE;
                    break;
                  case SDLK_RETURN:
                    return_val = options[selected_option].action;
                    break;
                }
                break;
              }
          }

        for (t_option & op : options)
          {
            op.texture->loadFromRenderedText (op.option_text,
                                              color[op.id == selected_option]);
          }

        if (return_val == MENU_CONTINUE)
          {
            state = MENU_STATE_UNLOAD;
            return_val = MENU_NONE;
          }
      }
    else if (state == MENU_STATE_DONE)
      {
        state = MENU_STATE_LOAD;
        reset_y(&menu_y);
        return_val = MENU_CONTINUE;
      }
    return return_val;
  }

  void InGameMenu::renderFixed (t_point point)
  {
    if (state == MENU_STATE_UNLOAD)
      {
        menu_y -= 40;
        if (menu_y <= -400)
          state = MENU_STATE_DONE;
      }
    else if (state == MENU_STATE_LOAD)
      {
        menu_y += 40;
        if (menu_y>= 0)
          state = MENU_STATE_FIXED;
      }

    _render();
  }

  InGameMenu::~InGameMenu ()
  {
  }

} /* namespace jumpinjack */
