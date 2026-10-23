#include "../inc/object.h"
#include "../inc/object_textures.h"

void Sprite::update_texture(const char* lookup_name) {
  i_sprite_texture dest = SPRITES[lookup_name];
  texture = dest.tex;
  num_frames = dest.num_frames;
  update_every = dest.update_every;
  oneshot = dest.oneshot;
}
