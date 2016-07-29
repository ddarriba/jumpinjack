/*
 * Projectile.cpp
 *
 *  Created on: May 19, 2015
 *      Author: diego
 */

#include <cmath>
#include "Gunshot.h"

namespace jumpinjack
{

  Gunshot::Gunshot (SDL_Renderer * renderer, std::string sprite_file,
                          t_direction direction, t_point & delta,
                          int shooting_angle, int power, int rotation_speed,
                          int lifespan) :
          Projectile (renderer, sprite_file, direction, delta,
                      shooting_angle, power, rotation_speed, lifespan)
  {
    setDirection (direction);
    att_gravity_effect = 0.0;
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

  Gunshot::~Gunshot ()
  {
  }

  t_collision Gunshot::onCollision (Drawable * item, t_direction dir,
                                    t_itemtype type, t_point & point,
                                    t_point & delta, t_point * otherpoint,
                                    t_point * otherdelta)
  {
    t_collision collision_result = COLLISION_IGNORE;

    if (type == ITEM_PLAYER)
    {
      collision_result = COLLISION_IGNORE;
    }
    else if (type != ITEM_CHECK)
    {
      unsetStatus (STATUS_ALIVE);
      collision_result = COLLISION_DIE;
    }

    return collision_result;
  }

} /* namespace jumpinjack */
