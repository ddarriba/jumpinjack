/*
 * StaticAnimation.cpp
 *
 *  Created on: May 19, 2015
 *      Author: diego
 */

#include "StaticAnimation.h"

namespace jumpinjack
{

  StaticAnimation::StaticAnimation (SDL_Renderer * renderer,
                                    std::string sprite_file, int sprite_length,
                                    int sprite_line, int sprite_frequency,
                                    int lifespan, int zindex) :
          PassiveDrawable (renderer, sprite_file, sprite_length, sprite_line,
                           sprite_frequency, zindex), lifespan (lifespan)
  {
    unsetStatus(STATUS_LISTENING);
    start_time = SDL_GetTicks ();
  }

  StaticAnimation::~StaticAnimation ()
  {
    // TODO Auto-generated destructor stub
  }

  void StaticAnimation::convertCoordinates (t_point & p)
  {
    /* convert x,y in surface to the point where it should be drawn */
    p.x -= sprite_size.x / 2;
    p.y -= sprite_size.y / 2;
  }

  void StaticAnimation::renderFixed (t_point point)
  {
//    t_rect renderQuad = updateSprite ();

    convertCoordinates (point);

    if (getStatus(STATUS_ALIVE))
      render (point, sprite_size, &renderQuad, SDL_FLIP_NONE);
  }

  void StaticAnimation::update (t_point & next_point)
  {
    renderQuad = updateSprite ();
    if (lifespan == LIFESPAN_ONE_LOOP)
      {
        if ((sprite_index==(sprite_length-1)) && (sprite_freq_divisor==(sprite_frequency-1)))
          unsetStatus (STATUS_ALIVE);
      }
    else if (lifespan > 0 && (SDL_GetTicks () - start_time > lifespan))
          {
            unsetStatus (STATUS_ALIVE);
          }
  }
} /* namespace jumpinjack */
