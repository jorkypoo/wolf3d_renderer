#include "map.h"

#define sq(n) (n*n)


// the gamemap of tiles
Tile GAMEMAP[MAPX*MAPY];

// this is just my way of organizing all the me- and user-defined tiles
// ill read them from a json files laaater but i just want somehting working
// allow for users to create their own lookup names - it doesn't have to be the filename!
static unordered_map<string, Tile> tiles;

// fogmap structure
// this could later be a dynamic array, to support multiple fogmaps
Color FOGMAP[NUM_FOGMAP_COLORS][FOGLEVELS][256];

// textures are loaded into ram & stored here
unordered_map<string, Color*> TEXTURES;


int Init_Tiles(void) {
  Tile air = {1, 0, 0, "air"};
  Tile blk = {0, 0, 0, "bricks"};
  Tile wod = {0, 0, 0, "concrete"};
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
        GAMEMAP[MAPX * y + x] = tiles["concrete"];

      if ((x == 3 && y == 5) || (x == 2 && y == 5))
        GAMEMAP[MAPX * y + x] = tiles["bricks"];
    }

  return 1;
}

/* void print_map() {
  for (int y = 0; y < MAPX; y++) {
    cout << "\n";
    for (int x = 0; x < MAPX; x++) {
      cout << GAMEMAP[MAPX * y + x].lookup_name << "\n";
    }
  }
} */

int Init_Map(void) {
  Init_Tiles();
  Init_Gamemap();

  return 1;
}

int Init_Fogmap(void) {
  // void generate_fogmap(color fm[NUM_FM_COLOURS][FOG_LEVELS][256], argb r, argb g, argb b) {
  // original function definition in C ^^
  // this'll read from a json file later, and create as many fog maps as the user wants...
  // for now ill use the original colour: 0x9E
  Color r = 0x9E;
  Color g = 0x9E;
  Color b = 0x9E;

  for (size_t l = 0; l < FOGLEVELS; l++) {
    int fog = (l * 256) / (FOGLEVELS - 1);

    for (size_t i = 0; i < 256; i++) {
      FOGMAP[FM_RED]  [l][i] = ((i * (255 - fog)) + (r * fog)) / 255;
      FOGMAP[FM_GREEN][l][i] = ((i * (255 - fog)) + (g * fog)) / 255;
      FOGMAP[FM_BLUE] [l][i] = ((i * (255 - fog)) + (b * fog)) / 255;
    }
  }

  return 1;
}

int Init_Textures(void) {
  // look in a specified mods folder too, so people can add their own textures
  // also, i should allow people to use their own lookup names

  std::ifstream file("data/map_textures.json");
  if (!file.is_open()) {
    std::cerr<<"failed to open file: data/map_textures.json\n";
    return 0;
  }

  if (!json::accept(file)) {
    std::cerr<<"invalid json in: data/map_textures.json\n";
    return 0;
  }

  file.seekg(0, std::ios::beg);

  json base_textures = json::parse(file);
  if (base_textures.is_discarded()) {
    std::cerr << "failed to parse json file: data/map_textures/json\n";
    return 0;
  }
  // std::cout<<"hi\n";

  for (auto i = base_textures.begin(); i != base_textures.end(); i++) {
    json& cur = i.value();
    Tile t = {cur["is air"], cur["is decor"], cur["depth"],
              cur["texture lookup name"], nullptr};

    if (!t.is_air) {
      TEXTURES[t.lookup_name] = new Color[sq(TEXSIZE)];
      std::string p = cur["texture path"].get<std::string>();
      load_image_to_buff(p.c_str(), TEXTURES[t.lookup_name], TEXSIZE, TEXSIZE);
      t.texture = TEXTURES[t.lookup_name];
    }

    tiles[t.lookup_name] = t;
  }

  // TEXTURES["brick"] = new Color[sq(TEXSIZE)];
  // load_image_to_buff("assets/texture/bricks.png",TEXTURES["brick"],TEXSIZE,TEXSIZE);

  // TEXTURES["concrete"] = new Color[sq(TEXSIZE)];
  // load_image_to_buff("assets/texture/concrete.png",TEXTURES["concrete"],TEXSIZE,TEXSIZE);

  // TEXTURES["metal"] = new Color[sq(TEXSIZE)];
  // load_image_to_buff("assets/texture/metal.png",TEXTURES["metal"],TEXSIZE,TEXSIZE);

  return 1;
}

int Init_Global_Textures(void) {
  if (!Init_Fogmap()) return 0;
  if (!Init_Textures()) return 0;

  return 1;
}

void Free_Textures(void) {
  for (const auto& pair : TEXTURES)
    delete pair.second;
}
