/*
 * GlobalDefs.h
 *
 *  Created on: May 16, 2015
 *      Author: diego
 */

#ifndef GLOBALDEFS_H_
#define GLOBALDEFS_H_

#include <SDL2/SDL.h>
#include <cassert>
#include <iostream>
#include <algorithm>

#define FRAMERATE_DYNAMIC  -1
#define MAX_EVENTS         20
#define MAX_LEVEL_ITEMS   400
#define MAX_PLAYERS         4

#define DEFAULT_ACCEL 10
#define DEFAULT_SPEED 15
#define DEFAULT_JUMP   5

#define JUMPING_TRIGGER 100
#define JUMPING_RESET     0

#define PI 3.1415926

typedef SDL_Point t_point;

typedef enum
{
  ETYPE_KEYBOARD,
  ETYPE_MOUSE
} t_event_type;

typedef enum
{
  EVENT_NONE,
  EVENT_MENU_LOAD,
  EVENT_MENU_UNLOAD,
  EVENT_LEFT,
  EVENT_RIGHT,
  EVENT_UP,
  EVENT_UP_RELEASE,
  EVENT_DOWN,
  EVENT_SHOOT,
  EVENT_SPRINT,
  EVENT_EXIT
} t_event;

typedef enum
{
  TRIGGER_HOLD,
  TRIGGER_DOWN,
  TRIGGER_UP
} t_trigger;

typedef enum
{
  ACTION_NONE   = 0,
  ACTION_LEFT   = 1,
  ACTION_RIGHT  = 2,
  ACTION_UP     = 4,
  ACTION_UP_REL = 8,
  ACTION_DOWN   = 16,
  ACTION_SHOOT  = 32,
  ACTION_SPRINT = 64
} t_action;

typedef enum
{
  STATUS_ALIVE = 1,
  STATUS_LISTENING = 2,
  STATUS_UNTOUCHABLE = 4,
  STATUS_DYING = 8
} t_status;

typedef enum
{
  DIRECTION_LEFT  = 1,
  DIRECTION_RIGHT = 2,
  DIRECTION_UP    = 4,
  DIRECTION_DOWN  = 8,
  DIRECTION_HORIZONTAL = 16,
  DIRECTION_VERTICAL   = 32
} t_direction;

typedef enum
{
  ITEM_PASSIVE,
  ITEM_PROJECTILE,
  ITEM_PLAYER,
  ITEM_ENEMY
} t_itemtype;

typedef enum
{
  COLLISION_IGNORE = 0,
  COLLISION_TURN = 1,
  COLLISION_HIT = 2,
  COLLISION_DIE = 3,
  COLLISION_EXPLODE = 4
} t_collision;

typedef struct
{
    t_event_type type;
    int event_id;
    t_event user_event;
    t_trigger trigger;
    bool status;
} t_event_record;

typedef t_point t_dim;

typedef enum
{
    RESOURCE_IMAGE = 1,
    RESOURCE_DATA = 2,
    RESOURCE_SOUND = 3,
    RESOURCE_FONT = 4
} t_resource;

namespace jumpinjack
{

  class GlobalDefs
  {
    public:
      static t_dim window_size;
      static int base_gravity;
      static int max_jump_speed;
      static int max_falling_speed;
      static int base_friction;
      static int framerate;

      static int jump_sensitivity;

      static std::string getResource (t_resource type, const char * file);
  };

} /* namespace sdlfw */

#endif /* GLOBALDEFS_H_ */
