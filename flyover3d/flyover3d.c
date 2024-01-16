// @sylefeb 2024-01, GPL v3
#include "framework.h"

int pos_u  = 0;
int pos_v  = 0;

void render()
{
  int maxv     = 22000;
  int inv_y    = 0;
  int offs_y   = 0;
  int u        = 0;
  int v        = 0;
  int lum      = 0;
  int on_floor = 0;

  unsigned char *pixel = framework_pixels();
  for (int pix_y=0;pix_y<framework_height();++pix_y) {

    if (pix_y < framework_height()/2) {
      offs_y = framework_height()/2 + 32 - pix_y;
      on_floor = 0;
    } else {
      offs_y = pix_y - (framework_height()/2 - 32);
      on_floor = 1;
    }

    if (offs_y >= 32 + 3)
    {

      inv_y = maxv / offs_y;

      lum = 255 - (inv_y>>1);
      if (lum > 255) {
        lum = 255;
      } else if (lum < 0) {
        lum = 0;
      }

      for (int pix_x=0;pix_x<framework_width();++pix_x) {

        u = (pos_u + ((pix_x - framework_width()/2) * inv_y)) >> 8;
        v =  pos_v + inv_y;

        pixel[0] = pixel[1] = pixel[2] = 0;

        if ( ((u>>5)&1) ^ ((v>>5)&1) ) {
          pixel[0] = lum;
          pixel[1] = lum;
          pixel[2] = lum;
        } else {
          if (on_floor) {
            pixel[1] = lum;
          } else {
            pixel[2] = lum;
          }
        }

        pixel += 4;
      }

    } else {
      pixel += framework_width()<<2;
    }
  }

  // prepare next frame
  pos_u = pos_u + 1024;
  pos_v = pos_v +    3;

}

int main(int argc,const char **argv)
{
  framework_loop(render);
  return 0;
}
