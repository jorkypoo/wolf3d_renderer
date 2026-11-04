#include "itemdrop.h"
// #include <iostream>

int Add_ItemDrop(std::initializer_list<Item*> is, v2 p) {
  if (SPRITES.find("assets/itemdrop.png") == SPRITES.end()) {
    cout << "could not initialise itemdrop: assets/itemdrop.png missing\n";
    return 0;
  }

  // create the sprite & its texture for the entity
  i_sprite_texture t = SPRITES["assets/itemdrop.png"];
  Sprite* dest = new Sprite(t.tex, t.num_frames, t.update_every, t.oneshot);

  // look up the correct entity params and stuff to initialise w/ constructor
  std::string entity_param_path = "example/path.json";
  std::string entity_param_id   = "example_name";
  Item_Drop* e = new Item_Drop(entity_param_path, entity_param_id, dest, is, p);
  ENTITIES.push_back(e);

  return 1;
}

Item_Drop::~Item_Drop() {
  // errors may occur here if player's pick up item drop code is fucky
  for (Item* i : items)
    if (i != nullptr)
      // std::cout<<"deleting "<<i->get_name()<<std::endl;
      delete i;
}

// Item_Drop::Item_Drop(std::initializer_list<Item*> itms, v2 p) {
//   for (Item* i : itms)
//     if (i != nullptr)
//       items.push_back(i);
// }

void Item_Drop::update() {
  std::cout<<"hi from item drop! auau\n";
}

void Item_Drop::add_items(std::initializer_list<Item*> drops) {
  for (Item* i : drops) if (i != nullptr) items.push_back(i);
}

std::vector<Item*>& Item_Drop::get_items() {
  return items;
}

