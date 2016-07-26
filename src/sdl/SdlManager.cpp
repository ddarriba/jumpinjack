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
    ingame_menu = NULL;
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
      for (int i = 0; i < SDL_GetNumAudioDrivers(); ++i) {
          printf("%s\n", SDL_GetAudioDriver(i));
        }

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
            //Set window icon
            SDL_Surface * window_icon = IMG_Load("data/img/icon.png");
            SDL_SetWindowIcon(window, window_icon);
            SDL_FreeSurface(window_icon);

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

    if (TTF_Init () == -1)
      {
        printf ("SDL_ttf could not initialize! SDL_ttf Error: %s\n",
        TTF_GetError ());
        success = false;
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
    ingame_menu = new InGameMenu(renderer);
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

    const Uint8 *key_states = SDL_GetKeyboardState ( NULL);
//    int x, y;
//    const Uint32 mouse_state = SDL_GetMouseState (&x, &y);

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

  menu_option SdlManager::showmenu (TTF_Font* font)
  {
    Uint32 time;
    int x, y;
    const int NUMMENU = 4;
    const char* labels[NUMMENU] =
        { "New Game",
          "Continue",
          "Change Controls",
          "Exit" };
    menu_option return_val = MENU_OPTION_CONTINUE;
    const menu_option returnval_array[NUMMENU] =
          { MENU_OPTION_CONTINUE,
            MENU_OPTION_CONTINUE,
            MENU_OPTION_CONTINUE,
            MENU_OPTION_EXIT };
    Drawable * menus[NUMMENU];
    int selected = 0;

    SDL_Color color[2] =
      {
        { 0, 0, 0 },
        { 255, 0, 0 } };
    SDL_Rect pos[NUMMENU];
        int yMenuOffset = 100;

    for (int i = 0; i < NUMMENU; i++)
      {
        menus[i] = new Drawable (renderer, i, false);
        menus[i]->loadFromRenderedText (labels[i], color[0]);
        pos[i].x = GlobalDefs::window_size.x / 2 - menus[i]->getWidth () / 2;
        pos[i].y = yMenuOffset;
        pos[i].w = menus[i]->getWidth ();
        pos[i].h = menus[i]->getHeight ();
        yMenuOffset += pos[i].h;
      }

    SDL_Event event;
    int in_loop = true;
    while (in_loop)
      {
        time = SDL_GetTicks ();
        while (SDL_PollEvent (&event))
          {
            switch (event.type)
              {
              case SDL_QUIT:
                return_val = MENU_OPTION_EXIT;
                in_loop = false;
                break;
              case SDL_MOUSEMOTION:
                x = event.motion.x;
                y = event.motion.y;
                for (int i = 0; i < NUMMENU; i += 1)
                {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w
                        && y >= pos[i].y && y <= pos[i].y + pos[i].h)
                    {
                      selected = i;
                    }
                }
                break;
              case SDL_MOUSEBUTTONDOWN:
                x = event.button.x;
                y = event.button.y;
                for (int i = 0; i < NUMMENU; i += 1)
                  {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w
                        && y >= pos[i].y && y <= pos[i].y + pos[i].h)
                      {
                        return_val = returnval_array[i];
                        in_loop = false;
                      }
                  }
                break;
              case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                  case SDLK_y:
                    return_val = returnval_array[selected];
                    in_loop = false;
                    break;
                  case SDLK_UP:
                    selected += NUMMENU-1;
                    selected %= NUMMENU;
                    break;
                  case SDLK_DOWN:
                    selected += 1;
                    selected %= NUMMENU;
                    break;
                }

                break;
              }
          }
        SDL_RenderClear (renderer);
//        gTextTexture.render ((SCREEN_WIDTH - gTextTexture.getWidth ()) / 2,
//                             (SCREEN_HEIGHT - gTextTexture.getHeight ()) / 2);

        for (int i = 0; i < NUMMENU; i += 1)
        {
          menus[i]->loadFromRenderedText(labels[i], color[selected == i]);
          menus[i]->render (
            {(GlobalDefs::window_size.x - menus[i]->getWidth ()) / 2, pos[i].y},
            {menus[i]->getWidth (), menus[i]->getHeight ()});
        }
        SDL_RenderPresent (renderer);
        //SDL_Flip(screen);
        if (1000 / 30 > (SDL_GetTicks () - time))
          SDL_Delay (1000 / 30 - (SDL_GetTicks () - time));
      }

    for (int j = 0; j < NUMMENU; j++)
                          delete (menus[j]);
    return return_val;
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

  void SdlManager::update (bool game_paused)
  {
    if (game_paused)
      {
        level->pause(true);
        switch(ingame_menu->poll())
        {
          case MENU_CONTINUE:
            level->pause(false);
            events_queue.push (
              { EVENT_MENU_UNLOAD,
                { 0, 0 } });
            break;
          case MENU_EXIT:
            events_queue.push (
              { EVENT_EXIT,
                { 0, 0 } });
            break;
          case MENU_SET_CONTROLS:
          case MENU_MAIN:
          case MENU_NONE:
            /* ignore */
            break;
          }
      }
    else
      {
        level->update ();
      }
  }

  void SdlManager::render (bool render_menu)
  {
    assert(level);

    /* Clear screen */
    SDL_SetRenderDrawColor (renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear (renderer);

    level->render ();

    if (render_menu)
      {
        assert(ingame_menu);
        ingame_menu->renderFixed (
          { 0, 0 });
      }

    /* Update screen */
    SDL_RenderPresent (renderer);
  }
} /* namespace sdlfw */
