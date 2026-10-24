#include "../inc/gameloop.h"
#include "../inc/entity.h"

void Update(Game_Manager* Game) {
  // update player timers

  for (Entity* cur : ENTITIES)
    cur->sprite->update_timer();

  // update enemy ai
}
