/*
 * a header containing the functions thatll be used in the main game loop
 * - collect the players input & update the player instance
 * - iterate through all npcs & whatnot and update based on their script
 * - render the players current view and fill the rendering buffer
 * 
 */

#ifndef _GAMELOOP_
#define _GAMELOOP_

#include "game.h"

void Input(Game_Manager Game);
void Update(Game_Manager Game);
void Render(Game_Manager Game);

#endif
