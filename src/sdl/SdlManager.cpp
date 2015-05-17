/*
 * SdlManager.cpp
 *
 *  Created on: May 16, 2015
 *      Author: diego
 */

#include "SdlManager.h"

#include <iostream>

using namespace std;

namespace jumpinjack
{

  SdlManager::SdlManager ()
  {
    init ();
    mapped_events.reserve (MAX_EVENTS);
    players.reserve(MAX_PLAYERS);
    level = NULL;
  }

  SdlManager::~SdlManager ()
  {
    SDL_DestroyRenderer (renderer);
    SDL_DestroyWindow (window);

    for (Player * player : players)
      delete player;

    if (level)
      delete level;

    SDL_Quit ();
  }

  bool SdlManager::init ()
  {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init ( SDL_INIT_EVERYTHING) < 0)
      {
        printf ("SDL could not initialize! SDL Error: %s\n", SDL_GetError ());
        success = false;
      }
    else
      {
        //Set texture filtering to linear
        if (!SDL_SetHint ( SDL_HINT_RENDER_SCALE_QUALITY, "1"))
          {
            printf ("Warning: Linear texture filtering not enabled!");
          }

        //Create window
        window = SDL_CreateWindow ("Jumpin'Jack", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
                                   GlobalDefs::window_size.x,
                                   GlobalDefs::window_size.y,
                                   SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
        if (window == NULL)
          {
            printf ("Window could not be created! SDL Error: %s\n",
                    SDL_GetError ());
            success = false;
          }
        else
          {
            //Create vsynced renderer for window
            renderer = SDL_CreateRenderer (
                window, -1,
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (renderer == NULL)
              {
                printf ("Renderer could not be created! SDL Error: %s\n",
                        SDL_GetError ());
                success = false;
              }
            else
              {
                //Initialize renderer color
                SDL_SetRenderDrawColor (renderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init (imgFlags) & imgFlags))
                  {
                    printf (
                        "SDL_image could not initialize! SDL_image Error: %s\n",
                        IMG_GetError ());
                    success = false;
                  }
              }
          }
      }
    return success;
  }

  int SdlManager::addPlayer(string sprite_file, int sprite_length,
                      int sprite_start_line, int sprite_frequency)
  {
    if (players.size() == MAX_PLAYERS)
      return -1;
    Player * new_player = new Player(renderer, sprite_file, sprite_length, sprite_start_line, sprite_frequency);
    players.push_back(new_player);
    return players.size()-1;
  }

  int SdlManager::startLevel(int level_id)
  {
    assert (players.size() > 0);
    assert (!level);

    level = new LevelManager(renderer, level_id, players);
    return 0;
  }

  void SdlManager::mapEvent (
      t_event_type type, int event_id, t_event user_event, t_trigger trigger)
  {
    mapped_events.push_back (
      { type, event_id, user_event, trigger, false });
  }

  void SdlManager::pollEvents ()
  {
    while (!events_queue.empty ())
      events_queue.pop ();
    int x, y;

    const Uint8 *key_states = SDL_GetKeyboardState ( NULL);
    const Uint32 mouse_state = SDL_GetMouseState (&x, &y);

    for (t_event_record & event : mapped_events)
      {
        switch (event.type)
          {
          case ETYPE_KEYBOARD:
            {
              if (key_states[(SDL_Keycode) event.event_id])
                {
                  if (event.trigger == TRIGGER_HOLD
                      || (event.trigger == TRIGGER_DOWN && !event.status))
                    {
                      events_queue.push (
                        { event.user_event,
                          { 0, 0 } });
                    }
                  event.status = true;
                }
              else
                {
                  if (event.trigger == TRIGGER_UP && event.status)
                    {
                      events_queue.push (
                        { event.user_event,
                          { 0, 0 } });
                    }
                  event.status = false;
                }
            }
            break;
          case ETYPE_MOUSE:
            {

            }
            break;
          default:
            assert(0);
          }
      }

    SDL_Event e;
    while (SDL_PollEvent (&e))
      {
        if (e.type == SDL_QUIT)
          {
            events_queue.push (
              { EVENT_EXIT,
                { 0, 0 } });
          }
      }
  }

  t_event SdlManager::pollSingleEvent (
      t_point * point)
  {
    if (events_queue.empty ())
      return EVENT_NONE;

    queued_event event = events_queue.front ();
    events_queue.pop ();

    if (point)
      {
        *point = event.point;
      }

    return event.user_event;
  }

  void SdlManager::applyAction (
      t_action action)
  {
    level->applyAction(0, action);
  }

  void SdlManager::startLoop ()
  {
    start_ticks = SDL_GetTicks ();
  }

  void SdlManager::endLoop ()
  {
    if (GlobalDefs::framerate != FRAMERATE_DYNAMIC)
      {
        Uint32 used_ticks = SDL_GetTicks () - start_ticks;
        if (used_ticks < 1000 / (Uint32) GlobalDefs::framerate)
          SDL_Delay (1000 / (Uint32) GlobalDefs::framerate - used_ticks);
        else
          cerr << "WARNING! Running under framerate. "
              << used_ticks - 1000 / (Uint32) GlobalDefs::framerate << endl;
      }
  }

  void SdlManager::update ()
  {
    level->update();
  }

  void SdlManager::render ()
  {
    assert(level);

    /* Clear screen */
    SDL_SetRenderDrawColor (renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear (renderer);

    level->render();
//    for (BackgroundLayer * bg : bgs)
//      {
//        bg->renderBg (0, 0, window_width, window_height);
//      }
//
//    level->render (0, 0);

    /* Update screen */
    SDL_RenderPresent (renderer);
  }
} /* namespace sdlfw */
