#include "drawthemap.h"
#include "state.h"

#include <time.h>
#include <SDL/SDL_rotozoom.h>

#define rotateSprite(spr) rotozoomSurface(spr, (double)(rand()%360), 1, SMOOTHING_OFF)

// ported from TestStuff source code
Uint32 getPixel(SDL_Surface *surface, int x, int y) /* lazyfoo */
{
    if(x<0 || y<0 || surface == NULL)
        return 0;

    SDL_LockSurface(surface);
    Uint32 *pixels = (Uint32*)surface->pixels;
    SDL_UnlockSurface(surface);

    return pixels[(y*surface->w)+x];
}

/** To avoid sprites crossing at resulting BnA map **/
Uint32 isAreaIsFree(SDL_Surface * targ, SDL_Surface * sprite, int x, int y,
                          int deadzone)
{
    Uint32 result = 0;
    register long xs,ys;

    if(deadzone)
    {
        for(xs = -deadzone; xs < deadzone; ++xs)
            for(ys = -deadzone; ys < deadzone; ++ys)
                if(x+xs >= 0 && x+xs < targ->w && y+ys >= 0 && y+ys < targ->h)
                    if(getPixel(targ, x+xs, y+ys))
                    {
                        result++;
                        goto leave_point;
                    }
    }
    else
    {
        for(xs = 0; xs < sprite->w; ++xs)
            for(ys = 0; ys < sprite->h; ++ys)
                if(x+xs < targ->w-sprite->w-1 && y+ys < targ->h-sprite->h-1)
                    if(getPixel(targ, x+xs, y+ys))
                    {
                        result++;
                        goto leave_point;
                    }
    }

    leave_point:
    return !result;
}

int isSpriteIsOutOfBounds(SDL_Surface * targ, SDL_Surface * spr,
                          int x, int y)
{
    int result = 0;
    // Avoiding sprites partially leaving the map's boundaries
    result += x > targ->w-spr->w-1 ? 1 : 0;
    result += y > targ->h-spr->h-1 ? 1 : 0;

    return result;
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
    if(s == NULL)
    {
        printf("%s: error occured while attempting to allocate a space for the map: %s\n", __FUNCTION__, SDL_GetError());
        return NULL;
    }
    SDL_Surface * s_format = SDL_DisplayFormat(s);
    SDL_FreeSurface(s);
    return s_format;
}

SDL_Surface * genCreateBnAMap(t_genParams * params, t_genSprites * sprites) {
    /// Checking for SDL
    if(!SDL_WasInit(0))
        return NULL;

    // Randomizing
    srand(params->seed ? params->seed : time(0));

    genState = GEN_PREPARING;

    SDL_Surface * themap = mkSurf(params->width, params->height);
    if(themap == NULL) return NULL;

    progbar_max = params->width;
    genState = GEN_WORKING;

    int x,y;
    Uint32 rnd = 0;

    for(x = 0; x < params->width; ++x)
    {
        progbar = x;

        for(y = 0; y < params->height; ++y)
        {
            rnd = (Uint32)(rand()%params->intensity);

            if(params->rotate) // Should sprites be rotated?
            {
                if(rnd <= sprites->count && rnd)
                {
                    SDL_Surface * temp = rotateSprite(sprites->surf[rnd - 1]);
                    if(!isSpriteIsOutOfBounds(themap, temp, x, y) &&
                        isAreaIsFree(themap, temp, x, y, params->deadzone))
                            blit(themap, temp, x, y);
                    SDL_FreeSurface(temp);
                }
            }
            else
            {
                if(rnd <= sprites->count && rnd
                    && isAreaIsFree(themap, sprites->surf[rnd - 1], x, y, params->deadzone))
                        if(!isSpriteIsOutOfBounds(themap, sprites->surf[rnd - 1]
                                                 , x, y))
                            blit(themap, sprites->surf[rnd - 1], x, y);
            }
        }

    }

    return themap;
}
