#include "../inc/entity_textures.h"

unordered_map<string, i_sprite_texture> SPRITES;

int Init_Sprites(void) {
  // read from a json file and collate this automatically - ill hard code it for now

  // for static objects, such as a barrel, they do not need an animation
  // thus, mark the num_frames of the animation as 0 (1 will not work due to the way timers work)
  // 
  // also, technically, the name does not have to be the file path, but it shall
  // always be unique, so why not use it for my own convetion
  // protections must be put in place when adding a new entry to SPRITES, so no
  // existing sprites would be overwritten

  // a static sprite
  SPRITES["assets/barrel.png"] = {0, "assets/barrel.png", 0, 0, 0};
  // if the sprite is static (only 1 frame - so num_frames is specified as 0) we gotta add 1 to the below statement
  // most reusable way to do this ermmm. its just hard coded now bc im lazy
  SPRITES["assets/barrel.png"].tex = new Color[sq(TEXSIZE) * (SPRITES["assets/barrel.png"].num_frames+1)];
  load_image_to_buff("assets/barrel.png",SPRITES["assets/barrel.png"].tex,TEXSIZE,TEXSIZE);

  // an animated sprite
  SPRITES["assets/hourai.png"] = {0, "assets/hourai.png", 2, 200, 0};
  SPRITES["assets/hourai.png"].tex = new Color[( sq(TEXSIZE) * (SPRITES["assets/hourai.png"].num_frames) )];
  // this step will have to be completed for however many sprite frames exist under the same texture
  // one sprite frame must be it's own unique file, sorry
  // also maintain an offset as this loading of images will be automatic
  // unsigned offset = i * sq(TEXSIZE) - first 
  load_image_to_buff("assets/hourai1.png",SPRITES["assets/hourai.png"].tex,            TEXSIZE,TEXSIZE);
  load_image_to_buff("assets/hourai2.png",SPRITES["assets/hourai.png"].tex+sq(TEXSIZE),TEXSIZE,TEXSIZE);

  return 1;
}

int Init_Global_Sprites(void) {
  Init_Sprites();

  return 1;
}

void Free_Sprites() {
  for (const auto& pair : SPRITES) {
    delete pair.second.tex; // memory was only allocated for the texture, not the struct as a whole
  }
}
