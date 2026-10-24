#include <iostream>
using namespace std;

#include "../inc/def.h"
#include "../inc/game.h"
#include "../inc/gameloop.h"
#include "../inc/map.h"
#include "../inc/map_textures.h"
#include "../inc/entity.h"
#include "../inc/entity_textures.h"


char WINDOWNAME[] = "Wolf3D in C++";

Player player(0.7);
Game_Manager Game(&player);


int main(int argv, char** argc) {
  if(!Game.Init(WINDOWNAME, WINDOWX, WINDOWY, PIXELX, PIXELY))
    cout << "erm oh well\n";

  Game.player->set_spawn_point(3, 2);

  // images are to be loaded into the global texture buffer here
  Init_Global_Textures();
  Init_Global_Sprites();
  Init_Entities();
  Init_Map();
  // print_map();
  

  while (Game.is_running()) {
    Input(&Game);
    Update(&Game);
    Render(&Game);
    Game.calculate_fps();
  }

  // cleanup
  Free_Entities();
  Free_Textures();
  Free_Sprites();

  return 0;
}
