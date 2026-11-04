#ifndef _ITEMS_
#define _ITEMS_

#include <string>
#include <vector>
#include <initializer_list>
#include "param.h"
#include "mathutils.h"

typedef enum {
  head,
  neck,
  chest,
  fingers,
  legs,
  feet,
} Bodypart;
const int num_bodyparts = 6;

typedef enum {
  right,
  left,
} Hand;
const int num_hands = 2;

class Item {
  public:
    Item(std::string n, uint w, uint v, uint a);
    virtual ~Item() = default;

    virtual void mod_param(Parameters& p) { return; } 
    virtual void unmod_param(Parameters& p) { return; } 
    virtual void mod_def(Parameters& p) { return; } 
    virtual void unmod_def(Parameters& p) { return; } 
    virtual void mod_dmg(Parameters& p) { return; } 
    virtual void unmod_dmg(Parameters& p) { return; } 

    uint get_weight() { return weight; }
    uint get_value() { return value; }
    uint get_appraisal() { return appraisal; }
    void set_appraised(uint a) { appraised = std::min(a, get_appraisal()); }
    const std::string& get_name() { return name; }

  protected:
    uint weight;
    uint value;
    uint appraisal;
    uint appraised;
    std::string name;

};

#endif
