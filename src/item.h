/*
 * items on their own are useless - they act as a base class for derived items;
 * weapons to be held, apparel to be equipped, items to go in the inventory,
 * potions to be drunk, food to be eaten, scrolls & books to be read
 * thus, item constructor only takes a few shared variables, and the derived
 * classes init from a json file/json id combo
 *
 */

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
  righthand,
  lefthand,
} Hand;
const int num_hands = 2;

class Item {
  public:
    Item(std::string n, uint w, uint v);
    virtual ~Item() = default;

    virtual void mod_param(Parameters& p) { return; }
    virtual void unmod_param(Parameters& p) { return; }
    virtual void mod_def(Parameters& p) { return; }
    virtual void unmod_def(Parameters& p) { return; }
    virtual void mod_dmg(Parameters& p) { return; }
    virtual void unmod_dmg(Parameters& p) { return; }

    uint get_weight() { return weight; }
    uint get_value() { return value; }
    // uint get_appraisal() { return appraisal; }
    // void set_appraised(uint a) { appraised = std::min(a, get_appraisal()); }
    const std::string& get_name() { return name; }

  protected:
    uint weight;
    uint value;
    // uint appraisal;
    // uint appraised;
    std::string name;

};

// ===================
// ===== weapons =====
// ===================

class Weapon : public Item {
  public:
    Weapon(std::string n, std::string lookup, int w, int v,
           PhysicalDamages phys=nophys, unsigned pa=0,
           ElementalDamages elm=noelm,  unsigned ea=0) :
    Item(n, w, v) {
      damage = Damage(phys, pa, elm, ea);
      tex_lookup_name = lookup;
      texture = nullptr;
    }

    virtual void mod_param(Parameters& p);
    virtual void unmod_param(Parameters& p);
    virtual void mod_def(Parameters& p);
    virtual void unmod_def(Parameters& p);
    virtual void mod_dmg(Parameters& p);
    virtual void unmod_dmg(Parameters& p);

    Color* texture;

    // sorry but i'm making a built in hand texture, just like the C version
    // "wah wah you already did this" idgaffff its fine
    // it won't be a seprate class either, just built into Weapon
    // this also encompasses stuff to tell the weapon when it applies it's damage idkkkk


    virtual void main_ability() { return; }
    virtual void  alt_ability() { return; }
    virtual std::string get_name() { return name; }

  protected:
    std::string tex_lookup_name;
    Damage damage;

};

// ===================
// ===== apparel =====
// ===================

class Apparel : public Item {
  public:
    Apparel(std::string n, int w, int v,
            uint sl=100, uint pe=100, uint bl=100,
            uint lt=100, uint fi=100, uint fo=100,
            uint po=100, uint mg=100, uint nc=100,
            uint di=100, uint pr=100) :
    Item(n, w, v) {
      defense = Defense(sl,pe,bl,lt,fi,fo,po,mg,nc,di,pr);
    }

    virtual void mod_param(Parameters& p);
    virtual void unmod_param(Parameters& p);
    virtual void mod_def(Parameters& p);
    virtual void unmod_def(Parameters& p);
    virtual void mod_dmg(Parameters& p);
    virtual void unmod_dmg(Parameters& p);

    virtual int equippable_on(Bodypart b) { return 0; }
    virtual Bodypart only_equippable_on() { return (Bodypart) 0; }
    virtual std::string& get_name() { return name; }

    Defense defense;
};

// ===================
// ===== potions =====
// ===================

class Potion : public Item {
  Potion(std::string n, int w, int v) : Item(n,w,v) { ; }
};

// ===================
// ===== scrolls =====
// ===================

class Scroll: public Item {
  Scroll(std::string n, int w, int v) : Item(n,w,v) { ; }
};

// ================
// ===== food =====
// ================

class Food: public Item {
  Food(std::string n, int w, int v) : Item(n,w,v) { ; }
};


#endif
