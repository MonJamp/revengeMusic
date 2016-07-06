#include <IPC.h>
#include <cstring>

IPC::IPC( std::string _location){

  //set fifo to home directory and check if there is allready a
  //fifo type file there
  fifo = _location.c_str();

  std::string path = _location;

  path += "revengeMusic.pid";

  pidfile = open( path.c_str(), O_CREAT | O_RDONLY);

  mkfifo( fifo, S_IWUSR | S_IRUSR |S_IRGRP | S_IROTH);

  quit = false;
  
  std::memset(message, ' ', MAX_BUF);
}

//returns true if there is no other instance, also locks
//the pid file to detect if there is already an instance running
bool IPC::IPCOriginal(){

   rc = flock( pidfile, LOCK_EX | LOCK_NB);

   if( rc == 0){

     //std::cout << "original" << std::endl;
     return true;
   }else{

     //std::cout << "secondary" << std::endl;
     return false;
   }
}

//sends a message to the original process
void IPC::IPCSend( const char buf[]){

  //if( rc){

     fd = open( fifo, O_WRONLY);
     write( fd, buf, MAX_BUF);
     close( fd);

     // }else{

     //   std::cout << "cannot write to read only fifo." << std::endl;
     // }
}

std::string IPC::IPCGet(){

  // if( rc == 0){
    
      fd = open( fifo, O_RDONLY);
      read( fd, message, MAX_BUF);
      close( fd);
      
      if( message[0] != '\0'){

        return std::string( message);
	//   }else{

	//	std::cout<<"message buffer empty"<<std::endl;
	
	// }

    }else{

      std::cout << "cannot read from write only fifo." << std::endl;
    }
}

//maje sure every thing is closed
void IPC::IPCClose(){

  std::cout<<"Closing IPC"<<std::endl;
  close( pidfile);
}
