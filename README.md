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
    - You need to register before downloading
    - Install the low level apis to /usr/lib
    - Extract FMOD to the project root directory

## Compiling
Make sure the FMOD libraries are included and libraries are linked in `build.sh`. Then run `./build.sh`

## Usage
```
./revengeMusic (--commands | <path>)
    commands:
        -h, --help      Shows this message
```


