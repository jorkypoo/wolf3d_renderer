#include "../inc/entity.h"
#include "../inc/entity_textures.h"

Sprite::Sprite(Color* tex, unsigned nf, Uint64 ue, int os) {
  if (tex) texture = tex;

  num_frames = nf;
  frame_num = 0;
  update_every = ue;

  oneshot = os;
  update = 0;

  timer_time = 0;
  timer_last_update = 0;

  sprite_dist = 0;
}

Sprite::~Sprite() {
  texture = nullptr;
}

void Sprite::update_texture(const char* lookup_name) {
  if (SPRITES.find(lookup_name) == SPRITES.end()) return;
  
  i_sprite_texture dest = SPRITES[lookup_name];
  texture = dest.tex;
  num_frames = dest.num_frames;
  update_every = dest.update_every;
  oneshot = dest.oneshot;
}

void Sprite::update_timer(void) {
  if (num_frames == 0) return;
  if (oneshot && !update) return;

  timer_time = SDL_GetTicksNS();

  // must the sprite update it's current sprite frame
  const int change_sprite = timer_time - timer_last_update >= MS_TO_NS(update_every);
  if (change_sprite) {
    frame_num++;

    // loop back to the first frame_num if necessary
    if (frame_num >= num_frames) {
      frame_num = 0;

      if (oneshot) update = 0;
    }

    timer_last_update = timer_time;
  }
  
}

unsigned Sprite::get_num_frames()   { return num_frames; }
unsigned Sprite::get_frame_num()    { return frame_num; }
uint64   Sprite::get_update_every() { return update_every; }
int      Sprite::is_oneshot()       { return oneshot; }

void Sprite::activate_oneshot_sprite() { if (oneshot) update = 1; }
