#ifndef IPC_H
#define IPC_H

#include <fcntl.h>
#include <pwd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <errno.h>

#define MAX_BUF 1024

class IPC{

  public:

    //constructor sets home directory for fifo
    IPC( std::string);
    ~IPC();

    const char* fifo; //named pipe
    bool quit;

    //Used to receive messages
    char message[MAX_BUF];

    //returns true if there is only one instance
    bool isOnlyInstance();

    //sends a message to the original process
    void IPCSend( const char[]);

    //listens for an incomming message and returns it
    std::string IPCGet();

  private:
    bool onlyInstance;
};

#endif
