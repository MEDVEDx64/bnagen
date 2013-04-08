#ifndef CONVERT_H_INCLUDED
#define CONVERT_H_INCLUDED

/** 24-bit to 8-bit palettized format
        surface converter **/

// Maximal colors count in palette
/*
#define DEFAULT_PALETTE_COLORS 110
#ifndef MAX_PALETTE_COLORS
#   define MAX_PALETTE_COLORS DEFAULT_PALETTE_COLORS
#else
#   if MAX_PALETTE_COLORS > 256 || MAX_PALETTE_COLORS < 0
#       undef MAX_PALETTE_COLORS
#       define MAX_PALETTE_COLORS DEFAULT_PALETTE_COLORS
#   endif
#endif
*/

#include <SDL/SDL.h>

/** Convert SDL_Color struct to RGBA integer **/
#define SDL_COLOR_TO_RGBA(color) \
    ( (color.b << 8) | (color.g << 16) | (color.r << 24) )

/** Convert RGBA back to SDL_Color **/
#define RGBA_TO_SDL_COLOR(color,rgba)   \
    do                                  \
    {                                   \
        color.r = rgba >> 16;           \
        color.g = rgba >> 8;            \
        color.b = rgba;                 \
    } while(0)

/** Create surface's 8-bit palette **/
SDL_Palette genScanSurface(SDL_Surface * surf, unsigned char colors);

/** Return color nearest to src_pixel and available in palette **/
//Uint8 genGetNearestColor(Uint32 src_pixel, SDL_Palette * palette);

/** Build the final palettized surface **/
SDL_Surface * genCreatePalettizedSurface(SDL_Surface * source, SDL_Palette * palette);

#endif // CONVERT_H_INCLUDED
