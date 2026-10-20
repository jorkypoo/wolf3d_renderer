/*
 * contains functions relevant to loading images from disk into texture buffer
 *
 */

#ifndef _IMAGE_LOAD_
#define _IMAGE_LOAD_

#include "def.h"

#include <cstdint> // size_t
using namespace std;


// load image data from path into buff, where buff is definitely of size w*h
// automatically resizes image if it does not match buff size
// the only image types supported are .png and .bmp
void load_image_to_buff(char* path, Color* buff, int w, int h);

#endif
