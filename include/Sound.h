#ifndef SOUND_H
#define SOUND_H

#include <string>
#include <iostream>

#include <fmod.hpp>
#include "fmod_errors.h"

class Sound{

public:

  bool playing;

  FMOD_RESULT result;

  FMOD::Channel *channel = NULL;

  FMOD::Sound *audio;

  FMOD::System *m_pSystem;

  Sound();

  void createSound( const char*);

  void playSound( bool);

  bool isPlaying();

  void releaseSound();
};

#endif
