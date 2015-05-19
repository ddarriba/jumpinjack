/*
 * PassiveDrawable.h
 *
 *  Created on: May 19, 2015
 *      Author: diego
 */

#ifndef SDL_PASSIVEDRAWABLE_H_
#define SDL_PASSIVEDRAWABLE_H_

#include "DrawableItem.h"

namespace jumpinjack
{

  class PassiveDrawable : public DrawableItem
  {
    public:
      PassiveDrawable (SDL_Renderer * renderer, std::string sprite_file,
                       int sprite_length, int sprite_line, int sprite_frequency,
                       int zindex) :
              DrawableItem (renderer, zindex, sprite_file, sprite_length,
                            sprite_start_line, sprite_frequency)
      {
      }
      virtual ~PassiveDrawable (){}

      virtual void onCreate (void)
      {
        setStatus (STATUS_ALIVE);
      }

      virtual void onDestroy (void)
      {
        unsetStatus (STATUS_ALIVE);
      }
  };

} /* namespace jumpinjack */

#endif /* SDL_PASSIVEDRAWABLE_H_ */
