#include <SDL/SDL.h>
#include "pixel.h"

/** Creating put/getPixel functions implementations using templates **/

GETPIXEL_BODY(Uint8, getPixel8)
GETPIXEL_BODY(Uint32, getPixel32)
GETPIXEL_BODY_S(Uint32, getPixel32s)
PUTPIXEL_BODY_S(Uint8, putPixel8s)
