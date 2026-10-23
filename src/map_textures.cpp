#include "../inc/map_textures.h"

// fogmap structure
// this could later be a dynamic array, to support multiple fogmaps
Color FOGMAP[NUM_FOGMAP_COLORS][FOGLEVELS][256];

// texture structure
unordered_map<string, Color*> TEXTURES;


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
  // basically, read these values from a json instead of doing this manually
  // look in a specified mods folder too, so people can add their own textures
  // also, i should allow people to use their own lookup names

  TEXTURES["assets/redbrick.png"] = new Color[sq(TEXSIZE)];
  load_image_to_buff("assets/redbrick.png",TEXTURES["assets/redbrick.png"],TEXSIZE,TEXSIZE);

  TEXTURES["assets/wood.png"] = new Color[sq(TEXSIZE)];
  load_image_to_buff("assets/wood.png",TEXTURES["assets/wood.png"],TEXSIZE,TEXSIZE);

  TEXTURES["assets/colorstone.png"] = new Color[sq(TEXSIZE)];
  load_image_to_buff("assets/colorstone.png",TEXTURES["assets/colorstone.png"],TEXSIZE,TEXSIZE);

  return 1;
}

int Init_Global_Textures(void) {
  Init_Fogmap();
  Init_Textures();

  return 1;
}

void Free_Textures(void) {
  for (const auto& pair : TEXTURES)
    delete pair.second;
}
