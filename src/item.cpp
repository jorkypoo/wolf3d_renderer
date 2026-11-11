#include "item.h"
#include <cstdlib>

std::unordered_map<std::string, Item*> ITEM_DESCRIPTIONS;

void Init_Item_Descriptions(void) {
  // i'll read from a json later (as always) but will hard code some stuff in now
  // ITEM_DESCRIPTIONS["gold helmet"] = new StraightSword("");
  ITEM_DESCRIPTIONS["sword"] = new StraightSword("path", "id");
}

void Free_Item_Descriptions(void) {
  auto it = ITEM_DESCRIPTIONS.begin();
  while (it != ITEM_DESCRIPTIONS.end()) {
    delete it->second;
    it = ITEM_DESCRIPTIONS.erase(it);
  }
}

Item::Item(std::string n, uint w, uint v) {
  name = n;
  weight = w;
  value = v;
  // appraisal = a;
  // appraised = 0;
}

Item::Item() {
  weight = 1;
  value = 0;
  name = "unset";
}

void Item::set_default_item_params(std::string n, uint w, uint v) {
  name = n;
  weight = w;
  value = v;
}

Item* spawn_item(const std::string& lookup_name) {
  auto it = ITEM_DESCRIPTIONS.find(lookup_name);

  if (it == ITEM_DESCRIPTIONS.end())
    return nullptr;

  Item* ret = it->second->clone();
  return ret;
}

Item* spawn_random_item() {
  int r = rand() % ITEM_DESCRIPTIONS.size();

  auto i = ITEM_DESCRIPTIONS.begin();
  for (int c = 0; c < r; c++, i++) {;}

  if (i == ITEM_DESCRIPTIONS.end() || i->second == nullptr) return nullptr;

  Item* dest = i->second->clone();
  return dest;
}

