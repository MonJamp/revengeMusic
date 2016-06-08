#ifndef SOUND_H
#define SOUND_H

#include <string>
#include <iostream>

//FMOD sound API headers
#include <fmod.hpp>
#include "fmod_errors.h"

class Sound{

public:

  //audio object
  FMOD::Sound *audio;

  // Create FMOD interface object
  FMOD::System *m_pSystem;

  Sound();

  void createSound( const char*);

  void playSound( bool);

  void releaseSound();
};

#endif
