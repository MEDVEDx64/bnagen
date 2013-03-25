#include "drawthemap.h"
#include "state.h"
#include "pixel.h"

#include <time.h>
#include <SDL/SDL_rotozoom.h>

// Sprite rotation macro
#define rotateSprite(spr) rotozoomSurface(spr, (double)(rand()%360) /* random fotation angle */, 1, SMOOTHING_OFF)

/** To avoid sprites crossing at resulting BnA map **/
Uint32 isAreaIsFree(SDL_Surface * targ, SDL_Surface * sprite, int x, int y,
                          int deadzone)
{
    Uint32 result = 0;
    register long xs,ys; // iterators

    if(deadzone) // if deadzone is set
    {
        for(xs = -deadzone; xs < deadzone; ++xs)    // Scanning (deadzone*2)x(deadzone*2) rectangle
                                                    // around x and y
            for(ys = -deadzone; ys < deadzone; ++ys)
                // Checking for getting out of bounds
                if(x+xs >= 0 && x+xs < targ->w && y+ys >= 0 && y+ys < targ->h)
                    if(getPixel(targ, x+xs, y+ys))
                    {
                        // Pixel color != black means that area isn't free
                        result++;
                        goto leave_point; // Leaving double cycle
                    }
    }
    else
    {
        for(xs = 0; xs < sprite->w; ++xs)  // Scanning the rect only belong to the sprite
            for(ys = 0; ys < sprite->h; ++ys)
                if(x+xs < targ->w-sprite->w-1 && y+ys < targ->h-sprite->h-1) // Acting bthe same, boundaries check
                    if(getPixel(targ, x+xs, y+ys))
                    {
                        result++;
                        goto leave_point;
                    }
    }

    leave_point:
    return !result;
}

/* Sometimes, sprites partially getting out of map,
   so we can prevent it. */
int isSpriteIsOutOfBounds(SDL_Surface * targ, SDL_Surface * spr,
                          int x, int y)
{
    int result = 0;

    result += x > targ->w-spr->w-1 ? 1 : 0;
    result += y > targ->h-spr->h-1 ? 1 : 0;

    return result;
}

// Drawing
void blit(SDL_Surface * dest, SDL_Surface * src, int x, int y)
{
    SDL_Rect r;
    r.x = x;
    r.y = y;
    //r.w = src->w;
    //r.h = src->h;

    SDL_BlitSurface(src, NULL, dest, &r);
}

// Create an empty space for the map
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

    // Converting it into a valid format
    SDL_Surface * s_format = SDL_DisplayFormat(s);
    // and destroying the original
    SDL_FreeSurface(s);
    return s_format;
}

SDL_Surface * genCreateBnAMap(t_genParams * params, t_genSprites * sprites) {
    /// Checking for SDL
    if(!SDL_WasInit(0))
        return NULL;

    // Randomizing
    srand(params->seed ? params->seed : time(0));

    /// Switching application's state here
    genState = GEN_PREPARING;

    SDL_Surface * themap = mkSurf(params->width, params->height);
    if(themap == NULL) return NULL;

    progbar_max = params->width * 2; // Progress bar's maximum should be equal to map's width
    genState = GEN_WORKING;

    int x,y;
    Uint32 rnd = 0; // A random number

    for(x = 0; x < params->width; ++x)
    {
        progbar = x; // Updating progressbar

        for(y = 0; y < params->height; ++y)
        {
            rnd = (Uint32)(rand()%params->intensity); // Rolling...

            if(params->rotate) // Should sprites be rotated?
            {
                /* To make an attempt to put a sprite, the random number
                   shouldn't be larger than sprites count.
                   This number decides which sprite from set will be
                   used this time.
                */
                if(rnd <= sprites->count && rnd)
                {
                    // Rotating the sprite and putting it into 'temp'
                    SDL_Surface * temp = rotateSprite(sprites->surf[rnd - 1] /* accessing the selected sprite */);
                    if(!isSpriteIsOutOfBounds(themap, temp, x, y) &&
                        isAreaIsFree(themap, temp, x, y, params->deadzone))
                            blit(themap, temp, x, y);
                    SDL_FreeSurface(temp); // Deleting the temporary
                }
            }
            else
            {
                if(rnd <= sprites->count && rnd // almost the same here but without sprite rotation
                    && !isSpriteIsOutOfBounds(themap, sprites->surf[rnd - 1], x, y))
                        if(isAreaIsFree(themap, sprites->surf[rnd - 1], x, y, params->deadzone))
                            blit(themap, sprites->surf[rnd - 1], x, y);
            }
        }

    }

    return themap;
}
