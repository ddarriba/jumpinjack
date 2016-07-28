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
      DrawableItem (SDL_Renderer * renderer, int zindex,
                    std::string sprite_file, int sprite_length,
                    int sprite_start_line, int sprite_frequency,
                    t_dim sprite_render_size = {0,0});
      virtual ~DrawableItem ();

      void setStatus (t_status s);
      void unsetStatus (t_status s);
      bool getStatus (t_status s);

      void resetSpriteIndex ( void );

      virtual void onCreate (void) = 0;
      virtual void onDestroy (void) = 0;
      virtual void update (SDL_Point & next_point) = 0;

    protected:
      SDL_Rect updateSprite(void);

      t_dim sprite_size;

      int sprite_length;
      int sprite_start_line;
      int sprite_frequency;
      int sprite_freq_divisor;

      int sprite_line;
      int sprite_index;

    private:
      t_status status;
  };

} /* namespace jumpinjack */

#endif /* SDL_DRAWABLEITEM_H_ */
