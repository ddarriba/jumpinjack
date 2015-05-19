/*
 * Proyectile.cpp
 *
 *  Created on: May 19, 2015
 *      Author: diego
 */

#include "Proyectile.h"
#include <cmath>

namespace jumpinjack
{

  Proyectile::Proyectile (SDL_Renderer * renderer, std::string sprite_file,
                          t_direction direction, t_point & delta, int angle,
                          int power, int rotation_speed) :
          ActiveDrawable (renderer, sprite_file, 1, 0, 1),
          angle (angle), power (power),
          rotation_speed (rotation_speed)
  {
    setDirection(direction);
    float rad = angle * PI / 180;
    delta.x = (int) round (power * cos (rad));
    if (direction == DIRECTION_LEFT)
      delta.x *= -1;
    delta.y = (int) -round (power * sin (rad));
  }

  Proyectile::~Proyectile ()
  {
  }

  void Proyectile::onCreate (void)
  {
  }

  void Proyectile::onDestroy (void)
  {
    unsetStatus (STATUS_LISTENING);
    setStatus (STATUS_DYING);
  }

  t_collision Proyectile::onCollision (Drawable * item, t_direction dir,
                                       t_itemtype type, t_point & point,
                                       t_point & delta, t_point * otherpoint,
                                       t_point * otherdelta)
  {
    int collision_result = COLLISION_IGNORE;

    if (type != ITEM_PASSIVE && type != ITEM_PLAYER)
      return COLLISION_DIE;
    else
      {
        /* bounce */
        if (dir & DIRECTION_DOWN)
          {
            delta.y = -delta.y + GlobalDefs::base_gravity;
            if (delta.y < -GlobalDefs::base_gravity)
              {
                return COLLISION_TURN;
              }
            else
              {
                setStatus (STATUS_DYING);
                return COLLISION_DIE;
              }
          }
      }
    return (t_collision) collision_result;
  }

  void Proyectile::update (SDL_Point & next_point)
  {
    if (getStatus (STATUS_DYING))
      {
        unsetStatus (STATUS_ALIVE);
      }
    else
      {
        if (direction == DIRECTION_RIGHT)
          next_point.x += 2;
        else
          next_point.x -= 2;
      }
  }

} /* namespace jumpinjack */
