/*
 * Projectile.cpp
 *
 *  Created on: May 19, 2015
 *      Author: diego
 */

#include <cmath>
#include "Checkpoint.h"

namespace jumpinjack
{

  Checkpoint::Checkpoint (SDL_Renderer * renderer, std::string sprite_file) :
          ActiveDrawable (renderer, sprite_file, 1, 0, 1, {64,64})
  {
    setDirection (DIRECTION_LEFT);
    taken = false;
  }

  Checkpoint::~Checkpoint ()
  {
  }

  void Checkpoint::onCreate (void)
  {
  }

  void Checkpoint::onDestroy (void)
  {
    assert(0);
  }

  t_collision Checkpoint::onCollision (Drawable * item, t_direction dir,
                                       t_itemtype type, t_point & point,
                                       t_point & delta, t_point * otherpoint,
                                       t_point * otherdelta)
  {
    int collision_result = COLLISION_IGNORE;
    if (!taken && type == ITEM_PLAYER)
    {
      taken = true;
      sprite_line = 1;
      return COLLISION_CHECKPOINT;
    }
    return (t_collision) collision_result;
  }

  void Checkpoint::update (SDL_Point & next_point)
  {
    ActiveDrawable::update(next_point);
  }

} /* namespace jumpinjack */
