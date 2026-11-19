#include "entity.h"

// logging tool or,
#include <iostream>

std::list<Entity*> ENTITIES;
std::unordered_map<std::string, i_entity_params> ENTITY_PARAMETERS;

// idk if i'll ever use this lmao
static unsigned _id_num = 1;

// for use in json handling - json files hold attack type used as a string
PhysicalDamages get_pdmg_from_string(std::string s) {
  if (!strcmp(s.c_str(), "slash")) return slash;
  if (!strcmp(s.c_str(), "pierce")) return pierce;
  if (!strcmp(s.c_str(), "blunt")) return blunt;

  return nophys;
}

// for use in json handling - json files hold attack type used as a string
ElementalDamages get_edmg_from_string(std::string s) {
  if (!strcmp(s.c_str(), "lightning")) return lightning;
  if (!strcmp(s.c_str(), "fire")) return fire;
  if (!strcmp(s.c_str(), "frost")) return frost;
  if (!strcmp(s.c_str(), "poison")) return poison;
  if (!strcmp(s.c_str(), "magic")) return magic;
  if (!strcmp(s.c_str(), "nuclear")) return nuclear;
  if (!strcmp(s.c_str(), "divine")) return divine;
  if (!strcmp(s.c_str(), "profane")) return profane;

  return noelm;
}

Entity::Entity(std::string path, std::string id, Sprite* sp, v2 p, v2 d, double cr, double ir) {
  sprite = sp;
  id = ++_id_num;
  pos = p;
  dir = d;
  to_remove = 0;

  collision_radius = cr;
  interact_radius = ir;

  // one parameter is kinda useless now, but use the first string as lookup
  // in the entity parameters structure - avoids file i/o so we fail at init
  // instead of during runtime
  if (ENTITY_PARAMETERS.count(path) == 0) {
    param = Parameters(1,1,1,1,1,1,1,1);
    defense = Defense();
    damage = Damage();
    return;
  }

  i_entity_params e = ENTITY_PARAMETERS[id];
  defense = Defense(e.slash_def, e.pierce_def, e.blunt_def,
                    e.lightning_def, e.fire_def, e.frost_def,
                    e.poison_def, e.magic_def, e.nuclear_def,
                    e.divine_def, e.profane_def);
  damage = Damage(e.pdmg, e.pdmg_amt, e.edmg, e.edmg_amt);
  param = Parameters(e.vitality, e.willpower, e.strength, e.dexterity,
                     e.intelligence, e.faith, e.charisma, e.luck);
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
  add_entity("json_path", "json_id", "yuki onna attack", {3,3});
  add_entity("json_path", "json_id", "woodcrate1", {5,5});

  // here i'll initialise a big structure to contain entity data during runtime
  // so initialising entities doesn't perform file lookups
  // again, look in the mods folder laterrrr
  json entity_params = io::read_json_file("data/entity_params.json");
  if (entity_params == nullptr)
    std::cerr<<"failed to open data/entity_params.json\n";

  for (auto i = entity_params.begin(); i != entity_params.end(); i++) {
    json& cur = i.value();

    // lets start with defense - the easiest!
    // if value is unset, keep to the default of 100
    // this is so boring dude
    if (cur["defense"].empty() || cur["attack"].empty() || cur["params"].empty())
      continue;

    json& def = cur["defense"];

    uint sl = def["slash"].empty()     ? 100 : (uint) def["slash"];
    uint pe = def["pierce"].empty()    ? 100 : (uint) def["pierce"];
    uint bl = def["blunt"].empty()     ? 100 : (uint) def["blunt"];
    uint lt = def["lightning"].empty() ? 100 : (uint) def["lightning"];
    uint fi = def["fire"].empty()      ? 100 : (uint) def["fire"];
    uint fo = def["frost"].empty()     ? 100 : (uint) def["frost"];
    uint po = def["poison"].empty()    ? 100 : (uint) def["poison"];
    uint mg = def["magic"].empty()     ? 100 : (uint) def["magic"];
    uint nu = def["nuclear"].empty()   ? 100 : (uint) def["nuclear"];
    uint dv = def["divine"].empty()    ? 100 : (uint) def["divine"];
    uint pf = def["profane"].empty()   ? 100 : (uint) def["profane"];

    // now on to attack parameters
    // you can only initialise a damage class with either or both phys and elm dmg
    // not zero, not more than 2

    // and params...

    // now add them into entity_parameters - they can be on the stakc ig

  }

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

int add_entity(std::string json_path, std::string json_id, std::string tex_lookup_name, v2 p) {
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
  Entity* e = new Entity(json_path, json_id, dest, p, {1,0}, 0.2, 0.5);
  ENTITIES.push_back(e);

  return 1;
}
