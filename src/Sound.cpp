#include "Sound.h"

#include <fmod.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h>

using namespace boost;

Sound::Sound(const char* music_dir) {
    
    channel = nullptr;
    newSound = nullptr;
    m_pSystem = nullptr;
    
    srand(time(NULL));
    getFileList(music_dir);
}

Sound::~Sound() {
    
    channel->stop();
    newSound->release();
    m_pSystem->release();
}

void Sound::getFileList(const char* music_dir) {
    
    filesystem::path dir(music_dir);

    try {
        if (filesystem::exists(dir)) {
            if(filesystem::is_regular_file(dir)) {
                
                dir = dir.parent_path();
            }
            
            if(filesystem::is_directory(dir)) {
                for (filesystem::directory_entry& x : filesystem::directory_iterator(dir)) {
                    if(x.path().extension() == ".mp3") {
                        
                        filelist.push_back(x.path().string());
                    }
                }
            } else {
                std::cout << dir << " is not supported or corrupt" << std::endl;
            }
        } else {
            
            std::cout << dir << " does not exist\n";
        }
    }
    catch (const filesystem::filesystem_error& error) {
        std::cout << error.what() << '\n';
    }
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
    
    playedFiles.push_back(pFile);
}

void Sound::pause() {
    
    channel->setPaused(true);
}

void Sound::play_next() {
    
    std::string nextSong;
    bool accept_song;
    std::vector<std::string>::iterator it;
    
    if(filelist.size() == playedFiles.size()) {
        playedFiles.clear();
    }
    
    do {
        accept_song = true;
        nextSong = filelist[rand() % filelist.size()];
        for(it = playedFiles.begin(); it < playedFiles.end(); it++) {
            if(*it == nextSong) {
                accept_song = false;
            }
        }
    } while(accept_song == false);
    
    filesystem::path track_dir(nextSong);
    std::cout << "Playing: " << track_dir.filename() << std::endl;
    play(nextSong.c_str());
}

void Sound::play_prev() {
    
    std::string nextSong;
    if(playedFiles.size() > 1) {
        nextSong = playedFiles[playedFiles.size() - 2];
        //TODO: Is there a "better" way to do this?
        std::string currentSong = playedFiles[playedFiles.size() - 1];
        playedFiles.pop_back();
        playedFiles.pop_back();
        playedFiles.push_back(currentSong);
    } else {
        nextSong = playedFiles[0];
        playedFiles.pop_back();
    }
    
    filesystem::path track_dir(nextSong);
    std::cout << "Playing: " << track_dir.filename() << std::endl;
    play(nextSong.c_str());
}
