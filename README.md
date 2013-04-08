*bnagen* is a Bow-n-Arrow map generator for Worms Armageddon.

Installation
============

First, clone this repository:
`git clone --recursive git://github.com/MEDVEDx64/bnagen.git`

You have to compile *bnagen*. Make sure that you have the following libraries installed:
 - *libSDL* (1.2);
 - *libSDL_image* (1.2);
 - *libSDL_ttf* (2.0);
 - *libSDL_gfx* (1.2).

In most of Debian-like systems this all may be installed via *apt*:
`sudo apt-get install libsdl1.2-dev libsdl-image1.2-dev libsdl-ttf2.0-dev libsdl-gfx1.2-dev`

*bnagen* uses [SDL_imagesave](https://github.com/TheWatcher/SDL_imagesave), which also requires of *libpng-dev* to be installed on your system.

Next, you have to run `make` and `sudo make install`.

If you have any problems on build stage, try to compile it by hand:
`cd src && cc *.c -o ../bnagen -s -O2 -lSDL -lSDLmain -lSDL_image -lSDL_ttf -I/usr/include/SDL/ -I./src/SDL_imagesave/`
`cd .. && sudo make install`

Note that installation is optional — you can build *bnagen* by `make` and run it from current directory: `./bnagen`.
Also note that you can uninstall *bnagen* from your system by typing `sudo make uninstall`.

How to
======

*bnagen* creates a map according to your custom sprites list. First, you have to pick up some png, bmp, etc. sprites into yout working directory. Then you need to create a line-by-line list of all your sprites you want to be used in your map (`ls -1 > sprites.txt` may help you with it). And then you simply run *bnagen* — fresh-cooked `out.png` will appear.

*bnagen* is tunable with these command-line keys:
 - *-w VALUE* — output image width
 - *-h VALUE* — output image height
 - *-i FILENAME* — specify the sprite list filename (use '-' to read stdin)
 - *-s VALUE* — seed
 - *-I VALUE* — intensity of sprites
 - *-o FILENAME* — output image filename
 - *-d VALUE* — deadzone, distance between sprites
 - *-r VALUE* — should sprites be randomly rotated?
 - *-c VALUE* — max. palette colors

All these keys are optional, the following defaults may be used instead:
 - width: *1920*
 - height: *696*
 - sprite list file: *sprites.txt*
 - seed: *0* (current UNIX-time will be used)
 - intensity: *38600* (please note that actual intensity goes lower as it's value is greater!)
 - deadzone: *78*
 - output image: *out.png*
 - palette colors: *63*

While deadzone == 0, an anlernative sprite-placing technique will be used.

Tips
====

 - There are `bnagen-openwith.sh` script comes with `sudo make install`, so you can open list files using *bnagen-openwith.sh*.

License
=======

*bnagen* is available under the terms of GNU General Public License v.2.0.
