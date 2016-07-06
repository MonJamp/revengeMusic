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
  bool running = true;

  std::string kill = "kill";
  const char* cwd;

  //Gets the Home directory of the user
  if ((cwd = getenv("HOME")) != NULL && argv[1] != NULL)  {

    cwd = getpwuid(getuid())->pw_dir;
    track = argv[1];
    music_dir = cwd;
  } else {
      perror("error getting present working directory");
      return 0;
  }

  Sound song;

  IPC ipc("/tmp/fifo");

  //Checks if this the only instance
  if( argc == 2 && ipc.IPCOriginal()){

    music_dir += ( "/Music/" + track);

    std::cout<< "Playing file -" + track <<std::endl;
    song.createSound( music_dir.c_str());
    song.playSound( false);

    std::cout<< "Program is running"<<std::endl;

    
  //might want to slow this down later too
  while( song.isPlaying() && running){

    //will listen for IPC calls and process them here
    if( ipc.IPCGet() == "kill" ) {
      std::cout << "~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
      running = false;
    }
    sleep( 10);
  }

    std::cout<< track + " stopped, closing." <<std::endl;
    ipc.IPCClose();
    return 0;

  //Send message then quit
  }else if( argc == 2 && !ipc.IPCOriginal()){

    //sends the message that was in args
    ///FOR NOW SIMPLY KILLS THE PROGRAM
    ipc.IPCSend( kill.c_str());
    ipc.IPCClose();
    return 0;
  }else{

    //if no argument, or one that is not a recognised command
    //is passed to this instande and an original instance is allready running
    //then send the kill command to shut the original program down
    if( !ipc.IPCOriginal()){

        ipc.IPCSend( kill.c_str());
	std::cout<< kill.c_str();

    //else the program was called with bad arguments or without any
    //displaying notes on working commands
    }else{

     //will output a list of commands when is completed
     std::cout<< "\n-Type \"revengeMusic -COMMAND\" eg:\n";
    }

    ipc.IPCClose();
    return 0;
  }
}
