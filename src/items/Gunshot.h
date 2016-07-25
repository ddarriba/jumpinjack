#ifndef ITEMS_GUNSHOT_H_
#define ITEMS_GUNSHOT_H_

#include "Projectile.h"

namespace jumpinjack
{

  class Gunshot : public Projectile
  {
    public:
      Gunshot (SDL_Renderer * renderer,
               std::string sprite_file,
               t_direction direction, t_point & delta,
               int shooting_angle,
               int power,
               int rotation_speed = 0,
               int lifespan = 1000);
      virtual ~Gunshot ();

      virtual t_collision onCollision (Drawable * item, t_direction dir,
                                       t_itemtype type, t_point & point,
                                       t_point & delta,
                                       t_point * otherpoint = 0,
                                       t_point * otherdelta = 0);
  };

} /* namespace jumpinjack */

#endif /* ITEMS_GUNSHOT_H_ */
