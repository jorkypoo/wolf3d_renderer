/*
 * header to provide:
 * the game structure & player,
 * texture structure (the raw image data),
 * the global gamemap & fogmaps/darkmaps, 
 * sprite list (animated & not - theyll use the same class),
 * npc/enemy list - theyre separate to sprites
 *
 * stuff like rendering and input & updates will draw from this during execution
 *
 */

#ifndef _WINDOW_
#define _WINDOW_

#include <SDL3/SDL.h>
// #include <stdbool.h> // std is bitchmode for this
#include <iostream>
using namespace std;

#include "def.h"
#include "mathutils.h"


// ill use this later
int Init_Game();


// player shall be a child of the game structure for easy access
// when passing it around
class Player;
class Game_Manager;

// global gamemap that is populated with each levels own configuration
// made up of tiles, each having their own attached textures & unique properties
// Tile GameMap;

// this shall be an unordered map - check later
extern Color Textures;

// same as before
extern Color Fog_Map;
extern Color Dark_Map;

// list of all sprites, animated or not - static objects around the gamemap
// Sprite Sprites;

// list of all npcs/enemies - they will need to be updated during the gameloop
// Entity Entities;


class Player {
  private:
    v2 pos;
    v2 dir;
    v2 plane;

    double collision_radius;

    // move penalty and stuff too!
  public:
    Player() {
      pos = {0,0};
      dir = {-1,0};
      plane = {0,0.7};

      collision_radius = 0.2;
    }
    
    Player(double fov) {
      pos = {0,0};
      dir = {-1,0};
      plane = {0,fov};

      collision_radius = 0.2;
    }

    void rotate(double rot) {
      const v2 od = dir;
      const v2 op = plane;
      dir.x = od.x * cos(rot) - od.y * sin(rot);
      dir.y = od.x * sin(rot) + od.y * cos(rot);
      plane.x = op.x * cos(rot) - op.y * sin(rot);
      plane.y = op.x * sin(rot) + op.y * cos(rot);
    }

    void move(double mov) {
      v2 tomov = { dir.x * mov, dir.y * mov };

      int next_x = (int) (pos.x + tomov.x + sign(tomov.x) * collision_radius);
      int cur_y  = (int) pos.y;

      // this function must have access to the global gamemap (which it will)
      // so it can check

      // if (GAMEMAP[cur_y * MAPX + next_x] == 0)
        pos.x += tomov.x;

      int cur_x  = (int) pos.x;
      int next_y = (int) (pos.y + tomov.y + sign(tomov.y) * collision_radius);

      // if (GAMEMAP[next_y * MAPX + cur_x] == 0)
        pos.y += tomov.y;
    }
};


// class to help me manage game/sdl shit
class Game_Manager {
  private:
    // game state variables
    int running;
    int borderless;

    // FPS variables
    Uint64 delta;
    Uint64 fps;
    Uint64 frame_old;
    Uint64 frame_new;
    Uint64 frame_cnt;
    
  public:
    // windowing variables
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    // attached game structures
    Player player;
    Color buffer[PIXELX * PIXELY];


    Game_Manager(Player p) {
      window   = nullptr;
      renderer = nullptr;
      texture  = nullptr;

      // set running to true once window and whatnot is set up
      running = 0;
      borderless = 0;

      frame_old = 0;
      frame_new = 0;
      frame_cnt = 0;

      player = p;
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
        cout << "Error: Failed to create window: " << SDL_GetError() << "\n";
        return 0;
      }

      renderer = SDL_CreateRenderer(window, NULL);
      if (!renderer) {
        cout << "Error: Failed to create renderer: " << SDL_GetError() << "\n";
        return 0;
      }
      
      SDL_SetRenderLogicalPresentation(renderer, window_w, window_h, SDL_LOGICAL_PRESENTATION_LETTERBOX);

      texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, tex_w, tex_h);
      if (!texture) {
        cout << "Error: Failed to create game texture: " << SDL_GetError() << "\n";
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

    int  is_running()             { return running; }
    void set_running_state(int s) { running = s; }

    int  is_borderless()              { return borderless; }
    void set_window_borderless(int s) { borderless = s; }

    Uint64 get_fps()         { return fps; }
    void   set_fps(Uint64 f) { fps = f; }

    Uint64 get_delta()         { return delta; }
    void   set_delta(Uint64 d) { delta = d; }
};

#endif
