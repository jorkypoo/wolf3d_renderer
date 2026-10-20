/*
 * header for drawing pixels to the screen - used in rendering 
 *
 */

#ifndef _DRAWING_
#define _DRAWING_

#include "def.h"

#include <cstdint> // size_t
using namespace std;

// fill buffer at index with colour
inline void fill_point(Color* buff, size_t x, size_t y, Color c) {
  buff[PIXELX * y + x] = c;
}

// fill line from y1 - y2 with colour
inline void fill_vertline(Color* buff, size_t x, size_t y1, size_t y2, Color c) {
  for (size_t y = y1; y < y2; y++)
    fill_point(buff, x, y, c);
}

// draw texture of size w*h to buffer at x,y - assumes buff is large enough
inline void draw_texture(Color* buff, size_t bw, size_t x, size_t y, Color* tex, size_t w, size_t h) {
  size_t ox = x;
  for (size_t ty = 0; y <= h-1; ty++, y++, x=ox)
  for (size_t tx = 0; x <= w-1; tx++, x++)
    if ((tex[w * ty + tx] & 0xFF000000) == 0xFF000000)
      buff[bw * y + x] = tex[w * ty + tx];
}

#endif
