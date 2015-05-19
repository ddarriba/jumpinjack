/*
 * Proyectile.h
 *
 *  Created on: May 19, 2015
 *      Author: diego
 */

#ifndef ITEMS_PROYECTILE_H_
#define ITEMS_PROYECTILE_H_

#include "../sdl/ActiveDrawable.h"

namespace jumpinjack
{

  class Proyectile : public ActiveDrawable
  {
    public:
      Proyectile (SDL_Renderer * renderer, std::string sprite_file,
                  t_direction direction, t_point & delta, int angle, int power,
                  int rotation_speed = 10);
      virtual ~Proyectile ();

      virtual void onCreate (void);
      virtual void onDestroy (void);
      virtual t_collision onCollision (Drawable * item, t_direction dir,
                                       t_itemtype type, t_point & point,
                                       t_point & delta,
                                       t_point * otherpoint = 0,
                                       t_point * otherdelta = 0);
      virtual void update (SDL_Point & next_point);

    private:
      int angle;
      int power;
      int rotation_speed;
  };

} /* namespace jumpinjack */

#endif /* ITEMS_PROYECTILE_H_ */
