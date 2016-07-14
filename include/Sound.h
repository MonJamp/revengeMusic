#ifndef SOUND_H
#define SOUND_H

#include <fmod.hpp>

class Sound{

    public:
        
        Sound();
        ~Sound();
        
        bool init();

        bool isPlaying();

        void play();
        void play(const char* track_dir);
        void pause();
        
    private:
    
        FMOD::Channel *channel;
        FMOD::Sound *newSound;
        FMOD::System *m_pSystem;
};

#endif
