#include <windows.h>
#include <cstring>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#include "Pipe.h"
#include "Logger.h"

Pipe::Pipe(const char* fifo_dir):
  PIPE_TIMEOUT(16)
{
  //set fifo to home directory and check if there is allready a
  //fifo type file there
  fifo = "\\\\.\\pipe\\revengeMusic";

  const char* mutex_name = "/tmp/revengeMusic.pid";

  //Initialize OVERLAPPED structure for asycn. pipes
  pipe_info = {0,0,0,0,NULL};
  pipe_info.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

  if(pipe_info.hEvent == NULL)
  { std::cout << "Failed to CreateEvent for pipe_info." << std::endl; }

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
          PIPE_ACCESS_INBOUND | FILE_FLAG_FIRST_PIPE_INSTANCE | FILE_FLAG_OVERLAPPED,
          PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT,
          PIPE_UNLIMITED_INSTANCES,
          MAX_BUF,
          MAX_BUF,
          1000,
          NULL);

        if(pipeHandle == INVALID_HANDLE_VALUE || pipeHandle == NULL)
        { std::cout << "Invalid pipe handle! " << GetLastError() << std::endl; }
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
void Pipe::SendMessage(const char* msg) 
{
  bool ready = WaitNamedPipe(
    fifo,
    NMPWAIT_USE_DEFAULT_WAIT);

  if(ready)
  {
    pipeHandle = CreateFile(
      fifo,
      GENERIC_WRITE,
      FILE_SHARE_DELETE | FILE_SHARE_WRITE,
      NULL,
      OPEN_EXISTING,
      FILE_FLAG_OVERLAPPED | SECURITY_ANONYMOUS,
      NULL
    );

    if(pipeHandle == INVALID_HANDLE_VALUE || pipeHandle == NULL)
    {
      std::cout << "Pipe not opened" << std::endl;
      return;
    }

    bool fSuccess = WriteFile(
      pipeHandle,
      msg,
      (strlen(msg)+1)*sizeof(char),
      NULL,
      &pipe_info);

    if(!fSuccess)
    {
      switch(int err = GetLastError())
      {
        case ERROR_IO_PENDING:
          switch(WaitForSingleObject(pipe_info.hEvent, 1000))
          {
            case WAIT_OBJECT_0:
              //std::cout << "msg sent" << std::endl;
            break;

            case WAIT_TIMEOUT:
              Logger::PrintError("WriteFile to pipe failed.");
            break;

            case WAIT_FAILED:
              Logger::PrintError("WaitForSingleObject has failed.");
            break;
          }
        break;

        default:
          Logger::PrintError("Last Error:"+err);
        break;
      }
    }

    CloseHandle(pipeHandle);
    pipeHandle = NULL;
  }
  else
  {
    Logger::PrintError("Pipe not ready");
  }
}

std::string Pipe::GetMessage() 
{
  std::memset(message, '\0', sizeof(message));

  bool fSuccess;

  fSuccess = ConnectNamedPipe(pipeHandle,&pipe_info);

  if(fSuccess == false) 
  {
    switch(int err = GetLastError()) 
    {
      case ERROR_PIPE_CONNECTED:
        //Client is connected
        std::cout << ""; //This makes the GetMessage() work stably, somehow!
        fSuccess = true;
      break;
 
      case ERROR_IO_PENDING:
        //std::cout << "Pending!" << std::endl;
        //Wait PIPE_TIMEOUT_CONNECT number of seconds
        if(WaitForSingleObject(pipe_info.hEvent, PIPE_TIMEOUT) == WAIT_OBJECT_0) 
        { fSuccess = true; } 
        else 
        { CancelIo(pipeHandle); }
      break;

      default:
      break;
    }
  }

  if(fSuccess == false)
  { return ""; }

  DWORD bytesread;

  fSuccess = ReadFile(
    pipeHandle,
    message,
    MAX_BUF,
    &bytesread,
    &pipe_info
    );

  switch(WaitForSingleObject(pipe_info.hEvent, PIPE_TIMEOUT))
  {
      case WAIT_OBJECT_0:
        //std::cout << "msg sent" << std::endl;
      break;

      case WAIT_TIMEOUT:
        Logger::PrintError("GetMessage() timed out.");
      break;

      case WAIT_FAILED:
        Logger::PrintError("WaitForSingleObject has failed.");
      break;
  }
  //std::cout << "msg:" << std::string(message) << std::endl;

  DisconnectNamedPipe(pipeHandle);

  return std::string(message);

  //std::cout << "Last Error:" << GetLastError() << std::endl;
  //std::cout << "ReadFile from pipe failed." << std::endl;
}
