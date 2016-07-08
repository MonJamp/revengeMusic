#include <windows.h>
#include <cstring>
#include <string>
#include <iostream>

#include "IPC.h"

Pipe::Pipe(const char* fifo_dir)
{
  //set fifo to home directory and check if there is allready a
  //fifo type file there
  fifo = "\\\\.\\pipe\\revengeMusic";

  const char* mutex_name = "/tmp/revengeMusic.pid";

  mutexHandle = CreateMutex(
    NULL,
    TRUE,
    mutex_name);

    if(GetLastError() == ERROR_ALREADY_EXISTS)
    {
        onlyInstance = false;
        pipeHandle = NULL;
    }
    else
    {
        onlyInstance = true;
        pipeHandle = CreateNamedPipe(
          fifo,
          PIPE_ACCESS_INBOUND,
          PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
          PIPE_UNLIMITED_INSTANCES,
          MAX_BUF,
          MAX_BUF,
          0,
          NULL);
    }
}

Pipe::~Pipe() {
    std::cout << "Closing IPC" << std::endl;
    if(mutexHandle != NULL)
    {
      ReleaseMutex(mutexHandle);
      CloseHandle(mutexHandle);
    }
    if(pipeHandle != NULL)
    { CloseHandle(pipeHandle); }
}

bool Pipe::isOnlyInstance() {

    return onlyInstance;
}

//sends a message to the fifo
void Pipe::SendMessage(const char* msg) {
    bool ready = WaitNamedPipe(
      fifo,
      NMPWAIT_USE_DEFAULT_WAIT);

    if(ready)
    {
      pipeHandle = CreateFile(
        fifo,
        GENERIC_WRITE,
        FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | SECURITY_ANONYMOUS,
        NULL
      );

      if(pipeHandle == NULL)
      {
        std::cout << "Pipe not opened" << std::endl;
        return;
      }

      bool fSuccess = WriteFile(
        pipeHandle,
        msg,
        strlen(msg)+1*sizeof(char),
        NULL,
        NULL);

      CloseHandle(pipeHandle);
      pipeHandle = NULL;

      if(!fSuccess)
      {
        std::cout << "Last Error:" << GetLastError() << std::endl;
        std::cout << "WriteFile to pipe failed." << std::endl;
      }
      else{std::cout << "msg sent" << std::endl;}
    }
}

std::string Pipe::GetMessage() {
    std::memset(message, ' ', MAX_BUF);

    bool fSuccess = ConnectNamedPipe(pipeHandle,NULL);

    if(!fSuccess && (GetLastError() != ERROR_PIPE_CONNECTED))
    {
      std::cout << "Pipe failed to connect" << std::endl;
      return "";
    }

    unsigned long bytesread;

    fSuccess = ReadFile(
      pipeHandle,
      message,
      MAX_BUF,
      &bytesread,
      NULL
      );

    DisconnectNamedPipe(pipeHandle);

    if(fSuccess){
        return std::string(message);
    } else {
        std::cout << "Last Error:" << GetLastError() << std::endl;
        std::cout << "ReadFile from pipe failed." << std::endl;
        return "";
    }
}
