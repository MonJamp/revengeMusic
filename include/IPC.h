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

    const char* fifo; //named pipe
    bool quit;

    //Used to receive messages
    char message[MAX_BUF];

    //this can determine if the program is the only instance
    int pidfile;

    //returns true if the program is the original process
    bool IPCOriginal();

    //sends a message to the original process
    void IPCSend( const char[]);

    //listens for an incomming message and returns it
    std::string IPCGet();

    //shuts down the IPC functionality, this way fifos and pid files dont stay open
    void IPCClose();
};

#endif
