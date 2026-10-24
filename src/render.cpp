#include "../inc/gameloop.h"
#include "../inc/draw.h"
#include "../inc/map_textures.h"
#include "../inc/entity.h"
#include "../inc/map.h"

// macros now, and can later be put as settings
// no idea if these can be hotswitched
#define FLOOR_RENDERING
#define FOG_RENDERING
#define ENTITY_SPRITE_RENDERING
#define PRINT_FPS


inline Color add_fog_to_color(Color (*fgmp)[FOGLEVELS][256], Color input_color, unsigned fog_amount) {
  unsigned char* color_components = (unsigned char*) &input_color;
  
  color_components[2] = (fgmp[FM_RED]  [fog_amount][color_components[2]]); 
  color_components[1] = (fgmp[FM_GREEN][fog_amount][color_components[1]]);
  color_components[0] = (fgmp[FM_BLUE] [fog_amount][color_components[0]]);

  return input_color;
}

void init_rendering(Game_Manager* Game);
void  end_rendering(Game_Manager* Game);


void Render(Game_Manager* Game) {
  init_rendering(Game);

  double zbuff[PIXELX] = {0};

  #ifdef FLOOR_RENDERING

  /* floor casting */
  { // this block is cheeky but removes the textures below from scope after were done w them

  // grab these from the level like:
  // Game->level/dungeon.get_floor_/ceiling_texture()
  Color* floor_tex = TEXTURES["assets/colorstone.png"];  
  Color*  ceil_tex = TEXTURES["assets/wood.png"];
  // Color* tex_for_col = TEXTURES[tile_for_col.lookup_name];

  for(int y = 0; y < PIXELY; y++) {
    // rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
    v2 raydir0 = { Game->player->get_dirx() - Game->player->get_planex(), Game->player->get_diry() - Game->player->get_planey() };
    v2 raydir1 = { Game->player->get_dirx() + Game->player->get_planex(), Game->player->get_diry() + Game->player->get_planey() };

    // Current y position compared to the center of the screen (the horizon)
    int p = y - PIXELY / 2;

    // Vertical position of the camera.
    float posz = 0.5 * PIXELY;

    // Horizontal distance from the camera to the floor for the current row.
    // 0.5 is the z position exactly in the middle between floor and ceiling.
    float row_dist = posz / p;

    // calculate the real world step vector we have to add for each x (parallel to camera plane)
    // adding step by step avoids multiplications with a weight in the inner loop
    v2 floor_step = {
      row_dist * (raydir1.x - raydir0.x) / PIXELX,
      row_dist * (raydir1.y - raydir0.y) / PIXELX
    };

    // real world coordinates of the leftmost column. This will be updated as we step to the right.
    v2 floor = {
      Game->player->get_posx() + row_dist * raydir0.x,
      Game->player->get_posy() + row_dist * raydir0.y
    };

    #ifdef FOG_RENDERING
    // calculate fog levels for each column
    const float middle = ((float) PIXELY / 2);
    unsigned f_fog = ((1 - ((y / middle) - 1)) * FOGLEVELS);
    unsigned c_fog = (((PIXELY - y - 1) / middle) * FOGLEVELS);

    if (f_fog >= FOGLEVELS) f_fog = FOGLEVELS-1;
    if (c_fog >= FOGLEVELS) c_fog = FOGLEVELS-1;
    #endif

    for(int x = 0; x < PIXELX; ++x) {
      // the cell coord is simply got from the integer parts of floorX and floorY
      v2i cell = { (int) (floor.x), (int) (floor.y) };

      // get the texture coordinate from the fractional part
      int tx = (int) (TEXSIZE * (floor.x - cell.x)) & (TEXSIZE - 1);
      int ty = (int) (TEXSIZE * (floor.y - cell.y)) & (TEXSIZE - 1);

      floor.x += floor_step.x;
      floor.y += floor_step.y;

      // choose texture and draw the pixel
      Color c = 0;
      
      // floor
      c = floor_tex[TEXSIZE * ty + tx];
      #ifdef FOG_RENDERING
      c = add_fog_to_color(FOGMAP, c, f_fog);
      #endif
      fill_point(Game->buffer, x, y, c);

      //ceiling (symmetrical, at screenHeight - y - 1 instead of y)
      c = ceil_tex[TEXSIZE * ty + tx];
      #ifdef FOG_RENDERING
      c = add_fog_to_color(FOGMAP, c, c_fog);
      #endif
      fill_point(Game->buffer, x, PIXELY - y - 1, c);
    } // loop to fill buff
  }   // loop through every row in screen

  } // this extra block removes the floor & ceiling texture from scope

  #endif // FLOOR_RENDERING


  /* wall casting */
  for (int x = 0; x < PIXELX; x++) {
    // x coord that intercepts with the plane
    double camera_x = 2.0 * x / (double) PIXELX - 1.0;

    // ray direction for this column shot from player position
    v2 ray = {
      Game->player->get_dirx() + Game->player->get_planex() * camera_x,
      Game->player->get_diry() + Game->player->get_planey() * camera_x
    };

    // integer coords for where the player is on the map
    v2i map_pos = { (int) Game->player->get_posx(), (int) Game->player->get_posy() };

    v2 delta_dist = {
      fabs(ray.x) < 1e-30 ? 1e30 : fabs(1 / ray.x),
      fabs(ray.y) < 1e-30 ? 1e30 : fabs(1 / ray.y),
    };

    v2 side_dist = {
      (ray.x < 0 ? Game->player->get_posx() - map_pos.x : map_pos.x + 1.0 - Game->player->get_posx()) * delta_dist.x,
      (ray.y < 0 ? Game->player->get_posy() - map_pos.y : map_pos.y + 1.0 - Game->player->get_posy()) * delta_dist.y,
    };

    v2i step = { (int) sign(ray.x), (int) sign(ray.y) };

    int hit = 0; int side = 0;
    // int val = 0; 


    // perform DDA
    while (!hit) {
      if (side_dist.x < side_dist.y) {
        side_dist.x += delta_dist.x;
        map_pos.x += step.x;
        side = 0;
      } else {
        side_dist.y += delta_dist.y;
        map_pos.y += step.y;
        side = 1;
      }

      // check for hit logic goes here, whatever it is
      if (!GAMEMAP[MAPX * map_pos.y + map_pos.x].is_air) {
        hit = 1;
      }
    } 

    // perpendictular distance to the hit
    double perp_dist = 0;
    if (side == 0) perp_dist = (side_dist.x - delta_dist.x);
    else           perp_dist = (side_dist.y - delta_dist.y);

    // calculating the height of the wall
    int wall_height = (int) PIXELY / perp_dist;

    int wall_top = -wall_height / 2 + PIXELY / 2; 
    int wall_btm =  wall_height / 2 + PIXELY / 2;

    // cap within bounds of the screen buffer
    if (wall_top >= PIXELY) wall_top = PIXELY - 1;
    if (wall_top < 0) wall_top = 0;
    if (wall_btm >= PIXELY) wall_btm = PIXELY;
    if (wall_btm < 0) wall_btm = 0;


    /* ===== texture rendering ===== */

    // tile that has been detected this column
    // 
    // this structure is defined in map.h & contains
    // the texture hash map lookup name 
    Tile tile_for_col = GAMEMAP[MAPX * map_pos.y + map_pos.x];

    // instead of querying the hashmap every pixel, query it every column
    // grab the texture for the column for array lookups every pixel
    Color* tex_for_col = TEXTURES[tile_for_col.lookup_name];

    // exact x value of where the wall was hit
    /*double wall_x = (side == 0 ? player.pos.y : player.pos.x) + dist * (side == 0 ? ray.y : ray.x);*/
    double wall_x = 0;
    if (side == 0) wall_x = Game->player->get_posy() + perp_dist * ray.y;
    else           wall_x = Game->player->get_posx() + perp_dist * ray.x;
    wall_x -= floor(wall_x);
    
    // x coordinate on the texture
    int tex_x = (int) (wall_x * TEXSIZE);
    if (side == 0 && ray.x > 0) tex_x = TEXSIZE - tex_x - 1;
    if (side == 1 && ray.y < 0) tex_x = TEXSIZE - tex_x - 1;

    double tex_step = 1.0 * TEXSIZE / wall_height;
    double tex_pos = (wall_top - (double) PIXELY / 2 + (double) wall_height / 2) * tex_step;

    #ifdef FOG_RENDERING
    // fog added to colour is proportional to the distance to
    // the pixel - further away a block is, the greater the fog effect
    // fog_dist is kinda the cutoff dist where only fog is rendered
    unsigned fog_amt = (perp_dist / FOGDIST) * FOGLEVELS;
    if (fog_amt >= FOGLEVELS) fog_amt = FOGLEVELS-1;
    #endif

    // when texture mapping, the pixels must be drawn at once, as their
    // colours will differ throughout the column
    Color c = 0;
    for (int y = wall_top; y < wall_btm; y++) {
      int tex_y = (int) tex_pos & (TEXSIZE - 1);
      tex_pos += tex_step;

      // original colour from the texure
      c = tex_for_col[TEXSIZE * tex_y + tex_x];

      // darken colour on specific sides to add a shadow effect/depth
      if (side == 1) {
        //c = darken_colour(drkmap, c, 31); // not working right now
        const Color br = ((c & 0xFF00FF) * 0xAF) >> 8;
        const Color g  = ((c & 0x00FF00) * 0xAF) >> 8;
        c = 0xFF000000 | (br & 0xFF00FF) | (g & 0x00FF00);
      }

      #ifdef FOG_RENDERING
      c = add_fog_to_color(FOGMAP, c, fog_amt);
      #endif
      
      fill_point(Game->buffer, x, y, c);
    }
      
    // darken colours
    if (side == 1) {
      const Color br = ((c & 0xFF00FF) * 0xC0) >> 8;
      const Color g  = ((c & 0x00FF00) * 0xC0) >> 8;
      c = 0xFF000000 | (br & 0xFF00FF) | (g & 0x00FF00);
    }

    zbuff[x] = perp_dist;

    #ifndef FLOOR_RENDERING
    // collect the provided floor/ceiling colour from the current level
    // ill hardcode this for now
    Color full_ceil_color = 0xFF003FCF;
    Color full_floor_color = 0xFFCFCFCF;

    fill_vertline(Game->buffer, x, 0, wall_top, full_ceil_color);
    fill_vertline(Game->buffer, x, wall_btm, PIXELY, full_floor_color);
    #endif

  } /* for x in PIXELX - wall rendering */

  #ifdef ENTITY_SPRITE_RENDERING

  /* ===== entity rendering ===== */

  // basically the rendering function only cares about the sprite part of an entity
  // every entity has one and it can either be static or animated - 1 sprite frame or > 1
  // 
  // animated sprite's current sprite frame rendered is based on an offset:
  // frame_num * (twidth * wheight)^2 - this is used as a texture array lookup 
  //
  // if it is a static sprite, frame_num should ALWAYS be 0, so you always get the
  // first element of the texture buffer, not reaching out of bounds of it's array
  //
  // this is possible bc animation frames are ASSUMED to be laid out contiguously
  // in memory, so its just a matter of making sure that is always true
  //
  // each animated sprite can also contain a pointer to the beginning of their sprite
  // frames so you don't have to perform complex array lookups during rendering

  // for each object.entity within all objects (it'll be a linked list):
  // calculate distance from the player to the entity
  // sort the linked list based on which is furthest from player (measure how fast this is - it should be fast!)
  //  
  // for each object.sprite within all objects:
  // boring maths i'll copy paste
  // if FOG_RENDERING is defined: fog calculations
  // calculate the offset for the current sprite's sprite frame; see above
  //
  // (btw if a sprite moves, it's position shall be updated in update())
  //
  // (these values are calulated in the boring maths part)
  // for each x in the x space to draw:
  // for each y in the y space to draw:
  // grab the colour: an array lookup from the current sprite * offset
  // if the alpha channel is 0xFF:
  // if FOG_RENDERING is defined: add fog to colour grabbed
  // fill the point
  // 
  // easy enough!

  for (Entity* cur : ENTITIES) {
    double distx = Game->player->get_posx() - cur->get_posx();    
    double disty = Game->player->get_posy() - cur->get_posy();
    cur->sprite->set_sprite_dist(distx * distx + disty * disty);
  }

  // sort the sprites here based on their distance

  // begin looping through the linked list
  for (Entity* cur : ENTITIES) {

    v2 sprite = {
      cur->get_posx() - Game->player->get_posx(),
      cur->get_posy() - Game->player->get_posy()
    };

    double inv_det = 1.0 / ( (Game->player->get_planex() * Game->player->get_diry()) - \
                             (Game->player->get_dirx() * Game->player->get_planey()) );
    
     
    v2 transform = {
      inv_det * ( Game->player->get_diry() * sprite.x - Game->player->get_dirx() * sprite.y),
      inv_det * (-Game->player->get_planey() * sprite.x + Game->player->get_planex() * sprite.y)
    };

    // basically don't render the sprite if it is very close
    if (transform.y < 0.1) continue;

    int spriteScreenX = (int) (((float) PIXELX / 2) * (1 + transform.x / transform.y));

    int spriteHeight = abs((int)(PIXELY / (transform.y))); 
    int spriteWidth =  abs((int)(PIXELY / (transform.y)));

    v2i draw_start = {
      (-spriteWidth / 2) + spriteScreenX,
      (-spriteHeight / 2) + (PIXELY / 2)
    };

    // cap the drawing values to be in the bounds of the buffer
    if (draw_start.x < 0) draw_start.x = 0;
    if (draw_start.y < 0) draw_start.y = 0;

    v2i draw_end = {
      (spriteWidth / 2) + spriteScreenX,
      (spriteHeight / 2) + (PIXELY / 2)
    };

    // again, cap values within the bounds of the buffer
    if (draw_end.x >= PIXELX) draw_end.x = PIXELX - 1;
    if (draw_end.y >= PIXELY) draw_end.y = PIXELY - 1;
        
    #ifdef FOG_RENDERING
    unsigned fog_amt = (cur->sprite->get_sprite_dist() / OBJ_FOGDIST) * FOGLEVELS;
    if (fog_amt >= FOGLEVELS) fog_amt = FOGLEVELS-1;
    #endif

    unsigned offset = cur->sprite->get_frame_num() * SQ(TEXSIZE);

    // loop through every vertical stripe of the sprite on screen
    for (int stripe = draw_start.x; stripe < draw_end.x; stripe++) {
      // wtf are the magic numbers...
      int texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * TEXSIZE / spriteWidth) / 256;

      // if the sprite is in view
      if (stripe > 0 && stripe < PIXELX && transform.y < zbuff[stripe]) {

        // loop through every row on the screen for every column
        for (int y = draw_start.y; y < draw_end.y; y++) {

          int d = (y) * 256 - PIXELY * 128 + spriteHeight * 128; //256 and 128 factors to avoid floats
          int texY = ((d * TEXSIZE) / spriteHeight) / 256;
          Color color = cur->sprite->texture[offset + (TEXSIZE * texY + texX)]; //get current color from the texture

          // if the color to be drawn is full alpha
          if((color & 0xFF000000) == 0xFF000000) { 
            #ifdef FOG_RENDERING
            color = add_fog_to_color(FOGMAP, color, fog_amt);
            #endif
            fill_point(Game->buffer, stripe, y, color); 
          }
        }

      } //  if sprite is in view
    }   // for stripe in sprite on screen

  }
  
  #endif // ENTITY_SPRITE_RENDERING

  end_rendering(Game);
}


void init_rendering(Game_Manager* Game) {
  SDL_SetRenderDrawColor(Game->renderer, 0, 0, 0, 255);
  memset(Game->buffer, 0, sizeof(Game->buffer));
}

void end_rendering(Game_Manager* Game) {
  // no idea why last param has to be like that but it works lol
  SDL_UpdateTexture(Game->texture, NULL, Game->buffer, PIXELX * sizeof(unsigned)); 
  SDL_RenderTexture(Game->renderer, Game->texture, NULL, NULL);

  #ifdef PRINT_FPS
  char fps[64];
  snprintf(fps, 64, "%Lf", Game->get_fps());
  SDL_SetRenderDrawColor(Game->renderer, 0, 255, 0, 255);
  SDL_RenderDebugText(Game->renderer, 0.0f, 0.0f, fps);
  #endif

  SDL_RenderPresent(Game->renderer);
}
