#ifndef _ENTITY_
#define _ENTITY_

#include <list>
using namespace std;

#include <SDL3/SDL.h> // SDL_GetTicksNS() & type Uint64

#include "def.h"
#include "mathutils.h"

#include "param.h"

class Sprite;
class Entity;

// global linked list of all objects (static & scripted/animated)
// entities ai scripts & timers updated in update() via this list
// and their sprite rendered in render() too
extern list<Entity*> ENTITIES;

int Init_Entities(void);
void Free_Entities(void);


// unfortunately, you'll have to make one of these functions for derived classes
// from entity, as they must call the appropriate constructor
// i deem that okay and semi modular
int Add_Entity(const char* entity_lookup_name, const char* tex_lookup_name, v2 p);


class Entity {
  public:
    Entity(std::string path, std::string id, Sprite* s, v2 p, v2 d, double cr, double ir);
    ~Entity();
    
    Sprite* sprite;
    // Parameters param;
    // Defense defense;
    // Damage damage;

    virtual void update() { return; } // called every frame - for ai scripts, if any
    // virtual void on_collision() { return; } // called on collision with the player
    // virtual void on_interaction() { return; } // called on interaction with the player
    virtual void on_removal() { return; } // called when entity is removed - for loot drops

    // remove a sprite from the ENTITIES list and free it's data
    // this needs to integrate with a call deferred system
    void remove(void);

    void   set_pos(v2 p)  { pos = p; }
    v2     get_pos(void)  { return pos; }
    double get_posx(void) { return pos.x; }
    double get_posy(void) { return pos.y; }

    void   set_dir(v2 d)  { dir = d; }
    v2     get_dir(void)  { return dir; }
    double get_dirx(void) { return dir.x; }
    double get_diry(void) { return dir.y; }

    void   set_int_radius(double r) { interact_radius = r; }
    double get_int_radius() { return interact_radius; }
    void   set_col_radius(double r) { collision_radius = r; }
    double get_col_radius() { return collision_radius; }

    unsigned get_id() { return id; }
    
  private:
    v2 pos;
    v2 dir;
    double collision_radius;
    double interact_radius;
    unsigned id;
};



class Sprite {
  public:
    Sprite(Color* tex, unsigned nf, Uint64 ue, int os);
    ~Sprite();

    Color* texture; // pointer to the texture to render - can be swapped out

    void update_texture(const char* lookup_name); // search loaded sprites and updates object to use the queried texture
    Color* get_texture();
    void update_timer(void); // used internally

    unsigned get_num_frames();
    unsigned get_frame_num();
    Uint64   get_update_every();
    int      is_oneshot();

    // if timer is marked as a oneshot, play the animation
    void activate_oneshot_sprite();

    // used in rendering - the distance from the player
    void   set_sprite_dist(double d) { sprite_dist = d; }
    double get_sprite_dist(void)     { return sprite_dist; }

  private:
    unsigned num_frames; // how many frames the current texture has
    unsigned frame_num;  // the frame the current sprite is on, starting at 0
    Uint64 update_every; // how often the sprite should update its sprites in ms

    int oneshot;         // if the animation should only play once; i.e., a trap or weapon 
    int update;          // used internally if sprite is a one-shot animation

    Uint64 timer_time;        // used internally for updating timers
    Uint64 timer_last_update; // used internally for updating timers

    double sprite_dist;       // used internally for rendering
};

#endif
