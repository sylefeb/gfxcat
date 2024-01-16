// @sylefeb 2024-01, GPL v3
#include "framework.h"

int pos_u  = 0;
int pos_v  = 0;

//_ fireplace
unsigned char tbl[320*240 + 8/*padding*/];

//_ 32 xRGB entries ('fire' palette)
unsigned char pal[] = {
  0x04,0x04,0x04,0, 0x1c,0x04,0x04,0, 0x2c,0x0c,0x04,0, 0x44,0x0c,0x04,0,
  0x54,0x14,0x04,0, 0x64,0x1c,0x04,0, 0x74,0x1c,0x04,0, 0x9c,0x2c,0x04,0,
  0xac,0x3c,0x04,0, 0xbc,0x44,0x04,0, 0xc4,0x44,0x04,0, 0xdc,0x54,0x04,0,
  0xdc,0x54,0x04,0, 0xd4,0x5c,0x04,0, 0xd4,0x5c,0x04,0, 0xd4,0x64,0x0c,0,
  0xcc,0x74,0x0c,0, 0xcc,0x7c,0x0c,0, 0xcc,0x84,0x14,0, 0xc4,0x84,0x14,0,
  0xc4,0x94,0x1c,0, 0xbc,0x9c,0x1c,0, 0xbc,0x9c,0x1c,0, 0xbc,0xa4,0x24,0,
  0xbc,0xa4,0x24,0, 0xbc,0xac,0x2c,0, 0xb4,0xac,0x2c,0, 0xb4,0xb4,0x2c,0,
  0xcc,0xcc,0x6c,0, 0xdc,0xdc,0x9c,0, 0xec,0xec,0xc4,0, 0xef,0xef,0xef,0};

//_ draws fire onto the LCD
void draw_fire()
{
  int *ipixel = (int*)framework_pixels(); // framebuffer as one int per pixel.
  unsigned char *fire = tbl + 320*240 - 1;
  for (int v=0;v<240;++v) {
    for (int u=0;u<320;u++) {
      // palette lookup
      -- fire;
      int  clr = ((*fire)>>2)&31;
      int *ptr = (int*)(pal) + clr;
      // write in framebuffer
      *ipixel = *ptr;
      // next
      ++ ipixel;
    }
  }
}

int rng  = 31421;  // random number generator seed

//_ update the fire framebuffer
void update_fire()
{
  // move up
  unsigned char *below   = tbl;
  unsigned char *current = tbl + 320;
  for ( ; current < tbl + 240*320 ; ++current ) {
    int clr = 0;
    if ((*below) > 1) {
      clr = (*below)-(rng&1);
    }
    rng = (rng<<3) + ((rng>>3) ^ (int)current);
    *(current + (rng&3)) = clr; // NOTE: table padding avoids
                                //       out of bounds access
    ++ below;
  }
  // keep the heat on
  unsigned char *ptr = tbl;
  for (int v=0;v<320;v++) {
    rng = (rng<<5) + ((rng>>5) ^ (int)ptr);
    (*ptr++) = 120 + (rng&7);
  }
}

// render and update the fire
void render()
{
  draw_fire();
  update_fire();
}

// entry point
int main(int argc,const char **argv)
{
  // render loop
  framework_loop(render);
  return 0;
}
