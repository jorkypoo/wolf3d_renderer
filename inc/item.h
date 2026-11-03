#ifndef _ITEMS_
#define _ITEMS_

#include "param.h"

class Item {
  private:
    uint weight;
    uint value;
    uint appraisal;
    std::string name;
    
  public:
    Item(const char* n, uint w, uint v, uint a) { name = n; weight = w; value = v; appraisal = a; }
    ~Item() = default;

    };

#endif
