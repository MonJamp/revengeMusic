#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <string>
#include <stdio.h>
#include <errno.h>
#include <iostream>

//standard unix headers, need this to get present working directory
#include <unistd.h>

int main( int argc, char *argv[]){

 //checks if args have been put in correctly
 if( argc == 2){

    //this block gets the present working directory
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
       fprintf(stdout, "Current working dir: %s\n", cwd);

    //returns an error and quits program
    else{

       perror("error getting present working directory");
       return -1;
    }

    //these strings will make up the rest of the path to music
    std::string slash = "/";
    std::string arg = argv[ 1];

    //the music object
    sf::Music music;

    //if music at PATH is not found then exit program
    if(!music.openFromFile( cwd + slash + arg)){

      std::cout<< "File -" + arg + "- Not Found in " + cwd;
      return -1;
    }else{

      //play music after outputting message
      std::cout<< "Playing file -" + arg + "- @ " + cwd <<std::endl;
      music.play();

      ///TO-DO: switch to duration to stop music
      //while music is not over, might want to slow this down later too
      while(!sf::Music::Status::Stopped){

        //will be implementing IPC here

        //if this thread is to simply sit and spin then it had better not do too
        //mutch too often
        sf::sleep(sf::milliseconds(1000));
      }
      std::cout<< arg + "stopped" <<std::endl;
   }


    return 0;
   }else{

    std::cout<< "music app exiting now \n";
    return 0;
  }
}
