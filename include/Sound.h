#ifndef SOUND_H
#define SOUND_H

#include <string>
#include <iostream>

//FMOD sound API headers
#include <fmod.hpp>

typedef FMOD::Sound* SoundClass;

class Sound{

public:

  FMOD::System *m_pSystem;

  Sound();

  void createSound( SoundClass, const char* );

  void playSound( SoundClass, bool);

  void releaseSound( SoundClass);
};

#endif
