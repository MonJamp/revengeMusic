#ifndef SOUND_H
#define SOUND_H

#include <fmod.hpp>

class Sound{

    public:
        
        Sound();
        ~Sound();
        
        bool init();
        void createSound(const char* track_dir);

        bool isPlaying();

        void play();
        void pause();
        
    private:
    
        FMOD::Channel *channel;
        FMOD::Sound *newSound;
        FMOD::System *m_pSystem;
};

#endif
