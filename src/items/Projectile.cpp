/*
 * Projectile.cpp
 *
 *  Created on: May 19, 2015
 *      Author: diego
 */

#include <cmath>
#include "Projectile.h"

namespace jumpinjack
{

  Projectile::Projectile (SDL_Renderer * renderer, std::string sprite_file,
                          t_direction direction, t_point & delta,
                          int shooting_angle, int power, int rotation_speed,
                          int lifespan) :
          ActiveDrawable (renderer, sprite_file, 10, 0, 1, {24,24}), lifespan (lifespan),
          power (power), rotation_speed (rotation_speed)
  {
    setDirection (direction);
    att_gravity_effect = 1.0;
    float rad = shooting_angle * PI / 180;
    angle = shooting_angle;
    delta.x = (int) round (power * cos (rad));
    if (direction == DIRECTION_LEFT)
      {
        delta.x *= -1;
        angle *= -1;
      }
    delta.y = (int) -round (power * sin (rad));
    start_ticks = SDL_GetTicks ();
  }

  Projectile::~Projectile ()
  {
  }

  void Projectile::onCreate (void)
  {
  }

  void Projectile::onDestroy (void)
  {
    if (getStatus (STATUS_DYING))
      return;

    /* switch sprite */
    unsetStatus (STATUS_LISTENING);
    setStatus (STATUS_DYING);
    sprite_line = 1;
    status_count = 0;
  }

  t_collision Projectile::onCollision (Drawable * item, t_direction dir,
                                       t_itemtype type, t_point & point,
                                       t_point & delta, t_point * otherpoint,
                                       t_point * otherdelta)
  {
    int collision_result = COLLISION_IGNORE;

    if (type == ITEM_PLAYER)
      {
        return COLLISION_IGNORE;
      }

    if (type != ITEM_PASSIVE)
      {
        return COLLISION_EXPLODE;
      }
    else
      {
        /* bounce */
        if (dir & DIRECTION_VERTICAL)
          {
            delta.y = -delta.y + GlobalDefs::base_gravity;
            if (delta.y < -GlobalDefs::base_gravity)
              {
                return COLLISION_TURN;
              }
          }
        else if (dir & DIRECTION_HORIZONTAL)
          {
            delta.x = -delta.x;
            if (direction == DIRECTION_LEFT)
              direction = DIRECTION_RIGHT;
            else
              direction = DIRECTION_LEFT;
          }
      }
    return (t_collision) collision_result;
  }

  void Projectile::update (t_point & next_point)
  {
    if (getStatus (STATUS_DYING))
      {
        status_count++;
        if (status_count >= 10)
        {
          unsetStatus (STATUS_ALIVE);
        }
      }
    else
    {
      if (direction == DIRECTION_RIGHT)
      {
        next_point.x += 2;
        angle += rotation_speed;
      }
      else
      {
        next_point.x -= 2;
        angle -= rotation_speed;
      }
      if (SDL_GetTicks () - start_ticks > lifespan)
      {
        setStatus (STATUS_DYING);
      }
    }

    ActiveDrawable::update(next_point);
  }

} /* namespace jumpinjack */
