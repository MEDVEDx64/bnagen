#include "convert.h"
#include "pixel.h"
#include "state.h"

#include <malloc.h>

#define compare_color(x,y) ( abs((x)->r - (y)->r) + abs((x)->g - (y)->g) + abs((x)->b - (y)->b) )

static inline Uint8 genGetNearestColor(Uint32 src_pixel, SDL_Palette * palette)
{
    /** alexx's code **/
    SDL_Color src;
    RGBA_TO_SDL_COLOR(src,src_pixel);

    register Uint8 i;
    static Uint8 result = 0;

    for (i = 1; i < palette->ncolors; i++) {
        if (compare_color(&src, &(palette->colors[result])) >  compare_color(&src, &(palette->colors[i]))) result = i;
    }

    return result;
}

#ifndef SCAN_ATTEMPTS_MULTIPLIER
#   define SCAN_ATTEMPTS_MULTIPLIER 2
#endif // SCAN_ATTEMPTS_MULTIPLIER

static inline int search_color(unsigned char r, unsigned char g, unsigned char b, SDL_Palette *pal)
{
    int i;
    for (i = 0; i<pal->ncolors; i++) {
        if (pal->colors[i].r == r && pal->colors[i].g == g && pal->colors[i].b == b) return 1;
    }

    return 0;
}

SDL_Palette genSortPalette(SDL_Palette * pal)
{
    pal->ncolors = 111; /// КОСТЫЛЬ!!!
    printf("сортировка...\n");
    SDL_Palette res;
    res.ncolors = pal->ncolors;
/*
    int i, j;
    res.colors[0] = pal->colors[0];
    for (i = 1; i<pal->ncolors; i++)
        if (min(&pal->colors[i], &res.colors[0])) res.colors[0] = pal->colors[i];
printf("1\n");
    for (i = 1; i<res.ncolors; i++) {
        res.colors[i].r = 255;
        res.colors[i].g = 255;
        res.colors[i].b = 255;
        for (j = 0; j<pal->ncolors; j++) {
            if (min(&pal->colors[j],&res.colors[i]) && min(&res.colors[i-1], &pal->colors[j])) res.colors[i] = pal->colors[j];
        }
    }
*/
    int i,j,k;

    int t = 0;
    for (i = 0; i<0xff; i++) {
        printf("i = %d\n", i);
        for (j = 0; j<0xff; j++) {
            for (k = 0; k<0xff; k++) {
                //поиск цвета 0xijk00 в pal
                if (search_color(i,j,k,pal)) {
                    res.colors[t].b = k;
                    res.colors[t].g = j;
                    res.colors[t].r = i;
                    t++;
                }
            }
        }
    }
    printf("done.\n");
    return res;
}

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

    printf(">>> %d [Kill me! Im at %d in %s!]\n", pal.ncolors, __LINE__, __FILE__);
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
