#ifndef _ITEMS_
#define _ITEMS_

#include "param.h"

class Item {
  public:
    Item(const char* n, unsigned w, unsigned v) { name = n; weight = w; value = v; }
    ~Item() = default;

    unsigned weight;
    unsigned value;
    const char* name;
};

class Weapon : public Item {
  public:
    Weapon();
    
    Damage damage;
};

class Apparel : public Item {
  public:
    Apparel();
    
    Parameters param;
};

#endif
