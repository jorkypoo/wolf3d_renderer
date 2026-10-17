/*
 * helps out with managing SDL and window shit
 *
 */

#ifndef _WINDOW_
#define _WINDOW_

#include <SDL3/SDL.h>
// #include <stdbool.h> // std is bitchmode for this
#include <iostream>
using namespace std;


// class to help me manage game/sdl shit
class Game_Manager {
  private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    int running;

    Uint64 delta;
    Uint64 fps;
    Uint64 frame_old;
    Uint64 frame_new;
    Uint64 frame_cnt;
    
  public:
    Game_Manager() {
      window   = nullptr;
      renderer = nullptr;
      texture  = nullptr;

      // set running to true once window and whatnot is set up
      running = 0;

      frame_old = 0;
      frame_new = 0;
      frame_cnt = 0;
    }

    ~Game_Manager() {
      SDL_DestroyTexture(texture);
      SDL_DestroyRenderer(renderer);
      SDL_DestroyWindow(window);
    }

    // returns 0 on failure & 1 on success
    int Init(char* window_name, int window_w, int window_h, int tex_w, int tex_h) {
      if (!SDL_Init(SDL_INIT_VIDEO)) {
        cout << "SDL_Init Failed";
        return 0;
      }

      window = SDL_CreateWindow(window_name, window_w, window_h, 0);
      if (!window) {
        cout << "Error: Failed to create window: %s\n", SDL_GetError();
        return 0;
      }

      renderer = SDL_CreateRenderer(window, NULL);
      if (!renderer) {
        cout << "Error: Failed to create renderer: %s\n", SDL_GetError();
        return 0;
      }
      
      SDL_SetRenderLogicalPresentation(renderer, window_w, window_h, SDL_LOGICAL_PRESENTATION_LETTERBOX);

      texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, tex_w, tex_h);
      if (texture) {
        cout << "Error: Failed to create game texture: %s\n", SDL_GetError();
        return 0;
      }
      
      SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

      running = 1;
      return 1;
    }

    void calculate_fps() {
      const unsigned long long NS_PER_SECOND = 1000 * 1000000;

      static Uint64 last_fps_update = 0;
      frame_old = frame_new;
      frame_new = SDL_GetTicksNS();
      delta = (double) (frame_new - frame_old) / NS_PER_SECOND;

      frame_cnt++;
      if (frame_old - last_fps_update >= NS_PER_SECOND) {
        fps = (double) frame_cnt * NS_PER_SECOND / (frame_old - last_fps_update);
        frame_cnt = 0;
        last_fps_update = frame_old;
      }
    }

    int is_running() { return running; }

    Uint64 get_fps() { return fps; }

    Uint64 get_delta() { return delta; }
};

#endif
