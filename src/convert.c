#include "convert.h"
#include "pixel.h"
#include "state.h"

#include <malloc.h>

static inline Uint8 genGetNearestColor(Uint32 src_pixel, SDL_Palette * palette)
{
    /** alexx's code **/

    register Uint8 i;
    static Uint8 result = 0;
    static SDL_Color src;
    RGBA_TO_SDL_COLOR(src,src_pixel);

    static unsigned int dif1 = 0;
    static unsigned int dif2 = 0;
    for (i = 1; i < palette->ncolors; i++) {
        dif1 = abs(src.r - palette->colors[result].r);
        dif1 += abs(src.g - palette->colors[result].g);
        dif1 += abs(src.b - palette->colors[result].b);

        dif2 = abs(src.r - palette->colors[i].r);
        dif2 += abs(src.g - palette->colors[i].g);
        dif2 += abs(src.b - palette->colors[i].b);

        if (dif1 > dif2) result = i;
    }

    return result;
}

#ifndef SCAN_ATTEMPTS_MULTIPLIER
#   define SCAN_ATTEMPTS_MULTIPLIER 2
#endif // SCAN_ATTEMPTS_MULTIPLIER

SDL_Palette genScanSurface(SDL_Surface * surf)
{
    int max_attempts = surf->w * SCAN_ATTEMPTS_MULTIPLIER;
    SDL_Palette pal;

    int i = 0; int z;
    pal.colors = malloc(sizeof(SDL_Color));
    while(i < max_attempts && pal.ncolors <= MAX_PALETTE_COLORS)
    {
        // Getting a random pixel from input surface
        Uint32 col = getPixel32(surf, (int)rand()%surf->w, (int)rand()%surf->h);

        int pixel_exist = 0;
        z = 0;
        // Checking pixel for existence in palette
        while(z < pal.ncolors)
        {
            if(SDL_COLOR_TO_RGBA(pal.colors[z]) == col)
            {
                pixel_exist = 1;
                break;
            }

            ++z;
        }

        // Appending it to pallette if it still not in
        if(!pixel_exist)
        {
            pal.ncolors++;
            pal.colors = realloc(pal.colors, pal.ncolors*sizeof(SDL_Color));
            RGBA_TO_SDL_COLOR(pal.colors[pal.ncolors-1], col);
        }

        i++;
    }

    return pal;
}

SDL_Surface * genCreatePalettizedSurface(SDL_Surface * source, SDL_Palette * palette)
{
    if(palette->ncolors > (Uint8)-1){
        printf("%s: alert: abnormal palette colors count detected (%d). "
               "Fixing it, but proceeding may result in fault!\n",
               __FUNCTION__, palette->ncolors);
        palette->ncolors = MAX_PALETTE_COLORS;
    }

    // Creating 8-bit blank surface
    SDL_Surface * result = SDL_CreateRGBSurface(0, source->w, source->h, 8, 0, 0, 0, 0);
    SDL_SetPalette(result, SDL_LOGPAL|SDL_PHYSPAL, palette->colors, 0, palette->ncolors);

    if(!result)
        return NULL;

    // Copying
    SDL_LockSurface(source);
    Uint32 * srcbuf = source->pixels;
    SDL_UnlockSurface(source);

    SDL_LockSurface(result);
    Uint8 * dstbuf = result->pixels;
    SDL_UnlockSurface(result);

    // Filling it with pixels
    register long i;
    register long z = 0;

    long pixels_n = source->w*source->h;

    for(i = 0; i < pixels_n; ++i)
    {
        //putPixel8s(result, i, genGetNearestColor(getPixel32s(source, i), palette));
        dstbuf[i] = genGetNearestColor(srcbuf[i], palette);

        if(++z > source->h)
        {
            ++progbar;
            z = 0;
        }
    }

    return result;
}
