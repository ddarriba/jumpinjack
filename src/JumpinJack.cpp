//============================================================================
// Name        : JumpinJack.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>

#include "GlobalDefs.h"
#include "sdl/SdlManager.h"

using namespace jumpinjack;
using namespace std;

int main (void)
{
  SdlManager manager;
  manager.mapEvent (ETYPE_KEYBOARD, SDL_SCANCODE_ESCAPE, EVENT_MENU_LOAD,
                    TRIGGER_DOWN);

  manager.mapEvent (ETYPE_KEYBOARD, SDL_SCANCODE_LEFT, EVENT_LEFT,
                    TRIGGER_HOLD);
  manager.mapEvent (ETYPE_KEYBOARD, SDL_SCANCODE_RIGHT, EVENT_RIGHT,
                    TRIGGER_HOLD);
  manager.mapEvent (ETYPE_KEYBOARD, SDL_SCANCODE_UP, EVENT_UP, TRIGGER_HOLD);
  manager.mapEvent (ETYPE_KEYBOARD, SDL_SCANCODE_UP, EVENT_UP_RELEASE,
                    TRIGGER_UP);
  manager.mapEvent (ETYPE_KEYBOARD, SDL_SCANCODE_DOWN, EVENT_DOWN,
                    TRIGGER_HOLD);

  manager.mapEvent (ETYPE_KEYBOARD, SDL_SCANCODE_A, EVENT_LEFT, TRIGGER_HOLD);
  manager.mapEvent (ETYPE_KEYBOARD, SDL_SCANCODE_D, EVENT_RIGHT, TRIGGER_HOLD);
  manager.mapEvent (ETYPE_KEYBOARD, SDL_SCANCODE_W, EVENT_UP, TRIGGER_HOLD);
  manager.mapEvent (ETYPE_KEYBOARD, SDL_SCANCODE_W, EVENT_UP_RELEASE,
                    TRIGGER_UP);
  manager.mapEvent (ETYPE_KEYBOARD, SDL_SCANCODE_S, EVENT_DOWN, TRIGGER_HOLD);

  manager.mapEvent (ETYPE_KEYBOARD, SDL_SCANCODE_SPACE, EVENT_SHOOT,
                    TRIGGER_DOWN);
  manager.mapEvent (ETYPE_KEYBOARD, SDL_SCANCODE_LSHIFT, EVENT_SPRINT,
                    TRIGGER_HOLD);

  manager.addPlayer (GlobalDefs::getResource (RESOURCE_IMAGE, "player1.png"),
                     4, 0, 3);



  bool in_game = true;

  if (TTF_Init () == -1)
    {
      printf ("SDL_ttf could not initialize! SDL_ttf Error: %s\n",
      TTF_GetError ());
    }

  TTF_Font * font = TTF_OpenFont ("data/fonts/zorque.ttf", 30);
  if (font == NULL)
    {
      printf ("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError ());
    }

  int i = manager.showmenu (font);
  if (i == 1)
    in_game = false;

  manager.startLevel (1);

  bool game_paused = false;
  while (in_game)
    {
      manager.startLoop ();

      int event;
      int next_action = ACTION_NONE;

      if (!game_paused)
        {
          manager.pollEvents ();
        }

      t_point point;
      while ((event = manager.pollSingleEvent (&point)) != EVENT_NONE)
        {
          switch (event)
            {
            case EVENT_EXIT:
              in_game = false;
              break;
            case EVENT_MENU_LOAD:
              game_paused = true;
              break;
            case EVENT_MENU_UNLOAD:
              game_paused = false;
              break;
            case EVENT_LEFT:
              next_action |= ACTION_LEFT;
              break;
            case EVENT_RIGHT:
              next_action |= ACTION_RIGHT;
              break;
            case EVENT_UP:
              next_action |= ACTION_UP;
              break;
            case EVENT_UP_RELEASE:
              next_action |= ACTION_UP_REL;
              break;
            case EVENT_DOWN:
              next_action |= ACTION_DOWN;
              break;
            case EVENT_SHOOT:
              next_action |= ACTION_SHOOT;
              break;
            case EVENT_SPRINT:
              next_action |= ACTION_SPRINT;
              break;
            default:
              break;
            }
        }
      manager.applyAction ((t_action) next_action);

      /* update */
      manager.update (game_paused);

      /* render */
      manager.render ();

      manager.endLoop ();
    }
  return EXIT_SUCCESS;
}
