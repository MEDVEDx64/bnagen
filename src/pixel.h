#ifndef PIXEL_H_INCLUDED
#define PIXEL_H_INCLUDED

/** This stuff based on lazyfoo's code,
    that he left on some forum board
    over the Internet **/

#define GETPIXEL_HEAD(typ, name) \
    typ name (SDL_Surface *surface, unsigned int x, unsigned int y)

#define PUTPIXEL_HEAD(typ, name) \
    void name (SDL_Surface *surface, unsigned int x, unsigned int y, typ arg)

// Declarations
/// Uint8 getPixel8(SDL_Surface *surface, int x, int y);
GETPIXEL_HEAD(Uint8, getPixel8);
/// Uint32 getPixel32(SDL_Surface *surface, int x, int y);
GETPIXEL_HEAD(Uint32, getPixel32);
/// void putPixel8(SDL_Surface *surface, int x, int y, Uint8 arg);
PUTPIXEL_HEAD(Uint8, putPixel8);

#define getPixel(surf, x, y) getPixel32(surf, x, y)

/* Implementation templates */
#define PIXEL_BASE(typ)                             \
    SDL_LockSurface(surface);                       \
    typ *pixels = (typ *)surface->pixels;           \
    SDL_UnlockSurface(surface);

#define GETPIXEL_BODY(typ, name)                        \
    GETPIXEL_HEAD(typ, name)                            \
    {                                                   \
        PIXEL_BASE(typ)                                 \
        return pixels[(y*surface->w)+x];                \
    }

#define PUTPIXEL_BODY(typ, name)                        \
    PUTPIXEL_HEAD(typ, name)                            \
    {                                                   \
        PIXEL_BASE(typ)                                 \
        pixels[(y*surface->w)+x] = arg;                 \
    }

#endif // PIXEL_H_INCLUDED
