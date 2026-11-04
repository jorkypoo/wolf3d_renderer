#include "gameloop.h"
#include "entity.h"
#include "itemdrop.h"

void Input(Game_Manager* Game) {
  const double rotspeed = 2.8 * Game->get_delta();
  const double movspeed = 2.5 * Game->get_delta();
  // const double movspeed = Game->player->param.get_movespeed() * Game->get_delta();
  const double mov_penalty = 0.8;
  // const double mov_penality = Game->player->get_movepenalty();

  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {

      /* will exit the game loop and cleanup & shutdown */
      case SDL_EVENT_QUIT:
        Game->set_running_state(0);
        break;

      /* key presses */
      case SDL_EVENT_KEY_DOWN:
        switch (e.key.key) {
          case SDLK_F1:
            SDL_SetWindowFullscreen(Game->window, Game->is_borderless());
            Game->set_window_borderless(!Game->is_borderless());
            break;
          case SDLK_ESCAPE:
            Game->set_running_state(0);
            break;
          case SDLK_F2:
            Add_ItemDrop({}, {2,3});
            break;
        } // e.key.key
    } // e.type
  }

  /* keyboard input */


  const bool* ks = SDL_GetKeyboardState(NULL);
  // moving players dir
  if (ks[SDL_SCANCODE_RIGHT])
    Game->player->rotate(-rotspeed);
  if (ks[SDL_SCANCODE_LEFT])
    Game->player->rotate(+rotspeed);

  // moving players position
  if (ks[SDL_SCANCODE_UP]) 
    Game->player->move(movspeed);
  if (ks[SDL_SCANCODE_DOWN]) 
    Game->player->move(-movspeed * mov_penalty);
  

  // shooting and menu stuff
  if (ks[SDL_SCANCODE_Z]) {
    ;
  }
  if (ks[SDL_SCANCODE_X]) {
    ;
  }
  if (ks[SDL_SCANCODE_C]) {
    ;
  }
}
