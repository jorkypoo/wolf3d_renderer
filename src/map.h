/*
 * holds the map structure that is made up of tiles, with their own properties
 * and functions to initialise the tiles & gamemap with said tiles
 * also here is the textures for the map - makes it nicer for them to be in 1 file
 *
 */

#ifndef _GLOBAL_MAP_
#define _GLOBAL_MAP_

#include <string>
#include <unordered_map>
#include <iostream>

#include "image_load.h"
#include "def.h"
#include "jsonio.h"

#define FOGLEVELS  (64)
// #define DARKLEVELS (32)
#define NUM_FOGMAP_COLORS (3)
typedef enum {
 FM_RED,
 FM_GREEN,
 FM_BLUE
} FM_COLORS;

// what the map is made of
typedef struct Tile Tile;
struct Tile {
  int is_air;   // basically always 0 except for my predefined air tile - can be left out
  int is_decor; // used in random generation - spreads block out if decor
  int depth;    // used in random generation - matches similar nums in a level together
  int floor;    // can the texture be used as a floor texture in a level
  int ceiling;  // can the texture be used as a ceiling texture in a level
  std::string lookup_name; // the filepath, but acts as a lookup for the texture hash map
  Color* texture;
};

// must this be defined here?
extern Tile GAMEMAP[MAPX*MAPY];

extern Color FOGMAP[NUM_FOGMAP_COLORS][FOGLEVELS][256];

// global texture buffer
extern unordered_map<string, Color*> TEXTURES;

int Init_Map(void);

int  Init_Global_Textures(void);
void Free_Textures(void);

#endif
