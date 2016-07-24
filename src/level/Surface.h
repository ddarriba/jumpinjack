/*
 * Surface.h
 *
 *  Created on: May 17, 2015
 *      Author: diego
 */

#ifndef SDL_SURFACE_H_
#define SDL_SURFACE_H_

#include "../GlobalDefs.h"
#include <SDL2/SDL_image.h>

#include <string>

namespace jumpinjack
{
  enum pixelType {
    PIXELTYPE_OUT,
    PIXELTYPE_NONE,
    PIXELTYPE_SOLID,
    PIXELTYPE_UP_ONLY,
    PIXELTYPE_DOWN_ONLY,
	PIXELTYPE_DEATH
  };

  class Surface
  {
    public:
      Surface (std::string file);
      virtual ~Surface ();

      int getPixel(t_point p);
      pixelType testPixel(t_point p);
    private:
      SDL_Surface * surface;
      int * pixels;

      int offset_h;
  };

} /* namespace jumpinjack */

#endif /* SDL_SURFACE_H_ */
