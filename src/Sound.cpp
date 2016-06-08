#include "Sound.h"

//constructor for sound class initialises the modules
Sound::Sound(){

  if( FMOD::System_Create( &m_pSystem) != FMOD_OK){

    std::cout<< "Could not create sound system." <<std::endl;
    return;
  }

  //gets all drivers for error checking
  int driverCount = 0;
  m_pSystem->getNumDrivers( &driverCount);

  if( driverCount == 0){

    std::cout<< "no sound driver was found" <<std::endl;
    return;
  }

  //system is initiallised with 10 channels for now
  m_pSystem->init( 10, FMOD_INIT_NORMAL, NULL);
}

//loads sound in path to memory
void Sound::createSound( SoundClass *psound, const char* pFile) {

  m_pSystem->createSound( pFile, FMOD_HARDWARE, 0, psound);
}

void Sound::playSound( SoundClass pSound, bool bLoop = false){

      if ( !bLoop)
         pSound->setMode(FMOD_LOOP_OFF);
      else{

         pSound->setMode(FMOD_LOOP_NORMAL);
         pSound->setLoopCount(-1);
      }

      m_pSystem->playSound( FMOD_CHANNEL_FREE, pSound, false, 0);
   }

   void Sound::releaseSound(SoundClass pSound){
      pSound->release();
   }
