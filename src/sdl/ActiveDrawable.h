/*
 * ActiveDrawable.h
 *
 *  Created on: May 16, 2015
 *      Author: diego
 */

#ifndef SDL_ACTIVEDRAWABLE_H_
#define SDL_ACTIVEDRAWABLE_H_

#include "DrawableItem.h"

#include <string>

namespace jumpinjack
{

  class ActiveDrawable : public DrawableItem
  {
    public:
      ActiveDrawable (SDL_Renderer * renderer, std::string sprite_file,
                      int sprite_length, int sprite_start_line,
                      int sprite_frequency);
      virtual ~ActiveDrawable ();

      virtual int getWidth ()
      {
        return sprite_size.x;
      }
      virtual int getHeight ()
      {
        return sprite_size.y * 2 / 3;
      }

      void convertCoordinates (t_point & p);

      virtual t_collision onCollision (Drawable * item, t_direction dir,
                                       t_itemtype type, t_point & point,
                                       t_point & delta,
                                       t_point * otherpoint = 0,
                                       t_point * otherdelta = 0) = 0;

      virtual void renderFixed (t_point point);

      virtual void update (SDL_Point & next_point);

      int getAccel () const;
      int getSpeed () const;
      int getJump () const;
      t_direction getDirection () const;
      void setDirection (t_direction dir);
      int multipleJump( void ) const;

      int onJump;
      int jumpId;
    protected:
      SDL_Rect renderQuad;

      t_direction direction;

      int hit_counter;
      int angle;

      /* attributes */
      int att_accel;
      int att_speed;
      int att_jump;

      int n_jumps;
  };

} /* namespace jumpinjack */

#endif /* SDL_ACTIVEDRAWABLE_H_ */
