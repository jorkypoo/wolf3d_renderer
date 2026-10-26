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

#include "map.h"
#include "param.h"


// ill use this later
int Init_Game();


// player shall be a child of the game structure for easy access
// when passing it around
class Player;
class Game_Manager;


class Player {
  private:
    v2 pos;
    v2 dir;
    v2 plane;
    double collision_radius;

  public:
    Parameters param;

    Player();
    ~Player() = default;
    Player(double fov); 

    void set_spawn_point(double x, double y);
    
    double get_posx() { return pos.x; }
    double get_posy() { return pos.y; }
    v2 get_pos()      { return pos; }
    double get_dirx() { return dir.x; }
    double get_diry() { return dir.y; }
    v2 get_dir()      { return dir; }
    double get_planex() { return plane.x; }
    double get_planey() { return plane.y; }
    v2 get_plane()      { return plane; }
    double get_collision_radius() { return collision_radius; }

    void rotate(double rot);
    void move(double mov);

};


// class to help me manage game/sdl shit
class Game_Manager {
  private:
    // game state variables
    int running;
    int borderless;

    // FPS variables
    long double delta;
    long double fps;

    Uint64 frame_old;
    Uint64 frame_new;
    Uint64 frame_cnt;
    
  public:
    // windowing variables
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    // attached game structures
    Player* player;
    Color buffer[PIXELX * PIXELY];


    Game_Manager(Player* p);
    ~Game_Manager();

    // returns 0 on failure & 1 on success
    int Init(const char* window_name, int window_w, int window_h, int tex_w, int tex_h);
    void calculate_fps();

    int  is_running()                 { return running; }
    void set_running_state(int s)     { running = s; }
    int  is_borderless()              { return borderless; }
    void set_window_borderless(int s) { borderless = s; }
    long double get_fps()             { return fps; }
    void set_fps(Uint64 f)            { fps = f; }
    long double get_delta()           { return delta; }
    void set_delta(Uint64 d)          { delta = d; }
};

#endif
