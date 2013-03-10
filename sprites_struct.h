#ifndef SPRITES_STRUCT_H_INCLUDED
#define SPRITES_STRUCT_H_INCLUDED

#define SPR_MAX 0x20

#include <SDL/SDL.h>

typedef
struct {
    SDL_Surface * surf[SPR_MAX];
    unsigned int count;
}
t_genSprites;

#endif // SPRITES_STRUCT_H_INCLUDED
