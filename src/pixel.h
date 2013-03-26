#ifndef PIXEL_H_INCLUDED
#define PIXEL_H_INCLUDED

/** This stuff based on lazyfoo's code,
    that he left on some forum board
    over the Internet **/

// Prototypes for accessing pixels by coordinates
#define GETPIXEL_HEAD(typ, name) \
    typ name (SDL_Surface *surface, unsigned int x, unsigned int y)

#define PUTPIXEL_HEAD(typ, name) \
    void name (SDL_Surface *surface, unsigned int x, unsigned int y, typ arg)

// Prototypes for accessing pixels by index
#define GETPIXEL_HEAD_S(typ, name) \
    typ name (SDL_Surface *surface, unsigned int addr)

#define PUTPIXEL_HEAD_S(typ, name) \
    void name (SDL_Surface *surface, unsigned int addr, typ arg)

// Declarations
/// Uint8 getPixel8(SDL_Surface *surface, unsigned int x, unsigned int y);
GETPIXEL_HEAD(Uint8, getPixel8);
/// Uint32 getPixel32(SDL_Surface *surface, unsigned int x, unsigned int y);
GETPIXEL_HEAD(Uint32, getPixel32);
/// Uint32 getPixel32s(SDL_Surface *surface, unsigned int addr);
GETPIXEL_HEAD_S(Uint32, getPixel32s);
/// void putPixel8s(SDL_Surface *surface, unsigned int addr, Uint8 arg);
PUTPIXEL_HEAD_S(Uint8, putPixel8s);

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

#define GETPIXEL_BODY_S(typ, name)                          \
    GETPIXEL_HEAD_S(typ, name)                              \
    {                                                       \
        PIXEL_BASE(typ)                                     \
        return pixels[addr];                                \
    }

#define PUTPIXEL_BODY_S(typ, name)                          \
    PUTPIXEL_HEAD_S(typ, name)                              \
    {                                                       \
        PIXEL_BASE(typ)                                     \
        pixels[addr] = arg;                                 \
    }

#endif // PIXEL_H_INCLUDED
