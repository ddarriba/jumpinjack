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
      unsigned long loadMusic (const std::string & path);
      void playSound(unsigned int sound_id, int loops = 0);
      void playMusic(unsigned int sound_id, int loops = -1);
      void setMusicVolume(int volume);
      void stopMusic();
      void cleanCache (void);
    private:
      bool audio_ok;
      unsigned long next_sound_id;
      std::map<unsigned long, Mix_Chunk *> cachedSounds;
      std::map<unsigned long, Mix_Music *> cachedMusic;
  };

} /* namespace jumpinjack */

#endif /* SOUND_MANAGER_H_ */
