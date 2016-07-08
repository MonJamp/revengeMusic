#include <string>
#include <stdio.h>
#include <errno.h>
#include <iostream>
#include <cstring>
#include <stdlib.h>

#include <fmod.hpp>
#include "fmod_errors.h"

#include "Sound.h"
#include "IPC.h"

//standard unix headers, need this to get present working directory
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>

int main( int argc, char *argv[]) {

    std::string track;
    std::string music_dir;
    const char* home_dir;
    bool running = true;

    std::string kill = "kill";
  
    //Get home directory of user
    home_dir = getenv("HOME");
    if(home_dir == NULL) {
        //Get home directory if it is not defined in the environment variable
        home_dir = getpwuid(getuid())->pw_dir;
        if(home_dir == NULL) {
            perror("Error: could not get home directory");
            return -1;
        }
    }
    
    if(argc > 0 && argv[1] != NULL) {
        track = argv[1];
        music_dir = home_dir;
        music_dir += "/Music/";
        music_dir += track;
    } else {
        perror("Error: track name invalid");
        return -2;
    }

    Sound song;

    IPC ipc("/tmp/fifo");

    if(!ipc.isOnlyInstance() && argv[1] == NULL) {
        
        ipc.SendMessage("kill");
        return 0;
    } else if(!ipc.isOnlyInstance() && argv[1] != NULL) {
        
        ipc.SendMessage(argv[1]);
        return 0;
    } else if(ipc.isOnlyInstance()) {
        
        std::cout << "Playing file: " << track << std::endl;
        song.createSound(music_dir.c_str());
        song.playSound(false);
        
        std::string msg;
        
        while(song.isPlaying() && running) {
            //GetMessage() is blocking which might need to be changed
            msg = ipc.GetMessage();
            
            if(msg == "kill") {
                std::cout << "Killed!" << std::endl;
                running = false;
            }
        }
        
        std::cout << track << " stopped, closing." << std::endl;
        //Currently no need to delete
        //delete(&ipc);
        return 0;
    }
}
