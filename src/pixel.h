#ifndef PIXEL_H_INCLUDED
#define PIXEL_H_INCLUDED

/** This stuff based on lazyfoo's code,
    that he left on some forum board
    over the Internet **/

#define GETPIXEL_HEAD(typ, name) \
    typ name (SDL_Surface *surface, int x, int y)

// Declarations
/// Uint8 getPixel8(SDL_Surface *surface, int x, int y);
GETPIXEL_HEAD(Uint8, getPixel8);
/// Uint32 getPixel32(SDL_Surface *surface, int x, int y);
GETPIXEL_HEAD(Uint32, getPixel32);

#define getPixel(surf, x, y) getPixel32(surf, x, y)

/* Implementation template */
#define GETPIXEL_BODY(typ, name)                \
    GETPIXEL_HEAD(typ, name)                    \
    {                                           \
        if(x<0 || y<0 || surface == NULL)       \
            return 0;                           \
                                                \
        SDL_LockSurface(surface);                       \
        Uint32 *pixels = (Uint32*)surface->pixels;      \
        SDL_UnlockSurface(surface);                     \
                                                        \
        return pixels[(y*surface->w)+x];                \
    }

#endif // PIXEL_H_INCLUDED
