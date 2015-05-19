/*
 * StaticAnimation.h
 *
 *  Created on: May 19, 2015
 *      Author: diego
 */

#ifndef ITEMS_STATICANIMATION_H_
#define ITEMS_STATICANIMATION_H_

#include "../sdl/PassiveDrawable.h"

#define LIFESPAN_ONE_LOOP 999999

namespace jumpinjack
{

  class StaticAnimation : public PassiveDrawable
  {
    public:
      StaticAnimation (SDL_Renderer * renderer, std::string sprite_file,
                       int sprite_length, int sprite_start_line,
                       int sprite_frequency, int lifespan, int zindex);
      virtual ~StaticAnimation ();

      virtual void update (SDL_Point & next_point);
      virtual void renderFixed (t_point point);

    private:
      void convertCoordinates (t_point & p);

      unsigned int start_time;
      unsigned int lifespan;    /* millis */
  };

} /* namespace jumpinjack */

#endif /* ITEMS_STATICANIMATION_H_ */
