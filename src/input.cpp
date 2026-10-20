#include "../inc/gameloop.h"

void Input(Game_Manager* Game) {
  const double rotspeed = 3.0 * Game->get_delta();
  const double movspeed = 3.0 * Game->get_delta();
  const double mov_penalty = 0.8;

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
            break;
        } // e.key.key
    } // e.type
  }

  /* keyboard input */

  /*

  const bool* ks = SDL_GetKeyboardState(NULL);
  // moving players dir
  if (ks[SDL_SCANCODE_RIGHT])
    Game.Player.rotate(-rotspeed);
  if (ks[SDL_SCANCODE_LEFT])
    rotate(+rotspeed);

  // moving players position
  if (ks[SDL_SCANCODE_UP]) 
    move(movspeed);
  if (ks[SDL_SCANCODE_DOWN]) 
    move(-movspeed * mov_penalty);
  

  // shooting and menu stuff
  if (ks[SDL_SCANCODE_Z]) {
    righthand.update = 1;
  }
  if (ks[SDL_SCANCODE_X]) {
    righthand.tex_equipped = testing_weapon;
    righthand.num_frames = 1;
  }
  if (ks[SDL_SCANCODE_C]) {
    righthand.tex_equipped = righthand_tex;
    righthand.num_frames = 4;
  }
  */
}
