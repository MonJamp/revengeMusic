#include <Pipe.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <iostream>

Pipe::Pipe(const char* fifo_dir) {

    //set fifo to home directory and check if there is allready a
    //fifo type file there
    fifo = fifo_dir;

    const char* pid_dir = "/tmp/revengeMusic.pid";

    //Creates and/or Locks the pid file
    pid_fd = open(pid_dir, O_CREAT | O_RDONLY);
    int lock = flock(pid_fd, LOCK_EX | LOCK_NB);
    if(lock == 0) {
        onlyInstance = true;
    } else {
        onlyInstance = false;
    }
    //close(pid_fd);

    std::memset(message, ' ', MAX_BUF);
}

Pipe::~Pipe() {
    close(pid_fd);
    std::cout << "Closing pipe" << std::endl;
}

bool Pipe::isOnlyInstance() {

    return onlyInstance;
}

//sends a message to the fifo
void Pipe::SendMessage(const char* msg) {
    mkfifo(fifo, 0666);
    int fd = open(fifo, O_WRONLY);
    write(fd, msg, MAX_BUF);
    close(fd);
}

std::string Pipe::GetMessage() {
    
    
    fd_set set;
    struct timeval tv;
    
    int fd = open(fifo, O_RDONLY | O_NONBLOCK);
    
    FD_ZERO(&set);
    FD_SET(fd, &set);
    
    tv.tv_sec = 0;
    tv.tv_usec = 16666;
    
    //Check if anything is read to pipe for 16.666ms (1/60th second)
    int status = select(fd + 1, &set, NULL, NULL, &tv);

    if(status > 0) {
        read(fd, message, MAX_BUF);
        close(fd);
        return std::string(message);
    }
    else {
        close(fd);
        return "none";
    }
}
