/*
 * Player.h
 *
 *  Created on: May 16, 2015
 *      Author: diego
 */

#ifndef CHARACTERS_PLAYER_H_
#define CHARACTERS_PLAYER_H_

#include "../sdl/ActiveDrawable.h"

namespace jumpinjack
{

  typedef enum
  {
    PLAYER_STAND  = 0,
    PLAYER_RUN    = 1,
    PLAYER_JUMP   = 2,
    PLAYER_FALL   = 3,
    PLAYER_HIT    = 4,
    PLAYER_CROUCH = 5,
    PLAYER_DEAD   = 6
  } playerState;

  class Player : public ActiveDrawable
  {
    public:
      Player (SDL_Renderer * renderer, std::string sprite_file,
              int sprite_length, int sprite_start_line, int sprite_frequency);
      virtual ~Player ();

      void setState (playerState new_state)
      {
        current_state = new_state;
      }
      playerState getState () const
      {
        return current_state;
      }

      virtual void onCreate (void);
      virtual void onDestroy (void);
      virtual t_collision onCollision (Drawable * item, t_direction dir,
                                       t_itemtype type, t_point & point,
                                       t_point & delta,
                                       t_point * otherpoint = 0,
                                       t_point * otherdelta = 0);
      virtual void update (SDL_Point & next_point);

      virtual void renderFixed (t_point point);

    private:
      playerState current_state;
  };

} /* namespace sdlfw */

#endif /* CHARACTERS_PLAYER_H_ */
