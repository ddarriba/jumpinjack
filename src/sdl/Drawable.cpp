/*
 * fwDrawable.cpp
 *
 *  Created on: May 3, 2015
 *      Author: diego
 */

#include "Drawable.h"

using namespace std;

namespace jumpinjack
{

  Drawable::Drawable (SDL_Renderer * renderer, int zIndex, bool cached) :
          renderer (renderer), zIndex (zIndex), cached (cached)
  {
    mTexture = 0;
    image_size =
      { 0,0};

    mSurface = 0;
  }

  Drawable::~Drawable ()
  {
    //Deallocate
    free ();
  }

  void Drawable::free ()
  {
    //Free texture if it exists
    if (!cached && mTexture)
      {
        SDL_FreeSurface (mSurface);
        SDL_DestroyTexture (mTexture);
        mTexture = 0;
      }
  }

  bool Drawable::loadFromFile (std::string path)
  {
    //Get rid of preexisting texture
    free ();
    bool surfaceLoaded = false;
    if (cached)
      {
        if (cachedSurfaces.find (path) != cachedSurfaces.end ())
          {
            mSurface = cachedSurfaces[path].surface;
            mTexture = cachedSurfaces[path].texture;
            surfaceLoaded = true;
          }
      }
    if (!surfaceLoaded)
      {
        //The final texture
        SDL_Texture* newTexture = NULL;

        //Load image at specified path
        SDL_Surface* loadedSurface = IMG_Load (path.c_str ());
        if (loadedSurface == NULL)
          {
            printf ("Unable to load image %s! SDL_image Error: %s\n",
                    path.c_str (),
                    IMG_GetError ());
          }
        else
          {
            //SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
            //Color key image
            SDL_SetColorKey (loadedSurface, SDL_TRUE,
                             SDL_MapRGB (loadedSurface->format, 0xFF, 0, 0xFF));

            //Create texture from surface pixels
            newTexture = SDL_CreateTextureFromSurface (renderer, loadedSurface);
            if (newTexture == NULL)
              {
                printf ("Unable to create texture from %s! SDL Error: %s\n",
                        path.c_str (), SDL_GetError ());
              }
          }
        //Return success
        mSurface = loadedSurface;
        mTexture = newTexture;
        if (cached)
          {
            cachedSurfaces[path] =
              { mSurface, mTexture};
          }
      }
    image_size =
      { mSurface->w, mSurface->h};
    return mTexture != NULL;
  }

  bool Drawable::loadFromRenderedText( std::string textureText, SDL_Color textColor )
  {
      //Get rid of preexisting texture
      free();

      TTF_Font * gFont = TTF_OpenFont(GlobalDefs::getResource(RESOURCE_FONT, "zorque.ttf").c_str(),40);

      //Render text surface
      SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
      TTF_CloseFont(gFont);

      if( textSurface == NULL )
      {
          printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
      }
      else
      {
          //Create texture from surface pixels
          mTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
          if( mTexture == NULL )
          {
              printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
          }
          else
          {
              //Get image dimensions
              image_size = {
              textSurface->w,
              textSurface->h};
          }

          //Get rid of old surface
          SDL_FreeSurface( textSurface );
      }

      //Return success
      return mTexture != NULL;
  }

  void Drawable::setColor (Uint8 red, Uint8 green, Uint8 blue)
  {
    //Modulate texture rgb
    SDL_SetTextureColorMod (mTexture, red, green, blue);
  }

  void Drawable::setBlendMode (SDL_BlendMode blending)
  {
    //Set blending function
    SDL_SetTextureBlendMode (mTexture, blending);
  }

  void Drawable::setAlpha (Uint8 alpha)
  {
    //Modulate texture alpha
    SDL_SetTextureAlphaMod (mTexture, alpha);
  }

  void Drawable::render (t_point point, t_dim size, SDL_Rect * clip,
                         SDL_RendererFlip flip, double angle, t_point * center)
  {
    //Set rendering space and render to screen
    SDL_Rect renderQuad =
      { point.x, point.y, size.x ? size.x : image_size.x,
          size.y ? size.y : image_size.y };

    if (clip)
      {
        clip->w = clip->w ? clip->w : image_size.x;
        clip->h = clip->h ? clip->h : image_size.y;
      }

    //Render to screen
    SDL_RenderCopyEx (renderer, mTexture, clip, &renderQuad, angle, center,
                      flip);
  }

  t_collision Drawable::getCollisionEffect (t_itemtype type,
                                            t_direction dir) const
  {
    return COLLISION_IGNORE;
  }

  std::map<std::string, graphicInfo> Drawable::cachedSurfaces;

  void Drawable::cleanCache (void)
  {
    for (map<string, graphicInfo>::iterator it = cachedSurfaces.begin ();
        it != cachedSurfaces.end (); ++it)
      {
        SDL_FreeSurface (it->second.surface);
        SDL_DestroyTexture (it->second.texture);
      }
    cachedSurfaces.clear ();
  }

} /* namespace fwsdl */
