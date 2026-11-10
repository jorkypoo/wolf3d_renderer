#include "game.h"
#include "entity.h"

// helper functions for this header

std::string get_hand_as_string(Hand h) {
  switch (h) {
  case righthand: return "right hand";
  case lefthand:  return "left hand";
  }
  return "unknown hand";
}

std::string get_bodypart_as_string(Bodypart b) {
  switch (b) {
    case head: return "head";
    case neck: return "neck";
    case chest: return "chest";
    case fingers: return "fingers";
    case legs: return "legs";
    case feet: return "feet";
  }
  return "unknown bodypart";
}

void remove_item_from_vector(std::vector<Item*>& v, const Item* i) {
  v.erase(
    std::remove_if(v.begin(), v.end(), [i](const Item* x)
      { return x == i; } ), v.end()
  );
}

///////////////////////////////////////
// Player class function definitions //
///////////////////////////////////////

Player::Player() {
  pos = {0,0};
  dir = {-1,0};
  plane = {0,0.7};

  collision_radius = 0.2;

  param = Parameters(1,1,1,1,1,1,1,1);
  defense = Defense();

  for (std::size_t i = 0; i < num_hands; i++)
    hands[i] = nullptr;

  for (std::size_t i = 0; i < num_hands; i++)
    body[i] = nullptr;
}

Player::Player(double fov) {
  pos = {0,0};
  dir = {-1,0};
  plane = {0,fov};

  collision_radius = 0.2;
  interact_radius = 0.5;

  param = Parameters(1,1,1,1,1,1,1,1);
  defense = Defense();

  for (std::size_t i = 0; i < num_hands; i++)
    hands[i] = nullptr;

  for (std::size_t i = 0; i < num_hands; i++)
    body[i] = nullptr;
}

Player::~Player() {
  for (std::size_t i = 0; i < inventory.size(); i++)
    if (inventory[i] != nullptr) delete inventory[i];
}

unsigned Player::inv_size() {
  return inventory.size();
}

int Player::inv_has(const Item* i) {
  return (std::find(inventory.begin(), inventory.end(), i)
          != inventory.end()) ? 1 : 0;
}

int Player::inv_get_item_index(const Item* i) {
  auto it = std::find(inventory.begin(), inventory.end(), i);
  if (it != inventory.end())
    return std::distance(inventory.begin(), it);
  else
    return -1;
}

void Player::inv_add(std::vector<Item*>& itms) {
  std::cout<<"player picked up items drop item drop\n";
  for (Item* i : itms) {
    if (i != nullptr)
      inventory.push_back(i);
  }
  itms.clear();
}

Item* Player::inv_get_from_index(unsigned i) {
  if (i >= inventory.size())
    return nullptr;
  return inventory[i];
}

void Player::inv_remove_by_index(unsigned i) {
  if (i >= inventory.size())
    return;
  delete inventory[i];
}

void Player::inv_remove_by_item(Item* i) {
  if (!inv_has(i) || body_has(i) || hand_has(i))
    return;

  remove_item_from_vector(inventory, i);
  delete i;
}


void Player::hand_attack_main(Hand h) {
  // std::cout<<hands[h]<<std::endl;
  if (hands[h] == nullptr) return;

  std::cout<<"main attacking with "<<get_hand_as_string(h)<<std::endl;
}

void Player::hand_attack_alt(Hand h) {
  if (hands[h] == nullptr) return;

  std::cout<<"alt attacking with "<<get_hand_as_string(h)<<std::endl;
}

void Player::hand_equip(Hand h, Weapon* w) {
  if ((int) h >= num_hands || !w) return;
  if (hand_is_equipped(h)) return;

  // unequip equipped weapon & equip requested - swapping out
  // if (hand_is_equipped(h)) {
    // damage.sub(hands[h]->damage);
    //hands[h]->unmod_attr(this->param);
  // }

  hands[h] = w;
  hands[h]->mod_dmg(this->param);
}

void Player::hand_unequip(Hand h) {
  if ((int) h >= num_hands) return;
  if (!hand_is_equipped(h)) return;

  hands[h]->unmod_dmg(this->param);
  hands[h] = nullptr;
}

int Player::hand_has(const Item* i) {
  return (std::find(hands.begin(), hands.end(), i) != hands.end()) ? 1 : 0;
}

int Player::hand_is_equipped(Hand h) {
  if ((int) h >= num_hands)
    return 0;
  return (hands[h] == nullptr) ? 0 : 1;
}

Weapon* Player::hand_get_equipped(Hand h) {
  if ((int) h >= num_hands)
    return nullptr;
  return hands[h];
}

Color* Player::hand_get_texture(Hand h) {
  if ((int) h >= num_hands) return nullptr;
  if (hands[h] == nullptr) return nullptr;

  return (hands[h]->texture == nullptr) ? nullptr : hands[h]->texture;
}

void Player::body_equip(Bodypart b, Apparel* a) {
  if (!a || b >= num_bodyparts) return;
  if (!inv_has(a)) return;

  if (a->equippable_on(b) && !body_is_equipped(b)) {
    // if (body_is_equipped(b))
      // body[b]->unmod_def(this->param);

    body[b] = a;
    body[b]->mod_def(this->param);
  }
}

void Player::body_unequip(Bodypart b) {
  if (b >= num_bodyparts || !body_is_equipped(b))
    return;

  body[b]->unmod_def(this->param);
  body[b] = nullptr;
}

int Player::body_has(const Item* a) {
  return (std::find(body.begin(), body.end(), a) != body.end()) ? 1 : 0;
}

int Player::body_is_equipped(Bodypart b) {
  if (b >= num_bodyparts)
    return 0;
  return (body[b] == nullptr) ? 0 : 1;
}

Apparel* Player::body_get_equipped(Bodypart b) {
  if (b >= num_bodyparts)
    return nullptr;
  return body[b];
}

void Player::set_spawn_point(double x, double y) {
  pos.x = x;
  pos.y = y;
  dir = { -1.0, 0.0 };
  // plane = { 0.0, 0.7 };
}

void Player::rotate(double rot) {
  const v2 od = dir;
  const v2 op = plane;

  dir.x = od.x * cos(rot) - od.y * sin(rot);
  dir.y = od.x * sin(rot) + od.y * cos(rot);
  plane.x = op.x * cos(rot) - op.y * sin(rot);
  plane.y = op.x * sin(rot) + op.y * cos(rot);
}

void Player::move(double mov) {
  /* v2 tomov = { dir.x * mov, dir.y * mov };
  int next_x = (int) (pos.x + tomov.x + sign(tomov.x) * collision_radius);
  int cur_y  = (int) pos.y;
  int next_y = (int) (pos.y + tomov.y + sign(tomov.y) * collision_radius);
  int cur_x  = (int) pos.x;
  if (GAMEMAP[cur_y * MAPX + next_x].is_air)
    pos.x += tomov.x;
  if (GAMEMAP[next_y * MAPX + cur_x].is_air)
    pos.y += tomov.y; */

  v2 tomov = { dir.x * mov, dir.y * mov };

  pos.x += tomov.x;
  for (Entity* e : ENTITIES) {
    if (e->get_col_radius() == 0.0) continue;

    double dx = pos.x - e->get_posx();
    double dy = pos.y - e->get_posy();
    double distSq = SQ(dx) + SQ(dy);
    double min_dist = collision_radius + e->get_col_radius();

    if (distSq < min_dist * min_dist) {
      double dist = sqrt(distSq);
      if (dist == 0.0f) dist = 0.001f;

      double overlap = min_dist - dist;
      pos.x += (dx / dist) * overlap;
    }
  }

  int next_x = (int)(pos.x + sign(tomov.x) * collision_radius);
  int cur_y  = (int)pos.y;

  if (!GAMEMAP[cur_y * MAPX + next_x].is_air) {
    if (tomov.x > 0) pos.x = (double)next_x - collision_radius - 0.001f;
    else             pos.x = (double)next_x + 1.0f + collision_radius + 0.001f;
  }

  pos.y += tomov.y;
  for (Entity* e : ENTITIES) {
    if (e->get_col_radius() == 0.0) continue;

    double dx = pos.x - e->get_posx();
    double dy = pos.y - e->get_posy();
    double distSq = SQ(dx) + SQ(dy);
    double min_dist = collision_radius + e->get_col_radius();

    if (distSq < min_dist * min_dist) {
      double dist = sqrt(distSq);
      if (dist == 0.0f) dist = 0.001f;
      double overlap = min_dist - dist;
      pos.y += (dy / dist) * overlap;
    }
  }

  int cur_x  = (int)pos.x;
  int next_y = (int)(pos.y + sign(tomov.y) * collision_radius);

  if (!GAMEMAP[next_y * MAPX + cur_x].is_air) {
    if (tomov.y > 0) pos.y = (double)next_y - collision_radius - 0.001f;
    else             pos.y = (double)next_y + 1.0f + collision_radius + 0.001f;
  }
}

void Player::update_attacks() {
  ;
}

void Player::attack_input_state_changed(Hand h, bool state) {
  // here, we read what actions have been pressed by the player during input()
  // clicking an attack's button activates the one-shot update, if the attack
  // is a one-shot; it also must be unset after the animation has played
  //
  // it also activates the active switch, which is unset when the button has
  // been unpressed

  if (hands[h] == nullptr) return;

  if (hands[h]->oneshot)
    hands[h]->update = 1;
  if (hands[h]->continuous)
    hands[h]->active = state;

}

void Player::update_equipped_weapon_timers() {
  for (Weapon* w : hands)
    if (w != nullptr)
      w->update_animation();
}
