# revengeMusic

Written in C++

A command line music program for linux,
Designed not to play two tracks at once and to be easily used in shell programs, this way key shortcuts and option flags
could allow a user to find a creative uses, I intend to release a gui mode and sound file search and create a music program with
similar implementation to rofi.

best case scenario is to mod rofi to play music files

# Building
## Dependencies
  - [FMOD API](http://www.fmod.org/download/)
    - **For Linux** 
      - Install the 'low level' headers to `/usr/include/fmodex/`
      - Install the 'low level' libraries to `/usr/lib/`
    - **For Windows**
      - FMOD has it's own installer, nothing needs to be done manually 
  - [CMake](https://cmake.org/download/) (recommended)
  - [MinGW](http://www.mingw.org/wiki/getting_started) (recommended)

## Compiling
  - **For Linux**
    - Run `./build.sh`
  - **For Windows**
    - Make sure cmake and make are in PATH
    - Run `build.cmd`

## Usage
```
./revengeMusic (--commands | <path>)
    commands:
        -h, --help      Shows this message
```
