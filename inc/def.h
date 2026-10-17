/*
 * contains definitions and utilities that the game should
 * draw from; some of which shall be initialised from a file...later
 * 
 */

#ifndef _DEFINITIONS_
#define _DEFINITIONS_

#define WINDOWNAME  "Wolf3D Demo"
#define WINDOWX     (960)
#define WINDOWY     (540)
#define PIXELSCALE  (2)
#define PIXELX      (320*PIXELSCALE)
#define PIXELY      (180*PIXELSCALE)
#define TEXSIZE     (64)
#define OBJSIZE     (128)
#define EQSIZE      (90)
#define FOGDIST     (15)
#define OBJ_FOGDIST (15*FOGDIST)

#include <cstdint>

typedef uint32_t Color;
typedef uint8_t  ARGB;

#endif
