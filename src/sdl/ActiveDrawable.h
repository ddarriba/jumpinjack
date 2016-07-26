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

  typedef enum {
    BH_COLLISION_IGNORE_ALL      = 0,
    BH_COLLISION_TURN_AT_PASSIVE = 1,
    BH_COLLISION_TURN_AT_ACTIVE  = 2,
    BH_COLLISION_TURN_AT_PLAYER  = 4,
    BH_COLLISION_DIE_AT_PASSIVE  = 8,
    BH_COLLISION_DIE_AT_ACTIVE   = 16,
    BH_COLLISION_DIE_AT_PLAYER   = 32
  } t_behavior_h_collision;

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
      double getGravityEffect () const;
      t_direction getDirection () const;
      void setDirection (t_direction dir);
      void turn ( void );
      int multipleJump( void ) const;

      int onJump;
      int jumpId;
    protected:
      t_collision defaultCollisionBehavior (Drawable * item,
                                            t_direction dir,
                                            t_itemtype type,
                                            t_point & point,
                                            t_point & delta,
                                            t_point * otherpoint = 0,
                                            t_point * otherdelta = 0);

      SDL_Rect renderQuad;

      t_direction direction;

      int hit_counter;
      int angle;

      /* attributes */
      int att_accel;
      int att_speed;
      int att_jump;
      double att_gravity_effect;

      int n_jumps;

      /* behavior */
      t_behavior_h_collision behavior_h_colision;
      int status_count;
  };

} /* namespace jumpinjack */

#endif /* SDL_ACTIVEDRAWABLE_H_ */
