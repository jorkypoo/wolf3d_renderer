#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_JPEG
#define STBI_NO_PSD
#define STBI_NO_TGA
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_PIC
#include "../inc/stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../inc/stb_image_resize2.h"

#include "../inc/render.h"


void provide_missing_texture(unsigned* buff, int w, int h) {
  // yes those are the exact colours when u dont install cs soucre lol
  for (int x = 0; x < w; x++)
  for (int y = 0; y < h; y++) {
    if (x < w/2 && y < h/2)   buff[h * y + x] = 0xFFFF00DC;
    if (x > w/2 && y < h/2)   buff[h * y + x] = 0xFF010001;
    if (x <= w/2 && y >= h/2) buff[h * y + x] = 0xFF010001;
    if (x >= w/2 && y >= h/2) buff[h * y + x] = 0xFFFF00DC;
  }
}

// basically, stb_image only allows for image dimensions of whatever the
// signed integer max is, so don't worry that it's not size_t, shhh
void load_image_to_buff(char* path, unsigned* buff, int w, int h) {
  // query the image to get it's dimensions and see if it can be
  // processed by stbi; if not, provide a default texture, lol
  int qw = 0; int qh = 0; int qc = 0;
  int ok = stbi_info(path, &qw, &qh, &qc);
  // printf("%s queried size from memory: %d*%d @ %d comps\n",path,qx,qy,qc);

  // image cannot be opened by stbi
  if (!ok) { 
    fprintf(stderr, "warning: could not init %s at (%d*%d): %s\n", path,w,h,stbi_failure_reason());
    provide_missing_texture(buff, w, h);
    return;
  }

  // i don't know how this would happen, but warn regardless
  // it may be a monotone image without RGB values. pass it through regardless 
  if (qc < 3) fprintf(stderr, "image file %s has less than 3 components;\
                      cannot guarantee successful loading into buffer\n", path);
  
  // 4 steps (components) per pixel: r, g, b and finally a
  const int step = 4;
  int n = 4;

  // load image at actual image size so it doesn't look fucked
  unsigned char* data = NULL;
  data = (qw == w || qh == h) ? stbi_load(path, &w, &h, &n, step)
                              : stbi_load(path, &qw,&qh,&n, step);

  if (!data) { // this should not happen, but best to be safe
    fprintf(stderr, "error: failed to open %s (%d*%d): %s",path,w,h,stbi_failure_reason());
    provide_missing_texture(buff, w, h);
    return;
  }

  // if the image required resizing to fit the specified buffer, resize it
  // and free the previously allocated image loaded above
  unsigned char* resize;
  if (qw != w || qh != h) {
    resize = stbir_resize_uint8_linear(data,qw,qh,0,0,w,h,0,STBIR_RGBA);
    if (!resize) {
      fprintf(stderr, "failed to resize image %s from %d*%d to specified %d*%d",path,qw,qh,w,h);
      provide_missing_texture(buff, w, h);
      return;
    }
    free(data);
    data = resize;
  }
  
  // collect all the comps in data, transform into an unsigned int
  // and pop into the buffer - assuming its large enough
  unsigned colour = 0;
  for (int i = 0, a = 0; i < w*h*step; i += step, a++) {
    colour = data[i+3]<<24 | data[i+0]<<16 | data[i+1]<<8 | data[i+2];
    buff[a] = colour;
  }

  free(data);
}
