#include <IPC.h>
#include <cstring>

IPC::IPC( std::string _location){

  //set fifo to home directory and check if there is allready a
  //fifo type file there
  fifo = _location.c_str();

  const char* pid_dir = "/tmp/revengeMusic.pid";

  //Creates and/or Locks the pid file
  int pid_fd = open(pid_dir, O_CREAT | O_RDONLY);
  int lock = flock(pid_fd, LOCK_EX | LOCK_NB);
  if(lock == 0) {
    onlyInstance = true;
  } else {
    onlyInstance = false;
  }
  close(pid_fd);

  mkfifo( fifo, S_IWUSR | S_IRUSR |S_IRGRP | S_IROTH);

  quit = false;
  
  std::memset(message, ' ', MAX_BUF);
}

bool IPC::isOnlyInstance(){
  return onlyInstance;
}

//sends a message to the original process
void IPC::IPCSend( const char buf[]){

     int fd = open( fifo, O_WRONLY);
     write( fd, buf, MAX_BUF);
     close( fd);
}

std::string IPC::IPCGet(){
    
      int fd = open( fifo, O_RDONLY);
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
}
