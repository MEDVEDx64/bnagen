#ifndef SPRITES_STRUCT_H_INCLUDED
#define SPRITES_STRUCT_H_INCLUDED

#ifndef MAX_SPRITES
#   define MAX_SPRITES 0x100
#endif

#include <SDL/SDL.h>

typedef
struct {
    SDL_Surface * surf[MAX_SPRITES];
    unsigned int count;
}
t_genSprites;

#endif // SPRITES_STRUCT_H_INCLUDED
