/*
 * Projectile.h
 *
 *  Created on: May 19, 2015
 *      Author: diego
 */

#ifndef ITEMS_PROJECTILE_H_
#define ITEMS_PROJECTILE_H_

#include "../sdl/ActiveDrawable.h"

namespace jumpinjack
{

  class Projectile : public ActiveDrawable
  {
    public:
      Projectile (SDL_Renderer * renderer, std::string sprite_file,
                  t_direction direction, t_point & delta, int shooting_angle, int power,
                  int rotation_speed = 10, int lifespan = 4000);
      virtual ~Projectile ();

      virtual void onCreate (void);
      virtual void onDestroy (void);
      virtual t_collision onCollision (Drawable * item, t_direction dir,
                                       t_itemtype type, t_point & point,
                                       t_point & delta,
                                       t_point * otherpoint = 0,
                                       t_point * otherdelta = 0);
      virtual void update (SDL_Point & next_point);

    protected:
      unsigned int start_ticks; /* starting time */
      unsigned int lifespan;    /* time (millis) the projectile is alive */
      int power;                /* shoot power (speed) */
      int rotation_speed;       /* rotation speed */
  };

} /* namespace jumpinjack */

#endif /* ITEMS_PROJECTILE_H_ */
