#include <IPC.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <iostream>

IPC::IPC(const char* fifo_dir) {

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

IPC::~IPC() {
    close(pid_fd);
    std::cout << "Closing IPC" << std::endl;
}

bool IPC::isOnlyInstance() {

    return onlyInstance;
}

//sends a message to the fifo
void IPC::SendMessage(const char* msg) {
    mkfifo(fifo, 0666);
    int fd = open(fifo, O_WRONLY);
    write(fd, msg, MAX_BUF);
    close(fd);
}

std::string IPC::GetMessage() {
    
    int fd = open(fifo, O_RDONLY);
    read(fd, message, MAX_BUF);
    close(fd);

    if(message[0] != '\0'){
        return std::string(message);
    } else {
        std::cout << "cannot read from write only fifo." << std::endl;
        return "none";
    }
}
