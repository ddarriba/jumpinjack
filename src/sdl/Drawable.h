/*
 * fwDrawable.h
 *
 *  Created on: May 3, 2015
 *      Author: diego
 */

#ifndef DRAWABLE_H_
#define DRAWABLE_H_

#include <string>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../GlobalDefs.h"

namespace jumpinjack
{

  typedef struct
  {
      SDL_Surface * surface;
      SDL_Texture * texture;
  } graphicInfo;

  class Drawable
  {
    public:
      Drawable (SDL_Renderer * renderer, int zIndex, bool cached = false);
      virtual
      ~Drawable ();

      bool loadFromFile (std::string path);
      bool loadFromRenderedText( std::string textureText, SDL_Color textColor );

      virtual int getWidth (void) const;
      virtual int getHeight (void) const;

      std::string getFilePath(void) const;

      void setColor (Uint8 red, Uint8 green, Uint8 blue);
      void setBlendMode (SDL_BlendMode blending);
      void setAlpha (Uint8 alpha);

      virtual void render (t_point point,
                           t_dim size,
                           t_rect * clip = NULL,
                           SDL_RendererFlip flip = SDL_FLIP_NONE,
                           double angle = 0.0,
                           t_point * center = NULL);
      virtual void renderFixed (t_point point) {};

      virtual t_collision getCollisionEffect (t_itemtype type,
                                              t_direction dir) const;

      static void cleanCache (void);

    protected:
      std::string file_path;
      SDL_Renderer * renderer;
      SDL_Surface* mSurface;
      SDL_Texture* mTexture;

      //Image dimensions
      t_dim image_size;
      t_dim render_size;

      int zIndex;

      bool cached;

      void free (void);

    private:
      static std::map<std::string, graphicInfo> cachedSurfaces;
  };

} /* namespace jumpinjack */

#endif /* DRAWABLE_H_ */
