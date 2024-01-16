// MIT license, see LICENSE_MIT in Silice repo root
// @sylefeb 2021
// https://github.com/sylefeb/Silice

// @sylefeb 2024-01, gfxcat port

#include "framework.h"

#include "tunnel.h" // see pre_tunnel.cc, run it first (Silice repo)

// Bayer 8x8 matrix, each row repeated 4 times for efficiency when drawing 32 pixel blocks
int bayer_8x8[8*8*4] = { // non const, ends up in RAM
  0, 32, 8, 40, 2, 34, 10, 42, 0, 32, 8, 40, 2, 34, 10, 42, 0, 32, 8, 40, 2, 34, 10, 42, 0, 32, 8, 40, 2, 34, 10, 42,
  48, 16, 56, 24, 50, 18, 58, 26, 48, 16, 56, 24, 50, 18, 58, 26, 48, 16, 56, 24, 50, 18, 58, 26, 48, 16, 56, 24, 50, 18, 58, 26,
  12, 44, 4, 36, 14, 46, 6, 38, 12, 44, 4, 36, 14, 46, 6, 38, 12, 44, 4, 36, 14, 46, 6, 38, 12, 44, 4, 36, 14, 46, 6, 38,
  60, 28, 52, 20, 62, 30, 54, 22, 60, 28, 52, 20, 62, 30, 54, 22, 60, 28, 52, 20, 62, 30, 54, 22, 60, 28, 52, 20, 62, 30, 54, 22,
  3, 35, 11, 43, 1, 33, 9, 41, 3, 35, 11, 43, 1, 33, 9, 41, 3, 35, 11, 43, 1, 33, 9, 41, 3, 35, 11, 43, 1, 33, 9, 41,
  51, 19, 59, 27, 49, 17, 57, 25, 51, 19, 59, 27, 49, 17, 57, 25, 51, 19, 59, 27, 49, 17, 57, 25, 51, 19, 59, 27, 49, 17, 57, 25,
  15, 47, 7, 39, 13, 45, 5, 37, 15, 47, 7, 39, 13, 45, 5, 37, 15, 47, 7, 39, 13, 45, 5, 37, 15, 47, 7, 39, 13, 45, 5, 37,
  63, 31, 55, 23, 61, 29, 53, 21, 63, 31, 55, 23, 61, 29, 53, 21, 63, 31, 55, 23, 61, 29, 53, 21, 63, 31, 55, 23, 61, 29, 53, 21,
};

unsigned int adv = 0;
int opacity = 1; int opacity_dir = 1;
int overlay = 0;
unsigned int shadow[10];
unsigned int bayer_binary[8];

void draw_tunnel()
{
  // fill pointers
  int *VRAM                       = (int*)framework_pixels();
  const unsigned int *tunnel_ptr  = tunnel;
  const unsigned int *overlay_ptr;
  // text overlay choice
  switch (overlay) {
    case  1: overlay_ptr = overlay3; break;
    case  2: overlay_ptr = overlay4; break;
    case  3: overlay_ptr = overlay2; break;
    default: overlay_ptr = overlay1; break;
  }
  // reset shadow
  for (int i=0 ; i < 10 ; ++i) { shadow[i] = 0; }
  // bayer matrix for overlay transparency
  {
    const unsigned int *bayer_ptr = bayer_8x8;
    for (int j=0 ; j < 8 ; ++j) {
      unsigned int mask = 0;
      for (int i=0 ; i < 32 ; ++i) {
        mask |= opacity > *(bayer_ptr++) ? (1<<i) : 0;
      }
      bayer_binary[j] = mask;
    }
  }
  // update opacity
  opacity += opacity_dir;
  if (opacity > 150) {
    opacity     = 64;
    opacity_dir = -2;
  } else if (opacity <= 0) {
    opacity     = 0;
    opacity_dir = 2;
    overlay     = overlay + 1;
    if (overlay == 4) {
      overlay = 0;
    }
  }

  for (int j=0 ; j < 200 ; j++) {
    for (int i=0 ; i < 10 ; ++i) {
      unsigned int *bayer_ptr      = bayer_8x8 + ((j&7)<<5);
      unsigned int pixels = 0;
      unsigned int pix    = 1;
      unsigned int t,d,uv0,uv1,drk,ba0,ba1,cl0,cl1;

#define TUNNEL_TWO_PIX(shade) \
        /* table content for 2 pixels */ \
        t   = *tunnel_ptr++; \
        uv0 = t;           \
        uv1 = (t>>16);     \
        if (shade) { \
          d   = (uv0 & 127);   /* dist */\
          drk = d > 63 ? (d-63) : 0; /* darkening*/\
        } \
        ba0 = *(bayer_ptr++);\
        ba1 = *(bayer_ptr++);\
        cl0 = texture[(uv0 + adv)&16383];\
        cl1 = texture[(uv1 + adv)&16383];\
        pixels = pixels | (cl0 > ba0 + drk ? pix : 0);\
        pix  <<= 1;\
        pixels = pixels | (cl1 > ba1 + drk ? pix : 0);\
        pix  <<= 1;

      TUNNEL_TWO_PIX(1); TUNNEL_TWO_PIX(0); TUNNEL_TWO_PIX(0); TUNNEL_TWO_PIX(0);
      TUNNEL_TWO_PIX(1); TUNNEL_TWO_PIX(0); TUNNEL_TWO_PIX(0); TUNNEL_TWO_PIX(0);
      TUNNEL_TWO_PIX(1); TUNNEL_TWO_PIX(0); TUNNEL_TWO_PIX(0); TUNNEL_TWO_PIX(0);
      TUNNEL_TWO_PIX(1); TUNNEL_TWO_PIX(0); TUNNEL_TWO_PIX(0); TUNNEL_TWO_PIX(0);

      unsigned int overlay = *(overlay_ptr++);
      unsigned int mask    = bayer_binary[j&7];
      for (int b=0;b<32;++b) {
        unsigned char *pix = (unsigned char *)VRAM;
        pix[0] = pix[1] = pix[2] = ((pixels & ~(shadow[i]&mask)) | (overlay&mask)) & (1<<b) ? 255 : 0;
        ++ VRAM;
      }
      shadow[i]            = overlay;
    }
  }
  // advance to next frame
  adv += (2 + (1<<7));
}

int main(int argc,const char **argv)
{
  framework_loop(draw_tunnel);
  return 0;
}
