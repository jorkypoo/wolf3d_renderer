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
  const int disposition_bonus = 3;

  vitality = v;
  willpower = w;
  strength = s;
  dexterity = d;
  intelligence = i;
  faith = f;
  charisma = c;
  luck = l;

  vit_xp = 0; vit_disp = v * disposition_bonus;
  wpr_xp = 0; wpr_disp = w * disposition_bonus;
  str_xp = 0; str_disp = s * disposition_bonus;
  dex_xp = 0; dex_disp = d * disposition_bonus;
  int_xp = 0; int_disp = i * disposition_bonus;
  fai_xp = 0; fai_disp = f * disposition_bonus;
  chr_xp = 0; chr_disp = c * disposition_bonus;
  lck_xp = 0; lck_disp = l * disposition_bonus;

  hp = get_maxhp();
  mp = get_maxmp();
  stamina = get_maxstam();
  hunger = get_maxhunger();
  
  hunger_accumulator = 0;
  stamina_accumulator = 0;
  mp_accumulator = 0;
}

void Parameters::take_damage(Defense& victim, Damage& source) {
  ;
}

void Parameters::award_xp(Params param, uint xp) {
  switch (param) {
    case vit:
      vit_xp += xp * vit_disp;
      while (vit_xp >= total_xp_rq(vitality+1)) vitality++;
      break;
      
    case str:
      str_xp += xp * str_disp;
      while (str_xp >= total_xp_rq(strength+1)) strength++;
      break;
      
    case dex:
      dex_xp += xp * dex_disp;
      while (dex_xp >= total_xp_rq(dexterity+1)) dexterity++;
      break;
      
    case wpwr:
      wpr_xp += xp * wpr_disp;
      while (wpr_xp >= total_xp_rq(willpower+1)) willpower++;
      break;
      
    case inte:
      int_xp += xp * int_disp;
      while (int_xp >= total_xp_rq(intelligence+1)) intelligence++;
      break;
      
    case fai:
      fai_xp += xp * fai_disp;
      while (fai_xp >= total_xp_rq(faith+1)) faith++;
      break;
      
    case chr:
      chr_xp += xp * chr_disp;
      while (chr_xp >= total_xp_rq(charisma+1)) charisma++;
      break;
      
    case lck:
      lck_xp += xp * lck_disp;
      while (lck_xp >= total_xp_rq(luck+1)) luck++;
      break;
      
    default: break;
  }
}


uint Parameters::get_param_level(Params param) {
  switch (param) {
    case vit:  return vitality; 
    case str:  return strength;
    case dex:  return dexterity;
    case wpwr: return willpower;
    case inte: return intelligence;
    case fai:  return faith;
    case chr:  return charisma;
    case lck:  return luck;
    default:   return 0;
  }
}


uint Parameters::get_maxhp() {
  return (vitality * 5) + (strength * 2) + (willpower * 3) + 40;
}
uint Parameters::get_maxmp() {
  return (willpower * 3) + (intelligence * 3) + (charisma * 2) + 25;
}
uint Parameters::get_maxstam() {
  return (vitality * 5) + (dexterity * 3) + 10;
}
uint Parameters::get_maxhunger() {
  return (vitality * 5) + (willpower * 5) + 90;
}

double Parameters::get_movespeed() {
  return (dexterity * 0.08) + 2.4;
}
uint Parameters::get_composure() {
  return (strength * 3) + (luck * 2) + 70;
}
uint Parameters::get_agility() {
  return (dexterity * 3) + (luck * 2) + 70;
}
uint Parameters::get_fortitude() {
  return (intelligence * 2) + (faith * 2) + (luck) + 70;
}
uint Parameters::get_endurance() {
  return (strength * 2) + (vitality);
}
uint Parameters::get_satiety() {
  return (vitality * 10) + (willpower * 8) - (dexterity);
}
uint Parameters::get_bartering() {
  return (charisma * 5) + (faith * 5) + 50;
}
uint Parameters::get_lore() {
  return (intelligence * 2) + (faith * 2) + (charisma * 5);
}
uint Parameters::get_stealth() {
  return (dexterity * 5) + (willpower * 2) - (strength * 2);
}

void Parameters::recover_hp(uint amt) {
  hp = std::min(hp+amt, get_maxhp());
}
void Parameters::recover_mp(long double delta) {
  mp_accumulator += delta; // times by constants

  if (mp_accumulator >= 1.0L) {
    mp = std::min(++mp, get_maxmp());
    mp_accumulator -= 1.0L;
  }
}
void Parameters::recover_stam(long double delta) {
   stamina_accumulator += delta;

  if (stamina_accumulator >= 1.0L) {
    stamina = std::min(++stamina, get_maxstam());
    stamina_accumulator -= 1.0L;
  }
}
void Parameters::recover_hunger(uint amt) {
   hunger = std::min(hunger+amt, get_maxhunger());
}

void Parameters::deplete_hp(uint amt) {
  hp = sub_will_underflow(hp,amt) ? 0 : hp - amt;
}
void Parameters::deplete_mp(uint amt) {
  mp = sub_will_underflow(mp,amt) ? 0 : mp - amt;
}
void Parameters::deplete_stam(uint amt) {
  stamina = sub_will_underflow(stamina,amt) ? 0 : stamina - amt;
}
void Parameters::deplete_hunger(long double delta) {
  hunger_accumulator += delta;

  if (hunger_accumulator >= 1.0L) {
    hunger = std::max(--hunger, get_maxhunger());
    hunger_accumulator -= 1.0L;
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
