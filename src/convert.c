#include "convert.h"
#include "pixel.h"
#include "state.h"
#include "logging.h"

#include <malloc.h>

#define compare_color(x,y) ( abs((x)->r - (y)->r) + abs((x)->g - (y)->g) + abs((x)->b - (y)->b) )

static inline Uint8 genGetNearestColor(Uint32 src_pixel, SDL_Palette * palette)
{
    /** alexx's code **/
    if (src_pixel == 0x00000000) return 0; // black color caching

    register Uint8 i;
    Uint8 result = 1;
    static SDL_Color src;
    RGBA_TO_SDL_COLOR(src,src_pixel);

    for (i = 2; i < palette->ncolors; i++)
        if (compare_color(&src, &(palette->colors[result])) >  compare_color(&src, &(palette->colors[i]))) result = i;

    return result;
}

#ifndef SCAN_ATTEMPTS_MULTIPLIER
#   define SCAN_ATTEMPTS_MULTIPLIER 3
#endif // SCAN_ATTEMPTS_MULTIPLIER

SDL_Palette genScanSurface(SDL_Surface * surf)
{
    int max_attempts = surf->w * SCAN_ATTEMPTS_MULTIPLIER;
    SDL_Palette pal;

    int i = 1;
    int z = 0;
    pal.colors = calloc(sizeof(SDL_Color), (unsigned char)-1);
    pal.ncolors = 1;

    // black color by default
    pal.colors[0].r = 0;
    pal.colors[0].g = 0;
    pal.colors[0].b = 0;

    while(i++ < max_attempts && pal.ncolors <= MAX_PALETTE_COLORS)
    {
        // Getting a random pixel from input surface
        SDL_Color col;
        unsigned int c = getPixel32(surf, (int)rand()%surf->w, (int)rand()%surf->h);
        RGBA_TO_SDL_COLOR(col,c);

        for (z = 0; z < pal.ncolors; z++) {
            if (col.r == pal.colors[z].r && col.g == pal.colors[z].g && col.b == pal.colors[z].b) break;
        }

        if (z < pal.ncolors) continue; //pixel exist!

        pal.colors[pal.ncolors++] = col;
    }

    return pal;
}

SDL_Surface * genCreatePalettizedSurface(SDL_Surface * source, SDL_Palette * palette)
{
    if(palette->ncolors > (Uint8)-1){
        char buf[0x500];
        sprintf(buf, "%s: alert: abnormal palette colors count detected (%d). "
               "Fixing it, but proceeding may result in fault!\n",
               __FUNCTION__, palette->ncolors);
        genPrint(buf);
        palette->ncolors = MAX_PALETTE_COLORS;
    }

    // Creating 8-bit blank surface
    SDL_Surface * result = SDL_CreateRGBSurface(0, source->w, source->h, 8, 0, 0, 0, 0);
    SDL_SetPalette(result, SDL_LOGPAL|SDL_PHYSPAL, palette->colors, 0, palette->ncolors);

    if(!result)
        return NULL;

    // Filling it with pixels
    register long i;
    register long z = 0;
    for(i = 0; i < source->w*source->h; ++i)
    {
        putPixel8s(result, i, genGetNearestColor(getPixel32s(source, i), palette));

        if(++z > source->h)
        {
            ++progbar;
            z = 0;
        }
    }

    return result;
}
