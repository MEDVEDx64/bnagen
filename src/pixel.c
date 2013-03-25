#include <SDL/SDL.h>
#include "pixel.h"

/** Creating put/getPixel functions implementations using templates **/

GETPIXEL_BODY(Uint8, getPixel8)
GETPIXEL_BODY(Uint32, getPixel32)
PUTPIXEL_BODY(Uint8, putPixel8)
