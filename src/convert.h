#ifndef CONVERT_H_INCLUDED
#define CONVERT_H_INCLUDED

/** 24-bit to 8-bit palettized format
        surface converter **/

// Maximal colors count in palette
#define DEFAULT_PALETTE_COLORS 112
#ifndef MAX_PALETTE_COLORS
#   define MAX_PALETTE_COLORS DEFAULT_PALETTE_COLORS
#else
#   if MAX_PALETTE_COLORS > 256 || MAX_PALETTE_COLORS < 0
#       undef MAX_PALETTE_COLORS
#       define MAX_PALETTE_COLORS DEFAULT_PALETTE_COLORS
#   endif
#endif

#include <SDL/SDL.h>

/** Creates surface's 8-bit palette **/
SDL_Palette * genScanSurface(SDL_Surface * surf);

#endif // CONVERT_H_INCLUDED
