/*
 * Surface.cpp
 *
 *  Created on: May 17, 2015
 *      Author: diego
 */

#include "Surface.h"

#include <cassert>
#include <iostream>

using namespace std;

namespace jumpinjack
{

  Surface::Surface (
      std::string file)
  {
    surface = IMG_Load (file.c_str ());
    if (surface == NULL)
      {
        printf ("Unable to load image %s! SDL_image Error: %s\n", file.c_str (),
        IMG_GetError ());
        assert(0);
      }
    pixels = (int *) surface->pixels;

    offset_h = surface->h - GlobalDefs::window_size.y;
  }

  Surface::~Surface ()
  {
    SDL_FreeSurface (surface);
  }

  int Surface::getPixel (
      t_point p)
  {
    //Get the requested pixel
    return (int) (pixels[(p.y * surface->w) + p.x] & 0xFFFFFF);
  }

  pixelType Surface::testPixel (
      t_point p)
  {
    int red, green, blue;
    p.y += offset_h;
    if (p.x < 0 || p.x >= surface->w || p.y < 0 || p.y >= surface->h)
      return PIXELTYPE_OUT;

    int pixel = getPixel (p);

    /* Get Red component */
    blue = pixel & 0xFF0000;
    blue = blue >> 16;

    /* Get Green component */
    green = pixel & 0xFF00;
    green = green >> 8;

    /* Get Blue component */
    red = pixel & 0xFF;

    if (((int) red == 255) && !green && !blue)
      {
        return PIXELTYPE_SOLID;
      }
    else if (((int) green == 255) && !red && !blue)
      {
        return PIXELTYPE_DOWN_ONLY;
      }
    else
      {
        return PIXELTYPE_NONE;
      }
  }
} /* namespace jumpinjack */
