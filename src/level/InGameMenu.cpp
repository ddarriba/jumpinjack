/*
 * InGameMenu.cpp
 *
 *  Created on: May 24, 2015
 *      Author: diego
 */

#include "InGameMenu.h"

namespace jumpinjack
{

  static void reset_y(int * y)
  {
    *y = -400;
  }

  InGameMenu::InGameMenu (SDL_Renderer * renderer) :
      Drawable(renderer, 100, false)
  {
    options.reserve (4);
    options.push_back (
      { "continue", MENU_CONTINUE, 0,
        { 0, 0, 0, 0 } });
    options.push_back (
      { "back to main", MENU_MAIN, 0,
        { 0, 0, 0, 0 } });
    options.push_back (
      { "set controls", MENU_SET_CONTROLS, 0,
        { 0, 0, 0, 0 } });
    options.push_back (
      { "exit game", MENU_EXIT, 0,
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
        op.selected = false;
        yMenuOffset += op.quad.h;
      }

    bg = new BackgroundDrawable(renderer, "data/img/menu-bg.png", 0);
    assert(bg);

    state = MENU_STATE_LOAD;
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
                    if (x >= op.quad.x && x <= op.quad.x + op.quad.w
                        && y >= op.quad.y && y <= op.quad.y + op.quad.h)
                      {
                        if (!op.selected)
                          {
                            op.selected = true;
                          }
                      }
                    else
                      {
                        if (op.selected)
                          {
                            op.selected = false;
                          }
                      }
                  }
                break;
              case SDL_MOUSEBUTTONDOWN:
                x = event.button.x;
                y = event.button.y;
                for (t_option & op : options)
                  {
                    if (x >= op.quad.x && x <= op.quad.x + op.quad.w
                        && y >= op.quad.y && y <= op.quad.y + op.quad.h)
                      {
                        return_val = op.action;
                      }
                  }
                break;
              case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                  {
                    return_val = MENU_CONTINUE;
                  }
                break;
              }
          }

        for (t_option & op : options)
          {
            op.texture->loadFromRenderedText (op.option_text,
                                              color[op.selected ? 1 : 0]);
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
    bg->render({
      (GlobalDefs::window_size.x - 640) / 2
      , menu_y}, {640,400});
    for (t_option & op : options)
          {
        op.texture->render({(GlobalDefs::window_size.x - op.quad.w) / 2, menu_y + op.quad.y},{op.quad.w,
            op.quad.h});
          }
  }

  InGameMenu::~InGameMenu ()
  {
    for (t_option & op : options)
      {
        if (op.texture)
          delete op.texture;
      }
    delete bg;
  }

} /* namespace jumpinjack */
