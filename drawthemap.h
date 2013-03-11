#ifndef DRAWTHEMAP_H_INCLUDED
#define DRAWTHEMAP_H_INCLUDED

#include <SDL/SDL.h>

#include "params.h"
#include "maskgen.h"
#include "sprites_struct.h"

/* Returns a surface with resulting BnA map.
   SDL should be initialized first. */
SDL_Surface * genCreateBnAMap(t_genParams * params, t_genSprites * sprites,
                              T_MASK_PIXEL * mask);

#endif // DRAWTHEMAP_H_INCLUDED
