#include "../inc/param.h"

// because the library is dealing with unsigned integers, i should
// at least be extra extra safe
// the unsigned int maximum is defined as UINT_MAX from climits header

// a - b
inline int sub_will_underflow(unsigned int a, unsigned b) {
  return b > a;
}

// a + b
inline int add_will_overflow(unsigned int a, unsigned int b) {
  return (a + b) < a;
}

inline int mult_will_overflow(unsigned int a, unsigned int b) {
  if (a == 0 || b == 0) return 0;
  return a > UINT_MAX / b;
}

// ====================
// ==== parameters ====
// ====================

Parameters::Parameters(uint v,uint w,uint s,uint d,uint i,uint f,uint c,uint l)
{
  vitality = v;
  willpower = w;
  strength = s;
  dexterity = d;
  intelligence = i;
  faith = f;
  charisma = c;
  luck = l;

  vit_xp = 0; uint vit_disposition = 100 + (v * 3);
  wpr_xp = 0; uint wpr_disposition = 100 + (w * 3);
  str_xp = 0; uint str_disposition = 100 + (s * 3);
  dex_xp = 0; uint dex_disposition = 100 + (d * 3);
  int_xp = 0; uint int_disposition = 100 + (i * 3);
  fai_xp = 0; uint fai_disposition = 100 + (f * 3);
  chr_xp = 0; uint chr_disposition = 100 + (c * 3);
  lck_xp = 0; uint lck_disposition = 100 + (l * 3);
}

void Parameters::award_xp(Params param, uint xp) {
  switch (param) {
    case vit:  break;
    case str:  break;
    case dex:  break;
    case wpwr: break;
    case inte: break;
    case fai:  break;
    case chr:  break;
    case lck:  break;

    case noparam: break;
    default:      break;
  }
}

// =================
// ==== damages ====
// =================

Damage::Damage(PhysicalDamages param1, uint dmg1, ElementalDamages param2, uint dmg2) {
  switch (param1) {
    case nophys:    break;
    case slash:     slash_dmg = dmg1; break;
    case pierce:    pierce_dmg = dmg1; break;
    case blunt:     blunt_dmg = dmg1; break;
  }
  
  switch (param2) {
    case noelm:     break;
    case lightning: lightning_dmg = dmg1; break;
    case fire:      fire_dmg = dmg1; break;
    case frost:     frost_dmg = dmg1; break;
    case poison:    poison_dmg = dmg1; break;
    case magic:     magic_dmg = dmg1; break;
    case nuclear:   nuclear_dmg = dmg1; break;
    case divine:    divine_dmg = dmg1; break;
    case profane:   profane_dmg = dmg1; break;
  }
  
}

uint Damage::get_slashdmg() { return slash_dmg; }
uint Damage::get_piercedmg() { return pierce_dmg; }
uint Damage::get_bluntdmg() { return blunt_dmg; }
uint Damage::get_lightningdmg() { return lightning_dmg; }
uint Damage::get_firedmg() { return fire_dmg; }
uint Damage::get_frostdmg() { return frost_dmg; }
uint Damage::get_poisondmg() { return poison_dmg; }
uint Damage::get_magicdmg() { return magic_dmg; }
uint Damage::get_nucleardmg() { return nuclear_dmg; }
uint Damage::get_divinedmg() { return divine_dmg; }
uint Damage::get_profanedmg() { return profane_dmg; }
void Damage::set_slashdmg(uint a) { slash_dmg = a; }
void Damage::set_piercedmg(uint a) { pierce_dmg = a; }
void Damage::set_bluntdmg(uint a) { blunt_dmg = a; }
void Damage::set_lightningdmg(uint a) { lightning_dmg = a; }
void Damage::set_firedmg(uint a) { fire_dmg = a; }
void Damage::set_frostdmg(uint a) { frost_dmg = a; }
void Damage::set_poisondmg(uint a) { poison_dmg = a; }
void Damage::set_magicdmg(uint a) { magic_dmg = a; }
void Damage::set_nucleardmg(uint a) { nuclear_dmg = a; }
void Damage::set_divinedmg(uint a) { divine_dmg = a; }
void Damage::set_profanedmg(uint a) { profane_dmg = a; }

// ==================
// ==== defenses ====
// ==================

Defense::Defense(uint sl,uint pe,uint bl,uint lt,uint fi,uint fo, uint po,uint mg,uint nc,uint di,uint pr)
  {
  slash_def = sl;
  pierce_def = pe;
  blunt_def = bl;
  lightning_def = lt;
  fire_def = fi;
  frost_def = fo;
  poison_def = po;
  magic_def = mg;
  nuclear_def = nc;
  divine_def = di;
  profane_def = pr;
}

uint Defense::get_slashdef() { return slash_def; }
uint Defense::get_piercedef() { return pierce_def; }
uint Defense::get_bluntdef() { return blunt_def; }
uint Defense::get_lightningdef() { return lightning_def; }
uint Defense::get_firedef() { return fire_def; }
uint Defense::get_frostdef() { return frost_def; }
uint Defense::get_poisondef() { return poison_def; }
uint Defense::get_magicdef() { return magic_def; }
uint Defense::get_nucleardef() { return nuclear_def; }
uint Defense::get_divinedef() { return divine_def; }
uint Defense::get_profanedef() { return profane_def; }
void Defense::set_slashdef(uint a) { slash_def = a; }
void Defense::set_piercedef(uint a) { pierce_def = a; }
void Defense::set_bluntdef(uint a) { blunt_def = a; }
void Defense::set_lightningdef(uint a) { lightning_def = a; }
void Defense::set_firedef(uint a) { fire_def = a; }
void Defense::set_frostdef(uint a) { frost_def = a; }
void Defense::set_poisondef(uint a) { poison_def = a; }
void Defense::set_magicdef(uint a) { magic_def = a; }
void Defense::set_nucleardef(uint a) { nuclear_def = a; }
void Defense::set_divinedef(uint a) { divine_def = a; }
void Defense::set_profanedef(uint a) { profane_def = a; }
