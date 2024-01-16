// @sylefeb 2024-01, GPL v3

#include "framework.h"

// ----------------------------------------------------------------------------

int time = 0;

void render()
{
  int rng = 31421;

  unsigned char *pixel = framework_pixels();
  for (int j=0 ; j<framework_height() ; ++j) {

    rng = ((rng<<5) ^ 6927) + (rng ^ j);
    rng = ((rng) ^ 31421) + (j);

    for (int i=0 ; i<framework_width() ; ++i) {
      int r = (((time<<(j&3)) + rng)&255);
      if ((i&255) == r) {
        int clr = (1+(j&3))<<6;
        if (clr > 255) clr = 255;
        pixel[0] = clr;
        pixel[1] = clr;
        pixel[2] = clr;
      } else {
        pixel[0] = 0;
        pixel[1] = 0;
        pixel[2] = 0;
      }
      pixel += 4;
    }
  }

  ++ time;
}

// ----------------------------------------------------------------------------

int main(int argc,char **argv)
{
  framework_loop(render);
  return 0;
}

// ----------------------------------------------------------------------------
