/*
 * contains relevant functions for drawing pixels to a buffer
 * alongside image loading functions into a buffer
 *
 */

#ifndef _RENDERING_
#define _RENDERING_

#include "def.h"

#include <cstdint> // size_t
using namespace std;


// fill buffer at index with colour
inline void fill_point(Color* buff, size_t x, size_t y, Color c) {
  buff[PIXELX * y + x] = c;
}

// fill line from y1 - y2 with colour
inline void fill_vertline(Color* buff, size_t x, size_t y1, size_t y2, Color c) {
  for (int y = y1; y < y2; y++)
    fill_point(buff, x, y, c);
}


// load image data from path into buff, where buff is definitely of size w*h
// automatically resizes image if it does not match buff size
// the only image types supported are .png and .bmp
void load_image_to_buff(char* path, Color* buff, int w, int h);

// the main rendering loop, all inside one function
void render(void);

#endif
