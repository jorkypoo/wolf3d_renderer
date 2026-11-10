/*
 * contains the base Entity class: any object - static or otherwise -
 * that may exist in the game map and be rendered
 *
 * also contains classes derived from entity, such as item drops,
 * enemy ai with custom update functions() and others
 *
 */

#ifndef _ENTITY_
#define _ENTITY_

#include <list>
#include <vector>
// #include <initializer_list> // item drops
#include <SDL3/SDL.h> // SDL_GetTicksNS() & type Uint64
#include "game.h"
#include "def.h"
#include "mathutils.h"
#include "param.h"
#include "item.h"
#include "entity_textures.h"

class Sprite;
class Entity;

// global linked list of all objects (static & scripted/animated)
// entities ai scripts & timers updated in update() via this list
// and their sprite rendered in render() too
extern std::list<Entity*> ENTITIES;

int Init_Entities(void);
void Free_Entities(void);
void Deferred_Remove_Entities(void);

// unfortunately, you'll have to make one of these functions for derived classes
// from entity, as they must call the appropriate constructor
// i deem that okay and semi modular
//
// add entity requires parameters, defense & attack values, and more, hence the
// json file path
int add_entity(std::string json_path, std::string json_id, std::string tex_lookup_name, v2 p);
int add_itemdrop(std::vector<Item*> is, v2 p);

// =============================
// ===== class definitions =====
// =============================

class Entity {
  public:
    Entity(std::string json_path, std::string json_id, Sprite* s, v2 p, v2 d, double cr, double ir);
    virtual ~Entity();

    Sprite* sprite;
    Parameters param;
    Defense defense;
    Damage damage;

    virtual void update(const Game_Manager* game) { return; } // called every frame - for ai scripts, if any
    virtual void on_collision(const Game_Manager* game) { return; } // called on collision with the player
    virtual void on_interaction(const Game_Manager* game) { return; } // called on interaction with the player
    virtual void on_removal() { return; } // called when entity is removed - for loot drops

    // remove a sprite from the ENTITIES list and free it's data
    // defers the removal to the end of the frame to avoid errors removing mid-frame
    void remove(void);
    int  set_for_removal(void); // for use in Deferred_Remove_Entities()

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
    int to_remove; // flags entity to be removed from the game at the end of the frame
    v2 pos;
    v2 dir;
    double collision_radius;
    double interact_radius;
    unsigned id;
};

class Item_Drop : public Entity {
  public:
    Item_Drop(std::string path, std::string id, Sprite* s, std::vector<Item*> itms, v2 p):
    Entity(path, id, s, p, {0,0}, 0.0, 0.5) {
      for (Item* i : itms)
        if (i != nullptr) items.push_back(i);
    }
    ~Item_Drop();

    virtual void update(const Game_Manager* game);
    virtual void on_interaction(const Game_Manager* game);

    void add_items(std::vector<Item*> drops);
    std::vector<Item*>& get_items();

  private:
    std::vector<Item*> items;
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
