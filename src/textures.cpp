#include "../inc/textures.h"

unordered_map<string, Color*> TEXTURES;

int Init_Textures(void) {
  // basically, read these values from a json instead of doing this manually
  // look in a specified mods folder too, so people can add their own textures

  TEXTURES["assets/redbrick.png"] = new Color[sq(TEXSIZE)];
  load_image_to_buff("assets/redbrick.png",TEXTURES["assets/redbrick.png"],TEXSIZE,TEXSIZE);

  TEXTURES["assets/wood.png"] = new Color[sq(TEXSIZE)];
  load_image_to_buff("assets/wood.png",TEXTURES["assets/wood.png"],TEXSIZE,TEXSIZE);

  return 1;
}

void Free_Textures(void) {
  // lol
}
