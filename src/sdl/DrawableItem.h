/*
 * DrawableItem.h
 *
 *  Created on: May 17, 2015
 *      Author: diego
 */

#ifndef SDL_DRAWABLEITEM_H_
#define SDL_DRAWABLEITEM_H_

#include "Drawable.h"

namespace jumpinjack
{

  class DrawableItem : public Drawable
  {
    public:
      DrawableItem (SDL_Renderer * renderer, int zindex);
      virtual ~DrawableItem ();

      void setStatus (t_status s);
      void unsetStatus (t_status s);
      bool getStatus (t_status s);

    private:
      t_status status;
  };

} /* namespace jumpinjack */

#endif /* SDL_DRAWABLEITEM_H_ */
