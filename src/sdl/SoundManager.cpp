#include "SoundManager.h"

using namespace std;

namespace jumpinjack
{

SoundManager::SoundManager ()
{
  /* first assigned will be number 1 */
  next_sound_id = 0;

  //Initialize SDL_mixer
  int frequency = 22050;
  Uint16 format = MIX_DEFAULT_FORMAT;
  int channels  = 2;
  int chunksize = 4096;
  if( Mix_OpenAudio( frequency, format, channels, chunksize ) == -1 )
  {
      printf("ERROR INITIALIZING SOUND!\n");
  }
}

SoundManager::~SoundManager ()
{
  cleanCache();
  Mix_CloseAudio();
}

unsigned long SoundManager::loadFromFile (const string & path)
{
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

void SoundManager::playSound(unsigned int sound_id)
{
  if( Mix_PlayChannel( -1, cachedSounds[sound_id], 0 ) == -1 )
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
}

}
