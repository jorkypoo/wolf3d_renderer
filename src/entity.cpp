#include "../inc/entity.h"
#include "../inc/entity_textures.h"

// logging tool or,
#include <iostream>

list<Entity*> ENTITIES;

static unsigned _id_num = 1;


Entity::Entity(std::string path, std::string id, Sprite* sp, v2 p, v2 d, double cr, double ir) {
  sprite = sp;
  id = _id_num++;
  pos = p;
  dir = d;


  collision_radius = cr;
  interact_radius = ir;

  // read json from the path to init the entity
  // Parameters param = Parameters(path);
  // Defense defense = Defense(path);
  // Damage damage = Damage(path);
}

Entity::~Entity() {
  ENTITIES.remove(this);
  delete sprite;
  sprite = nullptr;
}

void Entity::remove(void) {
  this->on_removal();
  delete this;
}

int Init_Entities(void) {
  Add_Entity("not ready", "assets/hourai.png", {3,3});
  Add_Entity("not ready", "assets/barrel.png", {5,5});

  return 1;
}

void Free_Entities(void) {
  while (!ENTITIES.empty()) {
    ENTITIES.front()->remove();
  }
}


int Add_Entity(const char* entity_lookup_name, const char* tex_lookup_name, v2 p) {
  // this will be the function called to initialise a basic entity
  // one without an ai script - so static objects for the most part
  // later, this function should only allow a path to a json file

  if (SPRITES.find(tex_lookup_name) == SPRITES.end()) {
    cout << "Could not initialise entity: %s" << tex_lookup_name << "\n";
    return 0;
  }
  
  // create the sprite & its texture for the entity
  i_sprite_texture t = SPRITES[tex_lookup_name];
  Sprite* dest = new Sprite(t.tex, t.num_frames, t.update_every, t.oneshot);

  // look up the correct entity params and stuff to initialise w/ constructor
  std::string entity_param_path = "example/path.json";
  std::string entity_param_id   = "example_name";
  Entity* e = new Entity(entity_param_path, entity_param_id, dest, p, {1,0}, 0.2, 0.5);
  ENTITIES.push_back(e);

  return 1;
}
