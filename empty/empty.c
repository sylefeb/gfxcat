// @sylefeb 2024-01, GPL v3
#include "framework.h"

void render()
{
  unsigned char *pixel = framework_pixels();
  for (int j=0;j<framework_height();++j) {
    for (int i=0;i<framework_width();++i) {
      pixel[0] = i;
      pixel[1] = j;
      pixel[2] = 0;
      pixel[3] = 0;
      pixel += 4;
    }
  }
}

int main(int argc,const char **argv)
{
  framework_loop(render);
  return 0;
}
