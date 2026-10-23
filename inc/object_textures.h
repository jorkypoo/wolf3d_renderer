/*
 * contains everything to do with object textures, or sprites
 * these require a bit more information per texture, such as
 * number of sprite frames, if it's a one-shot texture, etc
 *
 */ 

#ifndef _OBJECT_TEXTURES_
#define _OBJECT_TEXTURES_

#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

#include "image_load.h"
#include "def.h"

// textures are squares so...
#define sq(n) (n*n)

typedef struct i_sprite_texture i_sprite_texture;
struct i_sprite_texture {
  Color* tex;              // the texture itself
  const char* lookup_name; // the name given to the texture - must be unique
  unsigned num_frames;     // how many frames the texture is compromised of; 0 if it is a static tex)
  uint64 update_every;     // how often the sprite should update this texture
  int oneshot;             // is the sprite a one-shot animation?
};


/* ===== sprite stuff ===== */

// global texture buffer
extern unordered_map<string, i_sprite_texture> SPRITES;


int  Init_Global_Sprites(void);
void Free_Sprites(void);

#endif
