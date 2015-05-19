/*
 * BackgroundDrawable.h
 *
 *  Created on: May 17, 2015
 *      Author: diego
 */

#ifndef SDL_BACKGROUNDDRAWABLE_H_
#define SDL_BACKGROUNDDRAWABLE_H_

#include "Drawable.h"

namespace jumpinjack
{

  class BackgroundDrawable : public Drawable
  {
    public:
      BackgroundDrawable (SDL_Renderer * renderer, std::string imgfile,
                          int parallax_level, bool repeat_x = true,
                          int auto_speed = 0);
      virtual ~BackgroundDrawable ();

      void renderFixed (t_point point, double angle = 0.0);
    private:
      int parallax_level;
      bool repeat_x;
      int auto_speed;
      int counter;
  };

} /* namespace jumpinjack */

#endif /* SDL_BACKGROUNDDRAWABLE_H_ */
