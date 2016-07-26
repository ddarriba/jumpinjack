#include "SoundManager.h"

using namespace std;

namespace jumpinjack
{

SoundManager::SoundManager ()
{
  /* first assigned will be number 1 */
  next_sound_id = 0;
  audio_ok = true;

  //Initialize SDL_mixer
  int frequency = 44100;
  Uint16 format = MIX_DEFAULT_FORMAT;
  int channels  = 2;
  int chunksize = 2048;

  if( Mix_OpenAudio( frequency, format, channels, chunksize ) == -1 )
  {
    audio_ok = false;
    cerr << "Error initializing sound: " << Mix_GetError() << endl;
    cerr << "Sound disabled" << endl;
  }
}

SoundManager::~SoundManager ()
{
  cleanCache();
  Mix_CloseAudio();
}

unsigned long SoundManager::loadFromFile (const string & path)
{
  if (!audio_ok)
    return 0;

  ++next_sound_id;
  Mix_Chunk * new_sound = Mix_LoadWAV( path.c_str() );
  if (new_sound == NULL)
  {
    cerr << "ERROR LOADING SOUND " << path << endl;
    return 0;
  }

  cachedSounds[next_sound_id] = new_sound;

  return next_sound_id;
}

unsigned long SoundManager::loadMusic (const string & path)
{
  if (!audio_ok)
    return 0;

  ++next_sound_id;
  Mix_Music * new_music = Mix_LoadMUS ( path.c_str() );
  if (new_music == NULL)
  {
    cerr << "ERROR LOADING MUSIC " << path << endl;
    return 0;
  }

  cachedMusic[next_sound_id] = new_music;

  return next_sound_id;
}

void SoundManager::playMusic(unsigned int sound_id, int loops)
{
  if( audio_ok && Mix_PlayMusic(cachedMusic[sound_id], loops) == -1 )
  {
    printf("ERROR PLAYING MUSIC\n");
  }
}

void SoundManager::setMusicVolume(int volume)
{
  if (!audio_ok)
    return;
  Mix_VolumeMusic(volume);
}

void SoundManager::stopMusic()
{
  if (!audio_ok)
    return;
  Mix_HaltMusic();
}

void SoundManager::playSound(unsigned int sound_id, int loops)
{
  if( audio_ok && Mix_PlayChannel( -1, cachedSounds[sound_id], loops ) == -1 )
  {
    printf("ERROR PLAYING SOUND\n");
  }
}

void SoundManager::cleanCache (void)
{
  for (map<unsigned long, Mix_Chunk *>::iterator it = cachedSounds.begin ();
       it != cachedSounds.end (); ++it)
  {
    Mix_FreeChunk( it->second );
  }
  cachedSounds.clear();

  for (map<unsigned long, Mix_Music *>::iterator it = cachedMusic.begin ();
       it != cachedMusic.end (); ++it)
  {
    Mix_FreeMusic( it->second );
  }
  cachedMusic.clear();
}

}
