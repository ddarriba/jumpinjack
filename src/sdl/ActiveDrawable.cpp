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
          DrawableItem (renderer, 500), sprite_length (sprite_length),
          sprite_start_line (sprite_start_line),
          sprite_frequency (sprite_frequency), sprite_line (sprite_start_line),
          sprite_index (0), counter (0), hit_counter (0),
          att_accel (DEFAULT_ACCEL), att_speed (DEFAULT_SPEED),
          att_jump (DEFAULT_JUMP)
  {
    direction = (t_direction) (DIRECTION_RIGHT | DIRECTION_HORIZONTAL);

    loadFromFile (sprite_file);

    /* assuming square sprites */
    sprite_size =
      { image_size.x / sprite_length, image_size.x / sprite_length};

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

  void ActiveDrawable::renderFixed (t_point point)
  {
    counter = (counter + 1) % sprite_frequency;
    if (!counter)
      sprite_index = (sprite_index + 1) % sprite_length;

    convertCoordinates (point);

    t_point sprite_offset =
      { sprite_index * sprite_size.x, sprite_line * sprite_size.y };

    SDL_Rect renderQuad =
      { sprite_offset.x, sprite_offset.y, sprite_size.x, sprite_size.y };

    SDL_RendererFlip flip =
        (direction & DIRECTION_RIGHT) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

    render (point, sprite_size, &renderQuad, flip);
  }
} /* namespace jumpinjack */
