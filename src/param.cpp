#include "../inc/param.h"

const static unsigned LEVELCAP = 99;

// because the library is dealing with unsigned integers, i should
// at least be extra extra safe
// the unsigned int maximum is defined as UINT_MAX from climits header

// a - b
int sub_will_underflow(unsigned int a, unsigned b) {
  return b > a;
}

// a + b
int add_will_overflow(unsigned int a, unsigned int b) {
  return (a + b) < a;
}

int mult_will_overflow(unsigned int a, unsigned int b) {
  if (a == 0 || b == 0) return 0;
  return a > UINT_MAX / b;
}


Parameters::Parameters(uint v, uint w, uint s, uint d, uint i, uint f, uint c, uint l) {
  v > LEVELCAP ? Vitality = LEVELCAP     : Vitality = v;
  w > LEVELCAP ? Willpower = LEVELCAP    : Willpower = w;
  s > LEVELCAP ? Strength = LEVELCAP     : Strength = s;
  d > LEVELCAP ? Dexterity = LEVELCAP    : Dexterity = d;
  i > LEVELCAP ? Intelligence = LEVELCAP : Intelligence = i;
  f > LEVELCAP ? Faith = LEVELCAP        : Faith = f;
  c > LEVELCAP ? Charisma = LEVELCAP     : Charisma = c;
  l > LEVELCAP ? Luck = LEVELCAP         : Luck = l;

  exp = 0;
  exp_last = 0;
  level = 1;

  adjust_attr_from_level();
}

void Parameters::level_up(uint v, uint w, uint s, uint d, uint i, uint f, uint c, uint l) {
  v > LEVELCAP ? Vitality = LEVELCAP     : Vitality += v;
  w > LEVELCAP ? Willpower = LEVELCAP    : Willpower += w;
  s > LEVELCAP ? Strength = LEVELCAP     : Strength += s;
  d > LEVELCAP ? Dexterity = LEVELCAP    : Dexterity += d;
  i > LEVELCAP ? Intelligence = LEVELCAP : Intelligence += i;
  f > LEVELCAP ? Faith = LEVELCAP        : Faith += f;
  c > LEVELCAP ? Charisma = LEVELCAP     : Charisma += c;
  l > LEVELCAP ? Luck = LEVELCAP         : Luck += l;

  level++;
  exp_last = exp /* minus exp required to level up */;

  adjust_attr_from_level();
}


void Parameters::adjust_attr_from_level() {
  TotalHP;
  TotalMP;
  TotalStam;
  Stamina;  

  Satiation;
  Immunity;
  Movespeed;
  Movepen;
  Encumberance; 
  Evasion;
  Poise;

  SlashDef; 
  PierceDef;
  BluntDef;

  LightningDef;
  FireDef;
  FrostDef;
  PoisonDef;
  MagicDef;
  NuclearDef;

  DivineDef;
  ProfaneDef;
}
