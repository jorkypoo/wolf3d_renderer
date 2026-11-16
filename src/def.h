/*
 * contains definitions and utilities that the game should
 * draw from; some of which shall be initialised from a file...later
 *
 */

#ifndef _DEFINITIONS_
#define _DEFINITIONS_

#define WINDOWX     (960)
#define WINDOWY     (540)
#define PIXELSCALE  (2)
#define PIXELX      (320*PIXELSCALE)
#define PIXELY      (180*PIXELSCALE)
#define MAPX        (8)
#define MAPY        (8)
#define TEXSIZE     (128)
#define OBJSIZE     (128)
#define EQSIZE      (90)
#define FOGDIST     (12)
#define OBJ_FOGDIST (20*FOGDIST)

#define NS_IN_MS(ms) (ms * 1000000)

#include <cstdint>

typedef uint32_t Color;
typedef uint8_t  ARGB;
typedef uint64_t uint64; // same as SDL3's Uint64 definition

#endif
