#include "../inc/gameloop.h"
#include "../inc/entity.h"

void Update(Game_Manager* Game) {
  // update player timers
  // for each status effect in player.param.status effects
  // update dat timer

  for (Entity* cur : ENTITIES)
    cur->sprite->update_timer();

  // update enemy ai
}
