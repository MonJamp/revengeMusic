#include "Sound.h"

#include <fmod.hpp>

#include <iostream>

Sound::Sound() {
    
    channel = nullptr;
    newSound = nullptr;
    m_pSystem = nullptr;
}

Sound::~Sound() {
    
    channel->stop();
    newSound->release();
    m_pSystem->release();
}

bool Sound::init() {
    
    if(FMOD::System_Create(&m_pSystem) != FMOD_OK) {

        std::cout << "Could not create sound system." << std::endl;
        return false;
    }
    
    int driverCount = 0;
    m_pSystem->getNumDrivers(&driverCount);
    
    if(driverCount == 0) {
        
        std::cout << "No sound drivers found!" << std::endl;
        return false;
    }
    
    //system is initiallised with 10 channels for now
    m_pSystem->init(10, FMOD_INIT_NORMAL, NULL);
    
    return true;
}

bool Sound::isPlaying() {
    
    bool playing;
    channel->isPlaying(&playing);
    return playing;
}

void Sound::play() {
    
    FMOD::Sound *currentSound;
    channel->getCurrentSound(&currentSound);
    
    //Check if there's a new sound to be played
    //if there is, play that sound otherwise
    //unpause channel if it's paused
    if(currentSound != newSound) {
        
        m_pSystem->playSound(newSound, 0, false, &channel);
    } else {
        
        channel->setPaused(false);
    }
}

void Sound::play(const char* pFile) {
    
    newSound->release();
    m_pSystem->createStream(pFile, FMOD_DEFAULT, 0, &newSound);
    m_pSystem->playSound(newSound, 0, false, &channel);
}

void Sound::pause() {
    
    channel->setPaused(true);
}
