/*
 * everything relating to textures - tiles, sprites, enemies and weapons
 * also contains fogmaps which are applied within the rendering function
 *
 */

#ifndef _TEXTURES_
#define _TEXTURES_

#include "image_load.h"
#include "def.h"
#define sq(n) (n*n)

#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

// global texture buffer
extern unordered_map<string, Color*> TEXTURES;


int  Init_Textures(void);
void Free_Textures(void);

#endif
