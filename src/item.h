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
#include <unordered_map>
#include "param.h"
#include "mathutils.h"

class Item;

// N.B. item drops are defined in entity.h as they must be rendered
// a bit annoying, but it's the easiest to way to them rendered on the map

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

// ======================================
// ===== item description structure =====
// ======================================

// basically a big structure containing one of every possible item
// adding a gamemap is as simple as copying an item from this structure
// it will allocate the memory as required
extern std::unordered_map<std::string, Item*> ITEM_DESCRIPTIONS;

// init function to read all item json files &
void Init_Item_Descriptions(void);
void Free_Item_Descriptions(void);

// functions for spawning items from the item descriptions
Item* spawn_item(const std::string& lookup_name);
Item* spawn_random_item(void);


// ================
// ===== item =====
// ================

class Item {
  public:
    Item();
    Item(std::string n, uint w, uint v);
    virtual ~Item() = default;

    // copy constructor for copying from the item description structure
    // when spawning items into the gamemap
    // derived classes override this to copy specific data
    virtual Item* clone() { return nullptr; };

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
    // constructor of sorts, when initialising from json
    void set_default_item_params(std::string n, uint w, uint v);
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
    Weapon();
    Weapon(std::string n, std::string json_path, std::string json_lookup,
           int w, int v,
           PhysicalDamages phys=nophys, unsigned pa=0,
           ElementalDamages elm=noelm,  unsigned ea=0) :
    Item(n, w, v) {
      damage = Damage(phys, pa, elm, ea);
      texture = nullptr;
      attacking = 0;

      idle_tex_lookup_name = "assets/sword_idle.png";
      atk_tex_lookup_name = "assets/sword_atk.png";

      // either oneshot or continuous must be set, never both or none
      oneshot = 0;
      update = 0;
      continuous = 0;
      active = 0;
      attacking = 0;

      num_frames = 3;
      update_every = 300;

      // used internally
      frame_num = 0;
      timer_time = 0;
      timer_last_update = 0;
    }

    virtual Item* clone();

    virtual void mod_param(Parameters& p) { return; }
    virtual void unmod_param(Parameters& p) { return; }
    virtual void mod_def(Parameters& p) { return; }
    virtual void unmod_def(Parameters& p) { return; }
    virtual void mod_dmg(Parameters& p) { return; }
    virtual void unmod_dmg(Parameters& p) { return; }

    virtual void attack() { return; }
    virtual std::string get_name() { return name; }

    void update_animation(void);
    void update_equipped_texture(std::string lookup);

    Damage damage;

    Color* texture;

    std::string idle_tex_lookup_name;
    std::string atk_tex_lookup_name;

    int oneshot;    // is it a one-shot weapon animation?
    int update;     // then it uses this bool var for timers;
    int continuous; // otherwise, it's a continuous animation,
    int active;     // and it uses this bool var for timers


  protected:
    enum class animation_type {oneshot, continuous};

    // a few 'constructors' that are to be called inside, say, StraightSword's
    // constructor, once it has read from the json file that was specified

    void update_weapon_animation_info(animation_type type, unsigned num_frames,
                                      uint64 update_every)
    {
      switch (type) {
      case animation_type::oneshot:    oneshot = 1; continuous = 0; break;
      case animation_type::continuous: continuous = 1; oneshot = 0; break;
      }

      this->update_every = update_every;
      this->num_frames = num_frames;
    }

    void update_weapon_lookup_names(std::string idle, std::string atk) {
      idle_tex_lookup_name = idle;
      atk_tex_lookup_name = atk;
    }

    void update_weapon_damage_params(Damage d) {
      damage = Damage(d);
    }

  private:
    int attacking;  // used internally kinda - helps set texture only once

    unsigned num_frames; // number of frames for the attack animation
    uint64 update_every; // how often (in ms) the animation should update

    // used internally for timer stuff
    unsigned frame_num;
    uint64 timer_time;
    uint64 timer_last_update;


};

class StraightSword : public Weapon {
  public:
    StraightSword(std::string json_path, std::string json_id); // TODO
    virtual Item* clone();
    virtual void attack();
};

// ===================
// ===== apparel =====
// ===================

class Apparel : public Item {
  public:
    Apparel();
    Apparel(std::string n, int w, int v,
            uint sl=100, uint pe=100, uint bl=100,
            uint lt=100, uint fi=100, uint fo=100,
            uint po=100, uint mg=100, uint nc=100,
            uint di=100, uint pr=100) :
    Item(n, w, v) {
      defense = Defense(sl,pe,bl,lt,fi,fo,po,mg,nc,di,pr);
    }

    virtual Item* clone();

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

  protected:
    virtual void update_apparel_defense_params(Defense d) {
      defense = Defense(d);
    }
};

class Helmet : public Apparel {
  public:
    Helmet(std::string json_path, std::string json_id);
    virtual Item* clone();

    virtual int equippable_on(Bodypart b);
    virtual Bodypart only_equippable_on();
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
