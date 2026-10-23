/*
 * everything relating to textures - tiles, sprites, enemies and weapons
 * also contains fogmaps which are applied within the rendering function
 *
 */

#ifndef _TEXTURES_
#define _TEXTURES_

#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

#include "image_load.h"
#include "def.h"

// textures are squares so...
#define sq(n) (n*n)


/* ===== fogmap stuff ===== */

#define FOGLEVELS  (64)
#define DARKLEVELS (32)

#define NUM_FOGMAP_COLORS (3)
typedef enum {
 FM_RED,
 FM_GREEN,
 FM_BLUE
} FM_COLORS;

extern Color FOGMAP[NUM_FOGMAP_COLORS][FOGLEVELS][256];


/* ===== texture stuff ===== */

// global texture buffer
extern unordered_map<string, Color*> TEXTURES;


int  Init_Global_Textures(void);
void Free_Textures(void);

#endif
