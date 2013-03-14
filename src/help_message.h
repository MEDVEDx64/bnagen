#ifndef HELP_MESSAGE_H_INCLUDED
#define HELP_MESSAGE_H_INCLUDED

/** Help mesg to be shown once '-?' or '--help' args is appeared **/
/* warning: requires char *argv[] to be visible in your scope */

#define SHOWHELP printf("Bow and Arrows map generator for Worms Armageddon\n\
usage: %s [OPTIONS ...]\n\
            -w VALUE — output image width\n\
            -h VALUE — output image height\n\
            -i FILENAME — specify the sprite list filename\n\
            -s VALUE — seed\n\
            -I VALUE — intensity\n\
            -o FILENAME — output bitmap filename\n\
            -d VALUE — 'deadzone': distance between sprites\n\
            -r VALUE — should sprites be randomly rotated?\n\n\
All these keys are optional, the following defaults may be used instead:\n\
            width: %d\n\
            height: %d\n\
            sprite list file: %s\n\
            seed: %d (current UNIX-time will be used)\n\
            intensity: %d\n\
    (please note that actual intensity goes lower as it's value is greater!)\n\
            output bitmap: %s\n\
\nWhile deadzone isn't specified, an anlernative sprite-placing \
technique will be used. Deadzone is disabled by default (0 means it's disabled)\n", argv[0],\
            DEFAULT_W, DEFAULT_H, DEFAULT_FILENAME,\
            DEFAULT_SEED, DEFAULT_INTENSITY, DEFAULT_OUT_FN)

#endif // HELP_MESSAGE_H_INCLUDED
