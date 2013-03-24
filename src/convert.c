#include "convert.h"
#include "pixel.h"

#include <malloc.h>

Uint8 genGetNearestColor(Uint32 src_pixel, SDL_Palette * palette)
{
    /* Scanning upward and downwards,
        looking for src_pixel matching with any palette color */
    Uint32 current_color_upw = src_pixel;
    Uint32 current_color_dow = src_pixel + 1;
    int i;

    while(1)
    {
        for(i = 0; i < palette->ncolors; ++i)
        {
            if(current_color_upw == SDL_COLOR_TO_RGBA(palette->colors[i]))
                return i;

            if(current_color_dow == SDL_COLOR_TO_RGBA(palette->colors[i]))
                return i;
        }

        if(current_color_upw != (unsigned)-1) current_color_upw++;
        if(current_color_dow) current_color_dow--;

        /* Return 0 when nothing found */
        if(!current_color_dow && current_color_upw == (unsigned)-1)
            break;
    }

    return 0;
}

SDL_Palette genScanSurface(SDL_Surface * surf)
{
    int max_attempts = surf->w*surf->h;
    SDL_Palette pal;

    int i = 0; int z;
    while(i < max_attempts || pal.ncolors <= MAX_PALETTE_COLORS)
    {
        // Getting a random pixel from input surface
        Uint32 col = getPixel32(surf, (int)rand()%surf->w, (int)rand()%surf->h);

        int pixel_exist = 0;
        z = 0;
        // Checking pixel for existence in palette
        while(z < pal.ncolors)
            if(SDL_COLOR_TO_RGBA(pal.colors[z]) == col)
            {
                pixel_exist = 1;
                break;
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
    // Creating 8-bit blank surface

    /// TODO
    return NULL;
}
