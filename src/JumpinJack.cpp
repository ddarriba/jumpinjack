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

int main (
    void)
{
  SdlManager manager;
  manager.mapEvent(ETYPE_KEYBOARD, SDL_SCANCODE_ESCAPE, EVENT_EXIT, TRIGGER_UP);

  manager.mapEvent(ETYPE_KEYBOARD, SDL_SCANCODE_LEFT, EVENT_LEFT, TRIGGER_HOLD);
  manager.mapEvent(ETYPE_KEYBOARD, SDL_SCANCODE_RIGHT, EVENT_RIGHT, TRIGGER_HOLD);
  manager.mapEvent(ETYPE_KEYBOARD, SDL_SCANCODE_UP, EVENT_UP, TRIGGER_HOLD);
  manager.mapEvent(ETYPE_KEYBOARD, SDL_SCANCODE_UP, EVENT_UP_RELEASE, TRIGGER_UP);
  manager.mapEvent(ETYPE_KEYBOARD, SDL_SCANCODE_DOWN, EVENT_DOWN, TRIGGER_HOLD);

  manager.mapEvent(ETYPE_KEYBOARD, SDL_SCANCODE_A, EVENT_LEFT, TRIGGER_HOLD);
  manager.mapEvent(ETYPE_KEYBOARD, SDL_SCANCODE_D, EVENT_RIGHT, TRIGGER_HOLD);
  manager.mapEvent(ETYPE_KEYBOARD, SDL_SCANCODE_W, EVENT_UP, TRIGGER_HOLD);
  manager.mapEvent(ETYPE_KEYBOARD, SDL_SCANCODE_W, EVENT_UP_RELEASE, TRIGGER_UP);
  manager.mapEvent(ETYPE_KEYBOARD, SDL_SCANCODE_S, EVENT_DOWN, TRIGGER_HOLD);

  manager.mapEvent(ETYPE_KEYBOARD, SDL_SCANCODE_LSHIFT, EVENT_SPRINT, TRIGGER_HOLD);

  manager.addPlayer(GlobalDefs::getResource(RESOURCE_IMAGE,"player1.png"), 12, 0, 1);

  manager.startLevel(1);

  bool in_game = true;
  while (in_game)
    {
      manager.startLoop ();

      int event;
      int next_action = ACTION_NONE;
      manager.pollEvents ();

      t_point point;
      while ((event = manager.pollSingleEvent (&point)) != EVENT_NONE)
        {
          switch (event)
            {
            case EVENT_EXIT:
              in_game = false;
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
            case EVENT_SPRINT:
              next_action |= ACTION_SPRINT;
              break;
            default:
              break;
            }
        }
      manager.applyAction((t_action) next_action);

      /* update */
      manager.update ();

      /* render */
      manager.render ();

      manager.endLoop ();
    }
  return EXIT_SUCCESS;
}
