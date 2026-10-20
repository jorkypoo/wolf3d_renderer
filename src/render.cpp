#include "../inc/gameloop.h"
#include "../inc/draw.h"

#include "../inc/textures.h"
#include "../inc/map.h"

void init_rendering(Game_Manager* Game);
void  end_rendering(Game_Manager* Game);

void Render(Game_Manager* Game) {
  init_rendering(Game);

  /* wall casting */
  for (int x = 0; x < PIXELX; x++) {
    // x coord that intercepts with the plane
    double camera_x = 2.0 * x / (double) PIXELX - 1.0;

    // ray direction for this column shot from player position
    v2 ray = {
      Game->player.get_dirx() + Game->player.get_planex() * camera_x,
      Game->player.get_diry() + Game->player.get_planey() * camera_x
    };

    // integer coords for where the player is on the map
    v2i map_pos = { (int) Game->player.get_posx(), (int) Game->player.get_posy() };

    v2 delta_dist = {
      fabs(ray.x) < 1e-30 ? 1e30 : fabs(1 / ray.x),
      fabs(ray.y) < 1e-30 ? 1e30 : fabs(1 / ray.y),
    };

    v2 side_dist = {
      (ray.x < 0 ? Game->player.get_posx() - map_pos.x : map_pos.x + 1.0 - Game->player.get_posx()) * delta_dist.x,
      (ray.y < 0 ? Game->player.get_posy() - map_pos.y : map_pos.y + 1.0 - Game->player.get_posy()) * delta_dist.y,
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
    Tile tile_for_col = {0, 0};

    // used as an index for the textures array - check the GAMEMAP
    tile_for_col = GAMEMAP[MAPX * map_pos.y + map_pos.x];

    // exact x value of where the wall was hit
    /*double wall_x = (side == 0 ? player.pos.y : player.pos.x) + dist * (side == 0 ? ray.y : ray.x);*/
    double wall_x = 0;
    if (side == 0) wall_x = Game->player.get_posy() + perp_dist * ray.y;
    else           wall_x = Game->player.get_posx() + perp_dist * ray.x;
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
    unsigned fog_amt = (perp_dist / FOGDIST) * FOG_LEVELS;
    if (fog_amt >= FOG_LEVELS) fog_amt = FOG_LEVELS-1;
    if (fog_amt < 0) fog_amt = 0;
    #endif

    // when texture mapping, the pixels must be drawn at once, as their
    // colours will differ throughout the column
    Color c = 0;
    for (int y = wall_top; y < wall_btm; y++) {
      int tex_y = (int) tex_pos & (TEXSIZE - 1);
      tex_pos += tex_step;

      // original colour from the texure
      c = TEXTURES[tile_for_col.lookup_name][TEXSIZE * tex_y + tex_x];

      // darken colour on specific sides to add a shadow effect/depth
      if (side == 1) {
        //c = darken_colour(drkmap, c, 31); // not working right now
        const Color br = ((c & 0xFF00FF) * 0xAF) >> 8;
        const Color g  = ((c & 0x00FF00) * 0xAF) >> 8;
        c = 0xFF000000 | (br & 0xFF00FF) | (g & 0x00FF00);
      }

      #ifdef FOG_RENDERING
      c = add_fog_to_color(fogmap, c, fog_amt);
      #endif
      
      fill_point(Game->buffer, x, y, c);
    }
      
    // darken colours
    if (side == 1) {
      const Color br = ((c & 0xFF00FF) * 0xC0) >> 8;
      const Color g  = ((c & 0x00FF00) * 0xC0) >> 8;
      c = 0xFF000000 | (br & 0xFF00FF) | (g & 0x00FF00);
    }

    // zbuff[x] = perp_dist;

    fill_vertline(Game->buffer, x, 0, wall_top, 0xFF003FCF);
    fill_vertline(Game->buffer, x, wall_btm, PIXELY, 0xFFCFCFCF);

  } /* for x in PIXELX - wall rendering */

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

  // /*
  char fps[64];
  snprintf(fps, 64, "%lu", Game->get_fps());
  SDL_SetRenderDrawColor(Game->renderer, 0, 255, 0, 255);
  SDL_RenderDebugText(Game->renderer, 0.0f, 0.0f, fps);
  // */

  SDL_RenderPresent(Game->renderer);
}
