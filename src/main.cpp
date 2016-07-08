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

int main( int argc, char *argv[]){

  std::string track;
  std::string music_dir;
  const char* home_dir;
  bool running = true;

  std::string kill = "kill";
  
    //Get home directory of user
    home_dir = getenv("HOME");
    if(home_dir == NULL) {
        perror("Error: could not get home directory");
        return -1;
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

  //Checks if this the only instance
  if( argc == 2 && ipc.isOnlyInstance()){

    std::cout<< "Playing file -" + track <<std::endl;
    song.createSound( music_dir.c_str());
    song.playSound( false);

    std::cout<< "Program is running"<<std::endl;

    
  //might want to slow this down later too
  while( song.isPlaying() && running){

    //will listen for IPC calls and process them here
    if( ipc.GetMessage() == "kill" ) {
      std::cout << "~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
      running = false;
    }
    sleep( 10);
  }

    std::cout<< track + " stopped, closing." <<std::endl;
    delete(&ipc);
    return 0;

  //Send message then quit
  }else if( argc == 2 && !ipc.isOnlyInstance()){

    //sends the message that was in args
    ///FOR NOW SIMPLY KILLS THE PROGRAM
    ipc.SendMessage( kill.c_str());
    delete(&ipc);
    return 0;
  }else{

    //if no argument, or one that is not a recognised command
    //is passed to this instande and an original instance is allready running
    //then send the kill command to shut the original program down
    if( !ipc.isOnlyInstance()){

        ipc.SendMessage( kill.c_str());
    std::cout<< kill.c_str();

    //else the program was called with bad arguments or without any
    //displaying notes on working commands
    }else{

     //will output a list of commands when is completed
     std::cout<< "\n-Type \"revengeMusic -COMMAND\" eg:\n";
    }

    delete(&ipc);
    return 0;
  }
}
