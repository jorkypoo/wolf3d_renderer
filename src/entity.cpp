#include "../inc/entity.h"
#include "../inc/entity_textures.h"

// logging tool or,
#include <iostream>

list<Entity*> ENTITIES;

static unsigned _id_num = 1;


Entity::Entity(Sprite* sp, v2 p, v2 d) {
  sprite = sp;
  id = _id_num++;
  pos = p;
  dir = d;
}

Entity::~Entity() {
  ENTITIES.remove(this);
  delete sprite;
  sprite = nullptr;
}

void Entity::Remove(void) {
  delete this;
}

int Init_Entities(void) {
  Add_Entity("not ready", "assets/hourai.png", {3,3});
  Add_Entity("not ready", "assets/barrel.png", {5,5});

  return 1;
}

void Free_Entities(void) {
  while (!ENTITIES.empty()) {
    ENTITIES.front()->Remove();
  }
}

int Add_Entity(const char* entity_lookup_name, const char* tex_lookup_name, v2 p) {
  if (SPRITES.find(tex_lookup_name) == SPRITES.end()) {
    cout << "Could not initialise entity: %s" << tex_lookup_name << "\n";
    return 0;
  }
  
  // create the sprite & its texture for the entity
  i_sprite_texture t = SPRITES[tex_lookup_name];
  Sprite* dest = new Sprite(t.tex, t.num_frames, t.update_every, t.oneshot);

  // look up the correct entity params and stuff to initialise w/ constructor
  Entity* e = new Entity(dest, p, {0,0});
  ENTITIES.push_back(e);

  return 1;
}
