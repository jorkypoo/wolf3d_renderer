#include "entity.h"
#include "entity_textures.h"

// logging tool or,
#include <iostream>

list<Entity*> ENTITIES;

static unsigned _id_num = 1;


Entity::Entity(std::string path, std::string id, Sprite* sp, v2 p, v2 d, double cr, double ir) {
  sprite = sp;
  id = _id_num++;
  pos = p;
  dir = d;
  to_remove = 0;
  
  collision_radius = cr;
  interact_radius = ir;

  // read json from the path to init the entity here
  // butttt... i'll just hard code it
  Parameters param = Parameters(1,1,1,1,1,1,1,1);
  Defense defense = Defense();
  Damage damage = Damage();
}

Entity::~Entity() {
  this->on_removal();

  delete sprite;
  sprite = nullptr;
}

void Entity::remove(void) {
  this->to_remove = 1;
}

int Entity::set_for_removal(void) {
  return to_remove;
}

int Init_Entities(void) {
  // Add_Entity("yuki-onna", "SPRITES lookup name - not file name", pos);
  Add_Entity("not ready", "assets/hourai.png", {3,3});
  Add_Entity("not ready", "assets/barrel.png", {5,5});

  return 1;
}

void Free_Entities(void) {
  auto i = ENTITIES.begin();
  while (i != ENTITIES.end()) {
    delete *i;
    i = ENTITIES.erase(i);
  }
}

void Deferred_Remove_Entities(void) {
  // i hate c++ sometimes, look at this neat segfault: for (Entity* i : ENTITIES) if (i->set_for_removal()) delete i;
  // YOU'RE HIDING SHIT FROM MEEEEEEEEEEEEEEEEe
  auto i = ENTITIES.begin();
  while (i != ENTITIES.end()) {
    // i really hate c++ this is dumb
    if ((*i)->set_for_removal()) { // if entity must be removed
      delete *i;
      i = ENTITIES.erase(i);
    } else {                       // else, continue
      i++;
    }
  }
}

int Add_Entity(const char* entity_lookup_name, const char* tex_lookup_name, v2 p) {
  // this will be the function called to initialise a basic entity
  // one without an ai script - so static objects for the most part
  // later, this function should only allow a path to a json file

  if (SPRITES.find(tex_lookup_name) == SPRITES.end()) {
    cout << "could not initialise entity: " << tex_lookup_name << "\n";
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
