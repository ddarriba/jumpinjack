/*
 * BackgroundDrawable.cpp
 *
 *  Created on: May 17, 2015
 *      Author: diego
 */

#include "BackgroundDrawable.h"

using namespace std;

namespace jumpinjack
{

  BackgroundDrawable::BackgroundDrawable (SDL_Renderer * renderer,
                                          std::string imgfile,
                                          int parallax_level, bool repeat_x,
                                          int auto_speed) :
          Drawable (renderer, 0, false), parallax_level (parallax_level),
          repeat_x (repeat_x), auto_speed (auto_speed)
  {
    loadFromFile (imgfile);
    counter = 0;
  }

  BackgroundDrawable::~BackgroundDrawable ()
  {
  }

  void BackgroundDrawable::renderFixed (t_point point, double angle)
  {
    if (auto_speed)
      {
        counter++;
        point.x += counter * auto_speed;
//        if ((counter*auto_speed)%image_size.x == 0)
//          counter = 0;
        if ((point.x / parallax_level % image_size.x) == 0)
          counter = 0;
      }
    SDL_Rect renderQuad =
      { -(point.x / parallax_level % image_size.x), GlobalDefs::window_size.y
          - image_size.y, image_size.x, image_size.y };

    //Render to screen
    SDL_RenderCopyEx (renderer, mTexture, NULL, &renderQuad, 0, NULL,
                      SDL_FLIP_NONE);

    if (repeat_x)
      {
        renderQuad.x = -(point.x / parallax_level % image_size.x)
            + image_size.x;
        SDL_RenderCopyEx (renderer, mTexture, NULL, &renderQuad, 0, NULL,
                          SDL_FLIP_NONE);
      }
  }
} /* namespace jumpinjack */
