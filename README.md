# revengeMusic
A command line music program for linux, presently its just a prototype so I can practice system programming; the idea is 
to make it work well with i3 window manager how ever it may go on to have a gui like rofi.

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
