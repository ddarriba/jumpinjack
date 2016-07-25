/*
 * SdlManager.h
 *
 *  Created on: May 16, 2015
 *      Author: diego
 */

#ifndef SDL_SDLMANAGER_H_
#define SDL_SDLMANAGER_H_

#include "../GlobalDefs.h"
#include "../characters/Player.h"
#include "../level/LevelManager.h"
#include "../level/InGameMenu.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <vector>
#include <queue>
#include <string>

struct queued_event
{
    t_event user_event;
    t_point point;
};

enum menu_option {
  MENU_OPTION_CONTINUE,
  MENU_OPTION_EXIT
};

namespace jumpinjack
{
  class SdlManager
  {
    public:
      SdlManager ();
      virtual ~SdlManager ();

      int addPlayer(std::string sprite_file, int sprite_length,
                    int sprite_start_line, int sprite_frequency);

      int startLevel(int level_id);

      void mapEvent (
          t_event_type type, int event_id, t_event user_event,
          t_trigger trigger);
      void pollEvents ();
      t_event pollSingleEvent (
          t_point * point);

      void applyAction (t_action action);

      menu_option showmenu(TTF_Font* font);

      void startLoop ();
      void endLoop ();

      void update (bool game_paused = false);

      void render (bool render_menu = false);
    private:
      bool init();

      SDL_Window * window;
      SDL_Renderer * renderer;

      SDL_Surface * window_icon;
      SDL_Surface * menu_screen;

      Uint32 start_ticks;

      std::vector<t_event_record> mapped_events;
      std::queue<queued_event> events_queue;

      std::vector<Player *> players;
      LevelManager * level;
      InGameMenu * ingame_menu;
  };

} /* namespace sdlfw */

#endif /* SDL_SDLMANAGER_H_ */
