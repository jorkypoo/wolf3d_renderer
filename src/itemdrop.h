#ifndef _ITEMDROPS_
#define _ITEMDROPS_

#include <vector>
#include <initializer_list>
#include "item.h"

class Item_Drop {
  public:
    unsigned* texture;
    Item_Drop() {;}
    Item_Drop(std::initializer_list<Item*> itms) { for (Item* i : itms) if (i != nullptr) items.push_back(i); }
    // ~Item_Drop() { for (Item* i : items) if (i != nullptr) delete i; }

    void add_items(std::initializer_list<Item*> drops) {
      for (Item* i : drops) if (i != nullptr) items.push_back(i);
    }

    // assumed to be cleaned up in player's pickup
    std::vector<Item*>& get_items() { return items; }

  private:
    // v2 pos;
    // double radius;
    std::vector<Item*> items;
};

#endif
