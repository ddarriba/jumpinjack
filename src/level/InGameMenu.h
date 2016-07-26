/*
 * InGameMenu.h
 *
 *  Created on: May 24, 2015
 *      Author: diego
 */

#ifndef LEVEL_INGAMEMENU_H_
#define LEVEL_INGAMEMENU_H_

#include "../sdl/Drawable.h"
#include "../sdl/BackgroundDrawable.h"

enum menu_action {
  MENU_CONTINUE,
  MENU_MAIN,
  MENU_SET_CONTROLS,
  MENU_EXIT,
  MENU_NONE
};

const t_point ingame_menu_size =
  { 400, 300 };
const SDL_Color color[2] =
  {
    { 0, 0, 0 },
    { 255, 0, 0 } };

namespace jumpinjack
{

  typedef struct
  {
    int id;
    std::string option_text;
    menu_action action;
    Drawable * texture;
    SDL_Rect quad;
  } t_option;

  class InGameMenu : public Drawable
  {
    public:
      InGameMenu (SDL_Renderer * renderer);
      virtual ~InGameMenu ();

      menu_action poll ();

      virtual void renderFixed (t_point point);
    private:
      enum menu_state {MENU_STATE_LOAD, MENU_STATE_FIXED, MENU_STATE_UNLOAD, MENU_STATE_DONE };
      std::vector<t_option> options;
      BackgroundDrawable * bg;
      menu_state state;
      int selected_option;

      int menu_y;
  };

} /* namespace jumpinjack */

#endif /* LEVEL_INGAMEMENU_H_ */
