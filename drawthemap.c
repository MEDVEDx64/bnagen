#include "drawthemap.h"
#include "state.h"

// ported from TestStuff source code
T_MASK_PIXEL getPixel(SDL_Surface *surface, int x, int y) /* lazyfoo */
{
    if(x<0 || y<0 || surface == NULL)
        return 0;

    SDL_LockSurface(surface);
    T_MASK_PIXEL *pixels = (T_MASK_PIXEL*)surface->pixels;
    SDL_UnlockSurface(surface);

    return pixels[(y*surface->w)+x];
}

/** To avoid sprites crossing at result BnA map **/
T_MASK_PIXEL isAreaIsFree(SDL_Surface * targ, SDL_Surface * sprite, int x, int y)
{
    T_MASK_PIXEL result = 0;
    int xs,ys;
    for(xs = 0; xs < sprite->w; ++xs)
        for(ys = 0; ys < sprite->h; ++ys)
            if(x+xs < targ->w-sprite->w-1 && y+ys < targ->h-sprite->h-1)
                result |= getPixel(targ, x+xs, y+ys);

    // Avoiding sprites partially leaving the map's boundaries
    result |= x > targ->w-sprite->w-1 ? 1 : 0;
    result |= y > targ->h-sprite->h-1 ? 1 : 0;

    return !result;
}

void blit(SDL_Surface * dest, SDL_Surface * src, int x, int y)
{
    SDL_Rect r;
    r.x = x;
    r.y = y;
    //r.w = src->w;
    //r.h = src->h;

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

    SDL_Surface * s = SDL_CreateRGBSurface(0, w, h, 32,
                                rmask, gmask, bmask, amask);
    SDL_Surface * s_format = SDL_DisplayFormat(s);
    SDL_FreeSurface(s);
    return s_format;
}

#define COORD2ADDR(x_,y_) (y_*params->width+x_)

SDL_Surface * genCreateBnAMap(t_genParams * params, t_genSprites * sprites,
                              T_MASK_PIXEL * mask){
    /// Checking for SDL
    if(!SDL_WasInit(0))
        return NULL;

    genState = GEN_PREPARING;

    SDL_Surface * themap = mkSurf(params->width, params->height);

    progbar_max = params->width;
    genState = GEN_WORKING;

    int x,y;
    for(x = 0; x < params->width; ++x)
    {
        progbar = x;

        for(y = 0; y < params->height; ++y)
        {
            if(mask[COORD2ADDR(x,y)] <= sprites->count && mask[COORD2ADDR(x,y)]
               && isAreaIsFree(themap, sprites->surf[mask[COORD2ADDR(x,y)] - 1], x, y))
                blit(themap, sprites->surf[mask[COORD2ADDR(x,y)] - 1], x, y);
        }

    }

    return themap;
}
