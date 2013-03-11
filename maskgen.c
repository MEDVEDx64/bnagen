#include "maskgen.h"
#include "state.h"

#include <SDL/SDL.h>

void rnd_roll(int roll)
{
    // Let's roll the Wheel-o-Fortune
    int i = 0;
    while(i)
    {
        rand();
        --i;
    }

    // More random factor here!
    if(SDL_WasInit(0))
    {
        i = SDL_GetTicks();
        while(i)
        {
            rand();
            --i;
        }
    }
}

T_MASK_PIXEL * genCreatMask(t_genParams * params) {
    genState = GEN_MKMASK;
    rnd_roll(params->seed);

    if(!params->width || !params->height)
        return NULL;

    /// Creating the mask
    T_MASK_PIXEL * mask = malloc((params->width * params->height) * sizeof(T_MASK_PIXEL));
    if(mask == NULL)
    {
        printf("God, save the Queen!! Out of memory?!?\n");
        return NULL;
    }

    /// Filling that mask with random
    int i;
    for(i = 0; i < params->width*params->height; ++i) {
        mask[i] = (T_MASK_PIXEL)(rand()%params->intensity);
        /// Filtering
        //if(mask[i] > spr_info->count) mask[i] = 0;
    }

    return mask;
}
