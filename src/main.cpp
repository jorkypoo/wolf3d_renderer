// #include <iostream>
// using namespace std;
#include "../inc/def.h"
#include "../inc/game.h"
#include "../inc/gameloop.h"


char WINDOWNAME[] = "Wolf3D in C++";

Player Player(0.7);
Game_Manager Game(Player);


int main(int argv, char** argc) {
  if(!Game.Init(WINDOWNAME, WINDOWX, WINDOWY, PIXELX, PIXELY))
    cout << "erm oh well\n";

  // images are to be loaded into the global texture buffer here

  while (Game.is_running()) {
    Input(&Game);
    // Update(&Game);
    Render(&Game);
    // Game.set_running_state(0);
    Game.calculate_fps();
  }

  return 0;
}
