/*
 * ActiveDrawable.cpp
 *
 *  Created on: May 16, 2015
 *      Author: diego
 */

#include "ActiveDrawable.h"

using namespace std;

namespace jumpinjack
{

  ActiveDrawable::ActiveDrawable (SDL_Renderer * renderer,
                                  std::string sprite_file, int sprite_length,
                                  int sprite_start_line, int sprite_frequency,
                                  t_dim sprite_render_size) :
          DrawableItem (renderer, 500, sprite_file, sprite_length,
                        sprite_start_line, sprite_frequency,
                        sprite_render_size),
          hit_counter (0), angle (0), att_accel (DEFAULT_ACCEL),
          att_speed (DEFAULT_SPEED), att_jump (DEFAULT_JUMP)
  {
    direction = (t_direction) (DIRECTION_RIGHT | DIRECTION_HORIZONTAL);

    onJump = 0;
    jumpId = 0;
    n_jumps = 2;
    att_gravity_effect = 1.0;

    behavior_h_colision = BH_COLLISION_IGNORE_ALL;
  }

  ActiveDrawable::~ActiveDrawable ()
  {
  }

  void ActiveDrawable::convertCoordinates (t_point & p)
  {
    /* convert x,y in surface to the point where it should be drawn */
    p.x -= render_size.x / 2;
    p.y -= render_size.y;
  }

  void ActiveDrawable::onDestroy (void)
  {
    if (!getStatus(STATUS_DYING))
    {
      unsetStatus (STATUS_LISTENING);
      setStatus (STATUS_DYING);
      sprite_line = 1;
      status_count = 0;
    }
  }

  void ActiveDrawable::update (t_point & next_point)
    {
      renderQuad = updateSprite();
    }

  void ActiveDrawable::renderFixed (t_point point)
  {
    convertCoordinates (point);

    SDL_RendererFlip flip =
        (direction & DIRECTION_RIGHT) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

    render (point, render_size, &renderQuad, flip, angle);
  }

  int ActiveDrawable::getAccel(void) const
  {
	  return att_accel;
  }

  int ActiveDrawable::getSpeed(void) const
  {
	  return att_speed;
  }

  int ActiveDrawable::getJump(void) const
  {
    return att_jump;
  }

  double ActiveDrawable::getGravityEffect(void) const
  {
    return att_gravity_effect;
  }

  t_direction ActiveDrawable::getDirection(void) const {
	return direction;
  }

  void ActiveDrawable::setDirection(t_direction dir) {
	direction = dir;
  }

  void ActiveDrawable::turn (void)
  {
      setDirection((direction == DIRECTION_RIGHT)?DIRECTION_LEFT:DIRECTION_RIGHT);
  }

  int ActiveDrawable::multipleJump(void) const {
    return n_jumps;
  }

  t_collision ActiveDrawable::defaultCollisionBehavior (Drawable * item,
                                                        t_direction dir,
                                                        t_itemtype type,
                                                        t_point & point,
                                                        t_point & delta,
                                                        t_point * otherpoint,
                                                        t_point * otherdelta)
  {
    int collision_result = COLLISION_IGNORE;
    if (dir & DIRECTION_HORIZONTAL) //type != ITEM_PLAYER)
    {
      switch (type)
      {
        case ITEM_PASSIVE:
          if (behavior_h_colision & BH_COLLISION_TURN_AT_PASSIVE)
          {
            turn ();
            collision_result |= COLLISION_TURN;
          }
          else if (behavior_h_colision & BH_COLLISION_DIE_AT_PASSIVE)
          {
            collision_result = COLLISION_DIE;
          }
          break;
        case ITEM_ENEMY:
          if (behavior_h_colision & BH_COLLISION_TURN_AT_ACTIVE)
          {
            turn ();
            collision_result |= COLLISION_TURN;
          }
          else if (behavior_h_colision & BH_COLLISION_DIE_AT_ACTIVE)
          {
            collision_result = COLLISION_DIE;
          }
          break;
        case ITEM_PLAYER:
          if (behavior_h_colision & BH_COLLISION_TURN_AT_PLAYER)
          {
            turn ();
            collision_result |= COLLISION_TURN;
          }
          else if (behavior_h_colision & BH_COLLISION_DIE_AT_PLAYER)
          {
            collision_result = COLLISION_DIE;
          }
          break;
        case ITEM_PROJECTILE:
          collision_result = COLLISION_DIE;
          break;
        case ITEM_CHECK:
          collision_result = COLLISION_IGNORE;
          break;
      }
    }

    if (((dir & DIRECTION_UP) && type == ITEM_PLAYER)
        || type == ITEM_PROJECTILE)
    {
      return COLLISION_DIE;
      /* DIE */
    }
    return (t_collision) collision_result;
  }
} /* namespace jumpinjack */
