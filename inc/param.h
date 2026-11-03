/*
 * parameters are attached to both the player & all entities, even if it's static
 * the base parameters are levelled up according to a class and maybe covenant
 * all attributes - hp, mp, stamina, attributes - depend on one or more params
 *
 * entities of all kinds will have parameters, but really most of this stuff is
 * for the player - i find it unnecessary for entities to be taking hunger into consideration
 *
 * side note: seeing as most of these are unsigned, write helpers for over/underflows
 *
 */

#ifndef _PARAMETERS_
#define _PARAMETERS_

#include <unordered_map>
#include <string>
#include <climits>
#include <iostream>
#include "def.h"

typedef unsigned int uint; // for readability, above all else

typedef enum {
  noparam,
  vit,
  str,
  dex,
  wpwr,
  inte,
  fai,
  chr,
  lck
} Params;

typedef enum {
  nophys,
  slash,
  pierce,
  blunt
} PhysicalDamages;

typedef enum {
  noelm,
  lightning,
  fire,
  frost,
  poison,
  magic,
  nuclear,
  divine,
  profane
} ElementalDamages;

// basically damage is a class that is attached to every entity
// it represents what damage the player will take when they attack/come into
// contact with them - check the update() function
//
// the players damage will of course be updated with whatever equipment theyre wearing
// but enemies will just deal a single flat damage type
//
// in order to level up a parameter, you must gain xp by completing tasks related
// to said param. classes start with set parameters and certain predispositions to
// increasing certain parameters
//
// the amount of xp to level up is determined by a global structure, defined below



class Damage {
  public:
    Damage(PhysicalDamages param1, uint dmg1, ElementalDamages param2, uint dmg2);
    ~Damage() = default;

    uint get_slashdmg(); 
    uint get_piercedmg();
    uint get_bluntdmg();
    uint get_lightningdmg();
    uint get_firedmg();
    uint get_frostdmg();
    uint get_poisondmg();
    uint get_magicdmg();
    uint get_nucleardmg();
    uint get_divinedmg();
    uint get_profanedmg();

    void set_slashdmg(uint a);
    void set_piercedmg(uint a);
    void set_bluntdmg(uint a);
    void set_lightningdmg(uint a);
    void set_firedmg(uint a);
    void set_frostdmg(uint a);
    void set_poisondmg(uint a);
    void set_magicdmg(uint a);
    void set_nucleardmg(uint a);
    void set_divinedmg(uint a);
    void set_profanedmg(uint a);

  private:
    uint slash_dmg; 
    uint pierce_dmg;
    uint blunt_dmg;
    uint lightning_dmg;
    uint fire_dmg;
    uint frost_dmg;
    uint poison_dmg;
    uint magic_dmg;
    uint nuclear_dmg;
    uint divine_dmg;
    uint profane_dmg;
};

class Defense {
  public:
    Defense(uint sl=100, uint pe=100, uint bl=100,
            uint lt=100, uint fi=100, uint fo=100,
            uint po=100, uint mg=100, uint nc=100,
            uint di=100, uint pr=100);
    ~Defense() = default;
    
    uint get_slashdef(); 
    uint get_piercedef();
    uint get_bluntdef();
    uint get_lightningdef();
    uint get_firedef();
    uint get_frostdef();
    uint get_poisondef();
    uint get_magicdef();
    uint get_nucleardef();
    uint get_divinedef();
    uint get_profanedef();

    void set_slashdef(uint a); 
    void set_piercedef(uint a);
    void set_bluntdef(uint a);
    void set_lightningdef(uint a);
    void set_firedef(uint a);
    void set_frostdef(uint a);
    void set_poisondef(uint a);
    void set_magicdef(uint a);
    void set_nucleardef(uint a);
    void set_divinedef(uint a);
    void set_profanedef(uint a);

  private:
    uint slash_def; 
    uint pierce_def;
    uint blunt_def;
    uint lightning_def;
    uint fire_def;
    uint frost_def;
    uint poison_def;
    uint magic_def;
    uint nuclear_def;
    uint divine_def;
    uint profane_def;
};


class Parameters {
  public:
    Parameters(uint v,uint w,uint s,uint d,uint i,uint f,uint c,uint l);
    ~Parameters() = default;

    void take_damage(Defense& victim, Damage& source);

    void award_xp(Params param, uint xp);
    uint get_param_level(Params param);

    uint get_maxhp();
    uint get_maxmp();
    uint get_maxstam();
    uint get_maxhunger();

    uint get_movespeed();
    uint get_composure();
    uint get_agility();
    uint get_fortitude();
    uint get_endurance();
    uint get_satiety();
    uint get_bartering();
    uint get_lore();
    uint get_stealth();

    void deplete_hp();
    void deplete_mp();
    void deplete_stam();
    void deplete_hunger();

    void recover_hp();
    void recover_mp();
    void recover_stam();
    void recover_hunger();
    
  protected:
    // parameters
    uint vitality;
    uint willpower;
    uint strength;
    uint dexterity;
    uint intelligence;
    uint faith;
    uint charisma;
    uint luck;
    
    // xp is awarded to a parameter for accomplishing a task
    // relating to said param
    // level in a stat is determined by a getter function
    uint vit_xp;
    uint vit_disposition;
    uint wpr_xp;
    uint wpr_disposition;
    uint str_xp;
    uint str_disposition;
    uint dex_xp;
    uint dex_disposition;
    uint int_xp;
    uint int_disposition;
    uint fai_xp;
    uint fai_disposition;
    uint chr_xp;
    uint chr_disposition;
    uint lck_xp;
    uint lck_disposition;
    
    // attributes - 0 is bad; max is good
    uint hp;           // current HP
    uint mp;           // current MP
    uint stamina;      // current stamina
    uint hunger;       // current hunger 
    
};

#endif
