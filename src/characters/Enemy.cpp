/*
 * Enemy.cpp
 *
 *  Created on: May 17, 2015
 *      Author: diego
 */

#include "Enemy.h"

using namespace std;

namespace jumpinjack
{

  Enemy::Enemy (SDL_Renderer * renderer, std::string sprite_file,
                int sprite_length, int sprite_start_line, int sprite_frequency) :
          ActiveDrawable (renderer, sprite_file, sprite_length,
                          sprite_start_line, sprite_frequency)
  {
    att_accel = 5;
    att_jump = 0;
    att_speed = 5;
    direction = DIRECTION_LEFT;
    status_count = 0;
  }

  Enemy::~Enemy ()
  {
  }

  void Enemy::onCreate (void)
  {
  }

  void Enemy::onDestroy (void)
  {
    unsetStatus (STATUS_LISTENING);
    setStatus (STATUS_DYING);
  }

  t_collision Enemy::onCollision (Drawable * item, t_direction dir,
                                  t_itemtype type, t_point & point,
                                  t_point & delta, t_point * otherpoint,
                                  t_point * otherdelta)
  {
    int collision_result = COLLISION_IGNORE;
    if ((dir & DIRECTION_HORIZONTAL) && type != ITEM_PLAYER)
      {
        if (direction == DIRECTION_RIGHT)
          direction = DIRECTION_LEFT;
        else
          direction = DIRECTION_RIGHT;
        collision_result |= COLLISION_TURN;
      }
    if ((dir & DIRECTION_UP) && type == ITEM_PLAYER)
      {
        return COLLISION_DIE;
        /* DIE */
      }
    return (t_collision) collision_result;
  }

  void Enemy::update (SDL_Point & next_point)
  {
    if (getStatus (STATUS_DYING))
      {
        status_count++;
        //*y += 10;
        if (status_count >= 10)
          {
            unsetStatus (STATUS_ALIVE);
          }
      }
    else
      {
        if (direction == DIRECTION_RIGHT)
          next_point.x = att_speed;
        else
          next_point.x = -att_speed;
      }
  }

} /* namespace jumpinjack */
