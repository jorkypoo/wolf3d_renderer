#include "../inc/map.h"
#include <iostream> // for testing
#include <string>
#include <unordered_map>
using namespace std;

Tile GAMEMAP[MAPX*MAPY];

// this is just my way of organizing all the me- and user-defined tiles
// ill read them from a json files laaater but i just want somehting working
// allow for users to create their own lookup names - it doesn't have to be the filename!
unordered_map<string, Tile> tiles;


int Init_Tiles(void) {
  Tile air = {1, "air"};
  Tile blk = {0, "assets/redbrick.png"};
  Tile wod = {0, "assets/wood.png"};
  tiles[air.lookup_name] = air;
  tiles[blk.lookup_name] = blk;
  tiles[wod.lookup_name] = wod;

  return 1;
}

int Init_Gamemap(void) {
  // set everything to air idgaf
  for (int y = 0; y < MAPY; y++)
    for (int x = 0; x < MAPX; x++) 
      GAMEMAP[MAPX * y + x] = tiles["air"];

  for (int y = 0; y < MAPY; y++)
    for (int x = 0; x < MAPX; x++) {
      if (!y || !x || y == MAPY-1 || x == MAPX-1)
        GAMEMAP[MAPX * y + x] = tiles["assets/redbrick.png"];
        
      if ((x == 3 && y == 5) || (x == 2 && y == 5))
        GAMEMAP[MAPX * y + x] = tiles["assets/wood.png"];
    }

  return 1;
}

void print_map() {
  for (int y = 0; y < MAPX; y++) {
    cout << "\n";
    for (int x = 0; x < MAPX; x++) {
      cout << GAMEMAP[MAPX * y + x].lookup_name << "\n";
    }
  }
}

int Init_Map(void) {
  Init_Tiles();
  Init_Gamemap();

  return 1;
}
