#include "convert.h"
#include "pixel.h"

#include <malloc.h>

Uint8 asd = 0;

Uint8 genGetNearestColor(Uint32 src_pixel, SDL_Palette * palette)
{
    /**

    TEST CODE
    it actually doesn't work

    **/

    Uint32 exa_u = (unsigned)-1;
    Uint32 exa_u_old = (unsigned)-1;
    Uint32 exa_d = (unsigned)-1;
    Uint32 exa_d_old = (unsigned)-1;

    Uint8 candidate_u = 0;
    Uint8 candidate_d = 0;

    Uint8 i;
    for(i = 0; i < palette->ncolors; ++i)
    {
        exa_u_old = exa_u;
        exa_d_old = exa_d;

        exa_u = SDL_COLOR_TO_RGBA(palette->colors[i]) - src_pixel;
        exa_d = src_pixel - SDL_COLOR_TO_RGBA(palette->colors[i]);

        if(exa_u >= exa_u_old)
            exa_u = exa_u_old;
        else
            candidate_u = i;

        if(exa_d <= exa_d_old)
            exa_d = exa_d_old;
        else
            candidate_d = i;
    }

    if((src_pixel - SDL_COLOR_TO_RGBA(palette->colors[candidate_u]))
            > src_pixel - SDL_COLOR_TO_RGBA(palette->colors[candidate_d]))
                return candidate_d;
    else
        return candidate_u;
}

SDL_Palette genScanSurface(SDL_Surface * surf)
{
    int max_attempts = surf->w*8; /// FIXME — should it be different?
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
    // Creating 8-bit blank surface
    SDL_Surface * result = SDL_CreateRGBSurface(0, source->w, source->h, 8, 0xff000000, 0xff0000, 0xff00, 0xff);
    SDL_SetPalette(result, SDL_LOGPAL|SDL_PHYSPAL, palette->colors, 0, palette->ncolors);

    if(!result)
        return NULL;

    // Filling it with pixels
    register int x,y;
    for(x = 0; x < source->w; ++x)
        for(y = 0; y < source->h; ++y)
            putPixel8(result, x, y, genGetNearestColor(getPixel32(source, x, y), palette));

    return result;
}
