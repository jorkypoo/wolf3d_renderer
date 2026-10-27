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
using namespace std;

#include "image_load.h"
#include "def.h"


/* ===== map stuff ===== */

// what the map is made of
typedef struct Tile Tile;
struct Tile {
  int is_air;              // basically always 1 except for my predefined air tile
  const char* lookup_name; // the filepath, but acts as a lookup for the texture hash map
};

// must this be defined here?
extern Tile GAMEMAP[MAPX*MAPY];

int Init_Map(void);
void print_map(void); // for debugging


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
