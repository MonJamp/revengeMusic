#ifndef IPC_H
#define IPC_H

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>

#define MAX_BUF 1024

class IPC{

  public:

    //named pipe
    const char* fifo = "/tmp/fifo";

    //quit flag
    bool quit = false;

    //file descriptor
    int fd;

    //creating a large char array with nothing on it to recieve messages on
    char message[MAX_BUF] = {""};

    //this can determine if the program is the original process or rinning a message
    //to the original process
    int pidfile;

    //returns 0 if the process isnt 'flock'ed
    int rc;

    //returns true if the program is the original process
    bool IPCOriginal();

    //sends a message to the original process
    void IPCSend( char[]);

    //listens for an incomming message and returns it
    std::string IPCGet();
};

#endif
