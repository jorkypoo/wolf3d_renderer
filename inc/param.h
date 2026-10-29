/*
 * parameters are attached to both the player & all entities, even if it's static
 * the base parameters are levelled up according to a class and maybe covenant
 * all attributes - hp, mp, stamina, attributes - depend on one or more params
 *
 * entities shall have a preset set of parameters, only the player can level up
 * perhaps there is some cheeky predetermined attribute modifying for enemies
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
using namespace std;

#include "def.h"

// for readability, above all else
typedef unsigned int uint;

class Parameters;
class Damage;
class Status_Effect;

// basically damage is a class that is attached to every entity
// it represents what damage the player will take when they attack/come into
// contact with them - check the update() function
//
// the players damage will of course be updated with whatever equipment theyre wearing
// but enemies will just deal a single flat damage type


class Damage {
  public:
    Damage() { ; }
    ~Damage() = default;

    // how long attribute drains last
    uint64 time_lasts;

    // attribute drains
    int mp_drain;
    int stam_drain;

    // damages
    int slash_damage; 
    int pierce_damage;
    int blunt_damage;

    int lightning_damage;
    int fire_damage;
    int frost_damage;
    int Poison_damage;
    int magic_damage;
    int nuclear_damage;

    int divine_damage;
    int profane_damage;
};


class Parameters {
  public:
    Parameters() = default;
    Parameters(uint lvl);
    ~Parameters() = default;

    // only the parameters can be updated - everything else depends on the params
    void level_up(uint v=0, uint w=0, uint s=0, uint d=0, uint i=0, uint f=0, uint c=0, uint l=0);
    // levels the player based on their pre-disposition to their parameters
    // void level_up();

    void take_damage(Damage* source) {
      this->HP -= source->slash_damage;
      cout << "taken damage: " << this->HP;
    }
    
    void add_xp(uint x) { exp += x; }
    uint get_xp()       { return exp; }
    uint get_level()    { return level; }

    // N.B. pararms & attributes don't have getters/setters bc i'm lazy
    // fuck that!!! so be careful
    // still, i'll provide an interface for setting custom 

    // parameters
    uint Vitality;
    uint Willpower;
    uint Strength;
    uint Dexterity;
    uint Intelligence;
    uint Faith;
    uint Charisma;
    uint Luck;
    
    // attributes
    int TotalHP;      // total health points available
    int HP;           // current HP
    int TotalMP;      // total mana points available
    int MP;           // current MP
    int TotalStam;    // total stamina available for movements
    int Stamina;      // current stamina

    int Satiation;    // resistance to starvation
    int Immunity;     // resistance to status effects
    int Movespeed;    // move speed when moving forwards/backwards
    int Movepen;      // move penalty applied when moving backwards
    int Encumberance; // how much gear you can carry 
    int Evasion;      // ability to dodge incoming attacks
    int Poise;        // ability to withstand incoming attacks
    int Appraisal;    // ability to appraise items & understand their effects

    // physical defenses
    int SlashDef; 
    int PierceDef;
    int BluntDef;

    // elemental defenses
    int LightningDef;
    int FireDef;
    int FrostDef;
    int PoisonDef;
    int MagicDef;
    int NuclearDef;

    // godly defenses
    int DivineDef;
    int ProfaneDef;

  private:
    uint exp;
    uint exp_last;
    uint level;
    
    // based on the current stats, adjust attributes accordingly
    // used in constructor & on player level up
    // manual adjustment of enemy attributes is ok
    void adjust_attr_from_level();

};


// status effects shall be initialised in maybe erm
void Init_Status_Effects(void);
// and exist in maybe a erm global list so any item, status effect or anything
// can proc the status effect, grabbing it by name here
extern unordered_map<string, Status_Effect> STATUS_EFFECTS;

// the status effects will only exist on the player (its easy lol)
// and exist in a dynamic array where they
// can easily be taken off on their own during update()


class Status_Effect {
  // how long the status effect lasts
  uint64 lasts_for;

  const char* name;

  // the different things it effects
  // a consistent minus of a defense or attribute that lasts for the duration
  // or a per-second drain of certain attributes for the duration
  // or a flat decrease in certain attributes that is not dependent on duration
  
};

#endif
