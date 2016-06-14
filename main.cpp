#include <string>
#include <stdio.h>
#include <errno.h>
#include <iostream>
#include <cstring>

//FMOD sound API headers
#include <fmod.hpp>
#include "fmod_errors.h"

//sound and IPC class include
#include "Sound.h"
#include "IPC.h"

//standard unix headers, need this to get present working directory
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>

int main( int argc, char *argv[]){

  //stores the track to play
  std::string track;

  //this string will make up the path to "/Music" directory
  std::string PATH;

  //flag that is required to be true for the program to not exit immediatley
  bool running = true;

  //the kill command, is the signal to shut the running program down
  //an improved IPC class will be a lot tidier
  std::string kill = "kill";

  //this block gets the present working directory and builds the path to
  //the track
  const char* cwd;

  //this block gets the directory the user is using for the program
  if ((cwd = getenv("HOME")) == NULL)  {

    cwd = getpwuid(getuid())->pw_dir;
    track = argv[1];
    PATH = cwd;

  //returns an error and quits program
  }else if( cwd != NULL){

    track = argv[1];
    PATH = cwd;

  }else{

      perror("error getting present working directory");
      return 0;
  }

  //create sound object and initialise it to PATH
  Sound song;

  //create and initialise IPC object
  IPC ipc( PATH + "/tmp/fifo");

  //checks if args have been put in correctly and this instance is the music
  //program or just passing the music a message
  if( argc == 2 && ipc.IPCOriginal()){

    //the path was needed for the IPC object but the path now needs the track
    //to be appended to the file to create a path to the music
    PATH += ( "/Music/" + track);

    //create then play music after outputting message
    std::cout<< "Playing file -" + track <<std::endl;
    song.createSound( PATH.c_str());
    song.playSound( false);

  //while music is not over, might want to slow this down later too
  while( song.isPlaying() && running){

    //will listen for IPC calls and process them here
    if( ipc.IPCGet() == "kill" ) {

      //output something so I know it got the message
      std::cout << "~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
      running = false;
    }

        //if this thread is to simply sit and spin then it had better not do too
        //mutch too often, this function sleeps for 1 second
        sleep( 1);
    }

    std::cout<< track + " stopped, closing." <<std::endl;
    ipc.IPCClose();
    return 0;

  //in this case the instance of  the program is just going to send a message
  //and then exit
  }else if( argc == 2 && !ipc.IPCOriginal()){

    //sends the message that was in args and exits the program
    ///FOR NOW SIMPLY KILLS THE PROGRAM
    ipc.IPCSend( kill.c_str());
    ipc.IPCClose();
    return 0;
  }else{

    //if no argument, or one that is not a recognised command
    //is passed to this instande and an original instance is allready running
    //then send the kill command to shut the ptogram down
    if( !ipc.IPCOriginal()){

        ipc.IPCSend( kill.c_str());

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
