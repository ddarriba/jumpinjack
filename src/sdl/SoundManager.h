/*
 * fwDrawable.h
 *
 *  Created on: May 3, 2015
 *      Author: diego
 */

#ifndef SOUND_MANAGER_H_
#define SOUND_MANAGER_H_

#include <string>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "../GlobalDefs.h"

namespace jumpinjack
{

  class SoundManager
  {
    public:
      SoundManager ( void );
      virtual ~SoundManager ();

      unsigned long loadFromFile (const std::string & path);
      void playSound(unsigned int sound_id);
      void cleanCache (void);
    private:
      unsigned long next_sound_id;
      std::map<unsigned long, Mix_Chunk *> cachedSounds;
  };

} /* namespace jumpinjack */

#endif /* SOUND_MANAGER_H_ */
