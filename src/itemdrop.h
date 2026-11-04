#ifndef _ITEMDROPS_
#define _ITEMDROPS_

#include <vector>
#include <initializer_list>
#include "item.h"
#include "entity.h"
#include "entity_textures.h"

int Add_ItemDrop(std::initializer_list<Item*> is, v2 p);

class Item_Drop : public Entity {
  public:
    Item_Drop(std::string path, std::string id, Sprite* s, std::initializer_list<Item*> itms, v2 p):
    Entity(path, id, s, p, {0,0}, 0.0, 0.5) {
      for (Item* i : itms)
        if (i != nullptr) items.push_back(i);
    }
    ~Item_Drop();

    virtual void update();

    void add_items(std::initializer_list<Item*> drops);
    std::vector<Item*>& get_items();

  private:
    std::vector<Item*> items;
};

#endif
