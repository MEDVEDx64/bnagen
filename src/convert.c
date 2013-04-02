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
    Uint8 result = 0;

    for (i = 1; i < palette->ncolors; i++) {
        ///if (compare_color(&src, &(palette->colors[i])) > compare_color(&src, &(palette->colors[i-1]))) return i-1;

        if (compare_color(&src, &(palette->colors[result])) >  compare_color(&src, &(palette->colors[i]))) result = i;
        //else return result;
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

static int less_color(SDL_Color *a, SDL_Color *b)
{
    static SDL_Color null_color = {0,0,0,0};
    return compare_color(a, &null_color) < compare_color(b, &null_color);
}

void genSortPalette(SDL_Palette * pal)
{
    int i;
    int n = pal->ncolors;

    for (i = 0; i<n; i++) {
        //находим минимум от i элемента до n элемента
        int nmin = i;
        int j;
        for (j = i+1; j<n; j++) {
            if (less_color(&pal->colors[j], &pal->colors[nmin])) nmin = j;
        }

        //заменяем nmin элемент с i
        char tmp = pal->colors[i].r;
        pal->colors[i].r = pal->colors[nmin].r;
        pal->colors[nmin].r = tmp;

        tmp = pal->colors[i].g;
        pal->colors[i].g = pal->colors[nmin].g;
        pal->colors[nmin].g = tmp;

        tmp = pal->colors[i].b;
        pal->colors[i].b = pal->colors[nmin].b;
        pal->colors[nmin].b = tmp;
    }
}

SDL_Palette genScanSurface(SDL_Surface * surf)
{
    int max_attempts = surf->w * SCAN_ATTEMPTS_MULTIPLIER;
    SDL_Palette pal;

    int i = 0; int z = 0;
    pal.colors = calloc(sizeof(SDL_Color), (unsigned char)-1);
    pal.ncolors = 0;

    while(i++ < max_attempts && pal.ncolors <= MAX_PALETTE_COLORS)
    {
        // Getting a random pixel from input surface
        SDL_Color col;
        unsigned int c = getPixel32(surf, (int)rand()%surf->w, (int)rand()%surf->h);
        RGBA_TO_SDL_COLOR(col,c);

        for (z = 0; z < pal.ncolors; z++) {
            if (compare_color(&col, &(pal.colors[z])) < 40) break;
            ///if (col.r == pal.colors[z].r && col.g == pal.colors[z].g && col.b == pal.colors[z].b) break;
        }

        if (z < pal.ncolors) continue; //pixel exist!

        pal.colors[pal.ncolors++] = col;
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
