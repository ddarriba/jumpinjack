/*
 * AbstractMenu.h
 *
 *  Created on: Jul 27, 2016
 *      Author: diego
 */

#ifndef ABSTRACTMENU_H_
#define ABSTRACTMENU_H_

#include "../sdl/Drawable.h"
#include "../sdl/BackgroundDrawable.h"

const SDL_Color color[2] =
  {
    { 0, 0, 0 },
    { 255, 0, 0 } };

namespace jumpinjack
{

  enum menu_action {
    MENU_CONTINUE,
    MENU_MAIN,
    MENU_SET_CONTROLS,
    MENU_EXIT,
    MENU_NONE
  };

  typedef struct
    {
      int id;
      std::string option_text;
      menu_action action;
      Drawable * texture;
      SDL_Rect quad;
    } t_option;

  class AbstractMenu : public Drawable
  {
  public:
    AbstractMenu(SDL_Renderer * renderer, const char * bg_file);
    virtual ~AbstractMenu();

    virtual menu_action poll () = 0;
    virtual void renderFixed (t_point point) = 0;
  protected:
    enum menu_state
    {
      MENU_STATE_LOAD, MENU_STATE_FIXED, MENU_STATE_UNLOAD, MENU_STATE_DONE
    };

    t_point window_pos;
    t_point window_size;

    void _render( void ) const;
    std::vector<t_option> options;
    BackgroundDrawable * bg;
    menu_state state;
    int selected_option;
  };

} /* namespace jumpinjack */

#endif /* ABSTRACTMENU_H_ */
