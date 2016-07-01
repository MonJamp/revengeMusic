#include "Sound.h"

Sound::Sound(){

  if( FMOD::System_Create( &m_pSystem) != FMOD_OK){

    std::cout<< "Could not create sound system." <<std::endl;
    return;
  }

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
void Sound::createSound( const char *pFile) {

  m_pSystem->createStream( pFile, FMOD_DEFAULT, 0, &audio);
}

void Sound::playSound( bool bLoop){

      if ( !bLoop)
         audio->setMode( FMOD_LOOP_OFF);
      else{

         audio->setMode( FMOD_LOOP_NORMAL);
         audio->setLoopCount( -1);
      }

      playing = m_pSystem->playSound( audio, 0, false, &channel);
   }

bool Sound::isPlaying(){

  //polls the channel to check if music has stopped
  channel->isPlaying( &playing);

  return playing;
}

void Sound::releaseSound(){

  audio->release();
}
