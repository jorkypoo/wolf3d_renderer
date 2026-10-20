#include <iostream>
using namespace std;
#include "../inc/def.h"
#include "../inc/game.h"
#include "../inc/gameloop.h"
#include "../inc/map.h"
#include "../inc/textures.h"


char WINDOWNAME[] = "Wolf3D in C++";

Player player(0.7);
Game_Manager Game(&player);


int main(int argv, char** argc) {
  if(!Game.Init(WINDOWNAME, WINDOWX, WINDOWY, PIXELX, PIXELY))
    cout << "erm oh well\n";

  Game.player->set_spawn_point(3, 2);

  // images are to be loaded into the global texture buffer here
  Init_Textures();
  Init_Map();
  // print_map();
  

  while (Game.is_running()) {
    Input(&Game);
    // Update(&Game);
    Render(&Game);
    // cout << Game.player.get_posx() << "," << Game.player.get_posy() << "\t";
    // cout << Game.player.get_dirx() << "," << Game.player.get_diry() << "\t";
    // Game.set_running_state(0);
    Game.calculate_fps();
  }

  return 0;
}
