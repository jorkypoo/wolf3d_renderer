

#ifndef _OBJECTS_
#define _OBJECTS_

#include <list>
using namespace std;

#include <SDL3/SDL.h> // SDL_GetTicksNS() & type Uint64

#include "def.h"
#include "mathutils.h"


class Sprite;
class Entity;
class Object;

// global linked list of all objects (static & scripted/animated)
extern list<Object> OBJECTS;

class Sprite {
  public:
    Sprite();
    ~Sprite();

    void update_texture(const char* lookup_name); // search loaded sprites and updates object to use the queried texture
    void update_timer(void); // used internally

    unsigned get_num_frames();
    unsigned get_frame_num();
    Uint64   get_update_every();
    int      is_oneshot();

    // if timer is marked as a oneshot, play the animation
    void activate_oneshot_sprite();

  private:
    Color* texture;      // pointer to the texture to render - can be swapped out

    unsigned num_frames; // how many frames the current texture has
    unsigned frame_num;  // the frame the current sprite is on, starting at 0
    uint64 update_every; // how often the sprite should update its sprites in ms

    int oneshot;         // if the animation should only play once; i.e., a trap or weapon 
    int update;          // used internally if sprite is a one-shot animation

    uint64 timer_time;        // used internally for updating timers
    uint64 timer_last_update; // used internally for updating timers

    double sprite_dist;       // used internally for rendering
    unsigned sprite_order;    // used internally for rendering
    
};

#endif
