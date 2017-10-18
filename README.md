# revengeMusic

(REVerseENGineerMUSIC) Written in C++

The aim of this project was to create a music program that would be safe to run from the command line on windows or
linux. The user should be able to play a single track, file, playlist with key shortcuts or write a GUI program of their
own to run it without running into the problems that other command line music programs run into when this is attempted.




# Building
## Dependencies
  - [FMOD API](http://www.fmod.org/download/)
    - **For Linux**
      - Install the 'low level' headers to `/usr/include/fmodex/`
      - Install the 'low level' libraries to `/usr/lib/`
    - **For Windows**
      - FMOD has it's own installer, nothing needs to be done manually
  - [CMake 3.4](https://cmake.org/download/) (recommended)

## Compiling
  - **For Linux**
    ``` bash
    mkdir -p build/
    cd build
    cmake -DCMAKE_BUILD_TYPE=Debug ..
    make
    ```
  - **For Windows**
    - Make sure cmake and msbuild is in PATH
    - Run `build.cmd`
    - Move a copy of fmod.dll from the FMOD install folder into `revengeMusic/build/Debug/`

# Usage
```
./revengeMusic (--commands | <path>)
    commands:
        -h, --help      Shows this message
        -v, --version   Show version number
        -subdir         Specify a specific folder within the Music directory
        kill            Exits revengeMusic
        play            Unpause song
        pause           Pause song
        next            Play next song, next song is decided based on whether shuffle toggled on/off
        prev            Play previous song
        shuffle         Toggles shuffle on/off
        loop-file       Loops the current song
```
