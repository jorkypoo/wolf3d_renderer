#include "../inc/gameloop.h"

#include <cstring>

void Render(Game_Manager* Game) {
  std::memset(Game->buffer, 0, sizeof(Game->buffer));

  // no idea why last param has to be like that but it works lol
  SDL_UpdateTexture(Game->texture, NULL, Game->buffer, PIXELX * sizeof(unsigned)); 
  SDL_RenderTexture(Game->renderer, Game->texture, NULL, NULL);

  /* char fps[64];
  snprintf(fps, 64, "%lu", Game.get_fps());
  SDL_SetRenderDrawColor(Game.renderer, 0, 255, 0, 255);
  SDL_RenderDebugText(Game.renderer, 0.0f, 0.0f, fps); */

  SDL_RenderPresent(Game->renderer);
}
