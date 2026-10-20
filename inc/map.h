/*
 * holds the map structure that is made up of tiles, with their own properties
 * and functions to initialise the tiles & gamemap with said tiles
 *
 */

#ifndef _GLOBAL_MAP_
#define _GLOBAL_MAP_

#include "def.h"

typedef struct Tile Tile;
struct Tile {
  int is_air;              // basically always 1 except for my predefined air tile
  const char* lookup_name; // the filepath, but acts as a lookup for the texture hash map
};

// must this be defined here?
extern Tile GAMEMAP[MAPX*MAPY];


int Init_Map(void);
void print_map(void); // for debugging

#endif
