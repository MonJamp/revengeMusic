#include <string>
#include <stdio.h>
#include <errno.h>
#include <iostream>
#include <cstring>

//FMOD sound API headers
#include <fmod.hpp>
#include "fmod_errors.h"

//sound class include
#include "Sound.h"

//standard unix headers, need this to get present working directory
#include <unistd.h>

int main( int argc, char *argv[]){

  //stores the track to play
  std::string track;

  //this string will make up the path to "/Music" directory
  std::string PATH;

  //create sound object
  Sound song;

  //checks if args have been put in correctly
  if( argc == 2){

    //this block gets the present working directory and builds the path to
    //the track
    char cwd[1024];
    if ( getcwd(cwd, sizeof(cwd)) != NULL){

      track = argv[1];
      PATH = cwd;
      PATH += "/../../Music/";
      PATH += track;

    //returns an error and quits program
    }else{

       perror("error getting present working directory");
       return -1;
    }

    //create then play music after outputting message
    std::cout<< "Playing file -" + track <<std::endl;
    song.createSound( PATH.c_str());
    song.playSound( false);

    //while music is not over, might want to slow this down later too
    while( song.isPlaying()){

        //will be implementing IPC here

        //if this thread is to simply sit and spin then it had better not do too
        //mutch too often, this function sleeps for 1 second
        sleep( 1);
    }

    std::cout<< track + " finished, closing." <<std::endl;
    return 0;
   }else{

     //will output a list of commands when is completed
     std::cout<< "\n-Type \"revengeMusic -COMMAND\" eg:\n";
     return 0;
  }
}
