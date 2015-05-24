/*
 * ActiveDrawable.cpp
 *
 *  Created on: May 16, 2015
 *      Author: diego
 */

#include "ActiveDrawable.h"

using namespace std;

namespace jumpinjack
{

  ActiveDrawable::ActiveDrawable (SDL_Renderer * renderer,
                                  std::string sprite_file, int sprite_length,
                                  int sprite_start_line, int sprite_frequency) :
          DrawableItem (renderer, 500, sprite_file, sprite_length,
                        sprite_start_line, sprite_frequency),
          hit_counter (0), angle (0), att_accel (DEFAULT_ACCEL),
          att_speed (DEFAULT_SPEED), att_jump (DEFAULT_JUMP)
  {
    direction = (t_direction) (DIRECTION_RIGHT | DIRECTION_HORIZONTAL);

    onJump = 0;
  }

  ActiveDrawable::~ActiveDrawable ()
  {
  }

  void ActiveDrawable::convertCoordinates (t_point & p)
  {
    /* convert x,y in surface to the point where it should be drawn */
    p.x -= sprite_size.x / 2;
    p.y -= sprite_size.y;
  }

  void ActiveDrawable::update (SDL_Point & next_point)
    {
      renderQuad = updateSprite();
    }

  void ActiveDrawable::renderFixed (t_point point)
  {
    convertCoordinates (point);

    SDL_RendererFlip flip =
        (direction & DIRECTION_RIGHT) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

    render (point, sprite_size, &renderQuad, flip, angle);
  }
} /* namespace jumpinjack */
