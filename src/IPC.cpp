#include <IPC.h>

IPC::IPC( std::string _location){

  //set fifo to home directory and check if there is allready a
  //fifo type file there
  fifo = _location.c_str();

  std::string path = _location;

  path += ".revengeMusic.pid";

  pidfile = open( path.c_str(), O_CREAT | O_RDONLY);
}

//returns true if the program is the original process, also initialises
//the pid file so only one process runs
bool IPC::IPCOriginal(){

   rc = flock( pidfile, LOCK_EX | LOCK_NB);

   if( rc == 0){

     //makes a fifo file for communicating between processes
     mkfifo( fifo, 0666);

     //std::cout << "original" << std::endl;
     return true;
   }else{

     std::cout << "~";
     return false;
   }
}

//sends a message to the original process
void IPC::IPCSend( const char buf[]){

  if( rc){

   fd = open( fifo, O_WRONLY);
   write( fd, buf, sizeof(buf));
   close( fd);

 }else{

   std::cout << "cannot write to read only fifo." << std::endl;
 }
}

//listens for an incomming message and returns it
std::string IPC::IPCGet(){

    if( rc == 0){

      fd = open( fifo, O_RDONLY);
      read(fd, message, MAX_BUF);
      std::cout << message << std::endl;

      close( fd);

      if( message[0] != '\0'){

        return std::string( message);
      }
    }else{

      std::cout << "cannot read from write only fifo." << std::endl;
    }
}

//maje sure every thing is closed
void IPC::IPCClose(){

  close( pidfile);
}
