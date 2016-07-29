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
    status_count = 0;
    state = CKP_INIT;
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
      status_count = 0;
      sprite_line = 1;
      state = CKP_HIT;
      return COLLISION_CHECKPOINT;
    }
    return (t_collision) collision_result;
  }

  void Checkpoint::update (t_point & next_point)
  {
    if (state == CKP_HIT)
    {
      status_count++;
      if (status_count < 7)
        next_point.y -= 6;
      angle += 10;
      if (status_count >= 36)
        state = CKP_END;
    }
    ActiveDrawable::update(next_point);
  }

} /* namespace jumpinjack */
