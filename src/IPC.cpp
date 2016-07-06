#include <IPC.h>
#include <cstring>

IPC::IPC(std::string fifo_dir) {

    //set fifo to home directory and check if there is allready a
    //fifo type file there
    fifo = fifo_dir;

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

IPC::~IPC() {

    std::cout << "Closing IPC" << std::endl;
}

bool IPC::isOnlyInstance() {

    return onlyInstance;
}

//sends a message to the fifo
void IPC::SendMessage(const char* msg) {

     int fd = open(fifo, O_WRONLY);
     write(fd, buf, MAX_BUF);
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
    }
}
