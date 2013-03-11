#ifndef MASKGEN_H_INCLUDED
#define MASKGEN_H_INCLUDED

#include "params.h"
#include <SDL/SDL.h>
//#include "sprites_struct.h"

/* Creates the dot mask. */

#define T_MASK_PIXEL Uint32

T_MASK_PIXEL * genCreatMask(t_genParams * params);

#endif // MASKGEN_H_INCLUDED
