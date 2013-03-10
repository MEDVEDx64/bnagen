#include "drawthemap.h"
#include "state.h"

void blit(SDL_Surface * dest, SDL_Surface * src, int x, int y)
{
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = src->w;
    r.h = src->h;

    SDL_BlitSurface(src, NULL, dest, &r);
}

SDL_Surface * mkSurf(int w, int h)
{
    /* copypaste from sdl.beuc.net, lol */
    Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    return SDL_CreateRGBSurface(0, w, h, 32,
                                rmask, gmask, bmask, amask);
}

#define COORD2ADDR(x_,y_) (y_*params->width+x_)

SDL_Surface * genCreateBnAMap(t_genParams * params, t_genSprites * sprites,
                              unsigned char * mask){
    /// Checking for SDL
    if(!SDL_WasInit(0))
        return NULL;

    progbar_max = params->width;
    genState = GEN_WORKING;

    SDL_Surface * themap = mkSurf(params->width, params->height);

    int x,y;
    int test = 0;
    for(x = 0; x < params->width; ++x)
    {
        progbar = x;

        for(y = 0; y < params->height; ++y)
        {
            if(mask[COORD2ADDR(x,y)] <= sprites->count && mask[COORD2ADDR(x,y)])
                /// blit(themap, sprites->surf[mask[COORD2ADDR(x,y)] - 1], x, y);
                test++;
        }

    }
    printf("%d\n", test);

    return themap;
}
