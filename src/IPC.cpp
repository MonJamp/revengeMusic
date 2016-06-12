#include <IPC.h>

//returns true if the program is the original process
bool IPC::IPCOriginal(){

  char buf[MAX_BUF] = {""};

  pidfile = open("revengeMusic.pid", O_CREAT | O_RDONLY);

  rc = flock(pidfile, LOCK_EX | LOCK_NB);

  if( rc != 0){

    return false;
  }else{

    return true;
  }
}

//sends a message to the original process
void IPC::IPCSend( char buf[]){

  mkfifo( fifo, 0666);

   fd = open( fifo, O_WRONLY);
   write( fd, buf, sizeof(buf));
   close( fd);
}

//listens for an incomming message and returns it
std::string IPC::IPCGet(){

  fd = open( fifo, O_RDONLY);
  read( fd, message, MAX_BUF);
  close( fd);

  if(message[0] != '\0')
    return std::string( message);
}
