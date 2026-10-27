#include "../inc/gameloop.h"
#include "../inc/entity.h"

#include <iostream>
using namespace std;

void Update(Game_Manager* Game) {
  // update player timers
  // for each status effect in player.param.status effects
  // update dat timer

  // update entity ai
  for (Entity* cur : ENTITIES) {
    // cout << "Entity " << cur->get_id() << ": " << cur->param.HP << "\n";
  }

  // detect collisions between projectiles
  for (Entity* cur : ENTITIES) {
    // if entity is a projectile
    // if is colliding(Game->player->get_pos(), Game->player->get_coll(), cur->get_pos(), cur->get_coll())
    // Game->player->param.take_damage(cur->damage);
  }

  // update player sprites

  for (Entity* cur : ENTITIES)
    cur->sprite->update_timer();
}
