/*
* Enemy.cpp
*
*  Created on: May 17, 2015
*      Author: diego
*/

#include "Enemy.h"
#include "BehaviorWalker.h"

using namespace std;

namespace jumpinjack
{

  Enemy::Enemy (SDL_Renderer * renderer,
                std::string sprite_file,
                int sprite_length,
                int sprite_start_line,
                int sprite_frequency) :
    ActiveDrawable (renderer,
                    sprite_file,
                    sprite_length,
                    sprite_start_line,
                    sprite_frequency)
  {
    att_accel = 5;
    att_jump = 0;
    att_speed = 5;
    direction = DIRECTION_LEFT;
    status_count = 0;

    behavior_h_colision = (t_behavior_h_collision)
        (BH_COLLISION_TURN_AT_PASSIVE);// | BH_COLLISION_DIE_AT_PLAYER);

    behavior.push_back(new BehaviorWalker(att_speed));
  }

  Enemy::~Enemy ()
  {
    for (Behavior * b : behavior)
      delete b;
    behavior.clear();
  }

  void Enemy::onCreate (void)
  {
  }

  t_collision Enemy::onCollision (Drawable * item,
                                  t_direction dir,
                                  t_itemtype type, t_point & point,
                                  t_point & delta, t_point * otherpoint,
                                  t_point * otherdelta)
  {

    /* default behavior */
    t_collision collision_result = defaultCollisionBehavior(item, dir, type, point, delta, otherpoint, otherdelta);

    return (t_collision) collision_result;
  }

  t_collision Enemy::getCollisionEffect (t_itemtype type,
      t_direction dir) const
  {
    if (type == ITEM_PLAYER)
      return COLLISION_HIT;
    else
      if (direction == DIRECTION_RIGHT || direction == DIRECTION_LEFT)
        return COLLISION_TURN;
    return COLLISION_IGNORE;
  }

  void Enemy::update (t_point & next_point)
  {
    if (getStatus (STATUS_DYING))
    {
      status_count++;
      render_size.x += 10;
      render_size.y += 10;
      sprite_frequency = 1;
      if (status_count >= 5)
      {
        unsetStatus (STATUS_ALIVE);
      }
    }
    else
    {
      for (Behavior * b : behavior)
        b->update(next_point, direction);
    }

    ActiveDrawable::update(next_point);
  }

} /* namespace jumpinjack */
