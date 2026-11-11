#include "item.h"
#include "entity.h"

///////////////////////////
///// straight swords /////
///////////////////////////

StraightSword::StraightSword(std::string path, std::string id) {
  set_default_item_params("straight sword",1,1);

  // maybe weapons should use their own structure? idk
  i_sprite_texture info = SPRITES["assets/sword.png"];
  animation_type type = info.oneshot ? animation_type::oneshot : animation_type::continuous;
  update_weapon_animation_info(type, info.num_frames, info.update_every);
  update_equipped_texture(info.tex);

  update_weapon_lookup_names("assets/sword.png");
  Damage d = Damage();
  update_weapon_damage_params(d);
}

Item* StraightSword::clone() {
  return new StraightSword(*this);
}

void StraightSword::attack() {
  std::cout<<"attacking with "<<get_name()<<"!\n";
  return;
}

///////////////////////
///// weapon base /////
///////////////////////

Weapon::Weapon() {
  attacking = 0;
  num_frames = 0;
  update_every = 0;
  frame_num = 0;
  timer_time = 0;
  timer_last_update = 0;
  oneshot = 0;
  update = 0;
  continuous = 0;
  active = 0;
  texture = nullptr;
}

Item* Weapon::clone() {
  return new Weapon(*this);
}

void Weapon::update_equipped_texture(std::string lookup) {
  if (!SPRITES.count(lookup)) return;

  i_sprite_texture info = SPRITES[lookup];
  texture = info.tex;
  num_frames = info.num_frames;
  update_every = info.update_every;
}

void Weapon::update_equipped_texture(Color* tex) {
  if (!tex) return;

  texture = tex;
}

void Weapon::update_animation(void) {
  if (texture == nullptr) {
    if (!SPRITES.count(tex_lookup_name)) return;
    texture = SPRITES[tex_lookup_name].tex;
  }

  if (oneshot && update) { // it's a one-shot aniamtion - update accordingly
    timer_time = SDL_GetTicksNS();
    int should_update = timer_time - timer_last_update >=
                        NS_IN_MS(update_every);

    if (should_update) {
      frame_num++;
      // std::cout<<"player attacking with weapon\n";

      if (frame_num >= num_frames) { // animation is complete
        frame_num = 0;
        update = 0;
      }
      timer_last_update = timer_time;
    }
  }

  // continuous animations will display the first frame when not active
  // and display 1-num_frames whilst the thingy is active
  // this will probably work idk

  if (continuous) {
    if (!active) { // button is currently not being pressed
      frame_num = 0;

    } else {
      timer_time = SDL_GetTicksNS();
      int should_update = timer_time - timer_last_update >=
                          NS_IN_MS(update_every);

      if (should_update) {
        frame_num++;
        // std::cout<<"player attacking with weapon\n";

        if (frame_num >= num_frames) { // animation is complete
          frame_num = 1;
        }
        timer_last_update = timer_time;
      }
    }
  }

  /*
  if (continuous) { // it's a continuous animation - timers not really needed!
    if (active && !attacking) {
      attacking = 1; // prevent continuous texture lookup

      // std::cout<<"player blocking\n";
      if (TEXTURES.count(atk_tex_lookup_name)) {
        texture = TEXTURES[atk_tex_lookup_name];
      } else {
        ; // do something idk the texture wasnt found bro
      }

    } else if (!active && attacking) { // attack is not active and must go back to
      attacking = 0;

      // std::cout<<"player not blocking\n";
      if (TEXTURES.count(idle_tex_lookup_name)) {
        texture = TEXTURES[idle_tex_lookup_name];
      } else {
        ; // do something idk the texture wasnt found bro
      }
    }
  }
  */

}
