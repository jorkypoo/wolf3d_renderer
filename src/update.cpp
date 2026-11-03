#include "gameloop.h"
#include "entity.h"

#include <iostream>

void Update(Game_Manager* Game) {
  // update player timers
  // for each status effect in player.param.status effects
  // update dat timer

  // detect collisions between projectiles
  for (Entity* cur : ENTITIES) {
    // if entity is a projectile
    // if is colliding(Game->player->get_pos(), Game->player->get_coll(), cur->get_pos(), cur->get_coll())
    // Game->player->param.take_damage(cur->damage);
    if (is_colliding(Game->player->get_pos(), Game->player->get_col_radius(),
                     cur->get_pos(),          cur->get_col_radius())) {
      
    }
    
  }

  // update entity ai
  for (Entity* cur : ENTITIES) {
    // cout << "Entity " << cur->get_id() << ": " << cur->param.HP << "\n";
    cur->update();
  }

  // update player sprites

  for (Entity* cur : ENTITIES)
    cur->sprite->update_timer();
}
