/* Sprites loading */

#ifndef LOADER_H_INCLUDED
#define LOADER_H_INCLUDED

#include "sprites_struct.h"

/*
    Load sprite list from a text file.
    Each line is a name of a sprite graphic file.
    Will be taken a attempt to load them all.
*/
t_genSprites * genLoadSprites(const char * list_fn);

#endif // LOADER_H_INCLUDED
