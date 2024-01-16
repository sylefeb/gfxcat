// @sylefeb 2024-01, GPL v3

/*

This is a reference implementation of the dda algorithm and Menger sponge
in Silice vga_msponge.si (pipelined, racing-the-beam implementation)

I used this when prototyping the rendering algorithm, and it particular it
mimics the fixed point computations so I can check for precision issues.
This is obviously not meant for performance comparisons!

I also made a GPU version available on shadertoy:
https://www.shadertoy.com/view/DdB3zR

*/

#include "framework.h"
#include "sine_table.h"

// ----------------------------------------------------------------------------

int g_Frame = 0;

typedef unsigned int uint;

static inline int icos(int v)
{
  return sine_table[((v<<3)+1024)&4095]>>2;
}

static inline int isin(int v)
{
  return sine_table[(v<<3)&4095]>>2;
}

static inline int inv(int v)
{
  int abs_v = v<0 ? -v : v;
  return v == 0 ? ((1<<17)-1) : ((1<<18) / abs_v);
}

void render()
{
 const int tile[64] = {
   1,1,1,1,
   1,0,0,1,
   1,0,0,1,
   1,1,1,1,

   1,0,0,1,
   0,0,0,0,
   0,0,0,0,
   1,0,0,1,

   1,0,0,1,
   0,0,0,0,
   0,0,0,0,
   1,0,0,1,

   1,1,1,1,
   1,0,0,1,
   1,0,0,1,
   1,1,1,1
  };

  unsigned char *pixel = framework_pixels();
  for (int j=0 ; j<framework_height() ; ++j) {
    for (int i=0 ; i<framework_width() ; ++i) {

#define N_steps 256

      uint y=0u;         uint x=(0u);      uint wait=(0u);
      int  view_x=(0);    int view_y=(0);  int  view_z=(0);
      int  rot_x=(0);     int rot_y=(0);
      uint  inside=(0u);
      uint  clr=(0u);       uint  dist=(0u);
      uint  clr_r=(0u);     uint  clr_g=(0u);   uint  clr_b=(0u);
      int  cs0=(0);       int  ss0=(0);
      int  cs1=(0);       int  ss1=(0);
      int  r_x_delta=(0); int  r_z_delta=(0);

      view_x    = i - framework_width()/2;
      view_y    = j - framework_height()/2;
      view_z    = 384;

      inside    = 0;
      dist      = 255;
      clr       = 0;

      int frame = g_Frame;

      cs0     = icos(frame>>1);
      ss0     = isin(frame>>1);
      cs1     = icos((frame+(frame<<1))>>3);
      ss1     = isin((frame+(frame<<3))>>4);

      rot_x   = (view_x * cs1);
      rot_y   = (view_x * ss1);

      view_x  = rot_x - (view_y * ss1);
      view_y  = rot_y + (view_y * cs1);

      view_x = view_x >> 10;
      view_y = view_y >> 10;

      // a 'voxel' is 1<<12
      int vxsz = 1<<12;
      // 1<<11 is half a small 'voxel' (a cube in the smallest 4x4x4 struct)
      // level 0 is 1<<14 (4x4x4)
      // level 1 is 1<<16 (4x4x4)
      // level 2 is 1<<18 (4x4x4)
      // compute the ray direction (through rotations)
      int xcs = view_x * cs0;
      int xss = view_x * ss0;
      int zcs = view_z * cs0;
      int zss = view_z * ss0;
      r_x_delta = (xcs - zss);
      r_z_delta = (xss + zcs);
      // ray dir is (r_x_delta, view_y, r_z_delta)
      int rd_x  = r_x_delta>>10;
      int rd_y  = view_y;
      int rd_z  = r_z_delta>>10;
      // initialize voxel traversal
      // -> position
      int p_x   = (68<<11);
      int p_y   = (12<<11);
      int p_z   = (frame<<9);
      // -> start voxel
      int v_x   = p_x >> 12;
      int v_y   = p_y >> 12;
      int v_z   = p_z >> 12;
      // -> steps
      int s_x   = rd_x < 0 ? -1 : 1;
      int s_y   = rd_y < 0 ? -1 : 1;
      int s_z   = rd_z < 0 ? -1 : 1;
      // -> inv dot products
      int inv_x = inv(rd_x);
      int inv_y = inv(rd_y);
      int inv_z = inv(rd_z);
      // -> tmax
      int brd_x = (p_x - (v_x<<12)); // distance to border
      int brd_y = (p_y - (v_y<<12)); // distance to border
      int brd_z = (p_z - (v_z<<12)); // distance to border
      int tm_x  = ((rd_x < 0 ? (brd_x) : (vxsz - brd_x)) * inv_x)>>12;
      int tm_y  = ((rd_y < 0 ? (brd_y) : (vxsz - brd_y)) * inv_y)>>12;
      int tm_z  = ((rd_z < 0 ? (brd_z) : (vxsz - brd_z)) * inv_z)>>12;
      // -> delta
      int dt_x  = ((vxsz * inv_x)>>12)-1;
      int dt_y  = ((vxsz * inv_y)>>12)-1;
      int dt_z  = ((vxsz * inv_z)>>12)-1;

      int step;
      for (step=0 ; step<N_steps ; ++step)
      {
        int w_x = v_x; int w_y = v_y; int w_z = v_z;

        int  tex     = ((v_x)&63) ^ ((v_y)&63) ^ ((v_z)&63);
        int  vnum0   = (((w_z>>0)&3)<<4) | (((w_y>>0)&3)<<2) | (((w_x>>0)&3)<<0);
        int  vnum1   = (((w_z>>2)&3)<<4) | (((w_y>>2)&3)<<2) | (((w_x>>2)&3)<<0);
        int  vnum2   = (((w_z>>4)&3)<<4) | (((w_y>>4)&3)<<2) | (((w_x>>4)&3)<<0);
        if ((tile[vnum0] & tile[vnum1] & tile[vnum2]) != 0) {
          if (inside == 0u) {
            clr    = (uint)(tex);
            dist   = (uint)(step);
            inside = 1u;
            break;
          }
        }

        if (tm_x <= tm_y && tm_x <= tm_z) {
          // tm_x smallest
          v_x  = v_x  + s_x;
          tm_x = tm_x + dt_x;
        } else if (tm_y < tm_x && tm_y < tm_z) {
          // tm_y smallest
          v_y  = v_y  + s_y;
          tm_y = tm_y + dt_y;
        } else {
          // tm_z smallest
          v_z  = v_z  + s_z;
          tm_z = tm_z + dt_z;
        }

      }

      uint fog   = dist;
      uint light = (256u - dist);
      uint shade = light * clr;

      clr_r = (shade >> 7) + fog;
      clr_g = (shade >> 7) + fog;
      clr_b = (shade >> 8) + fog;

      pixel[0] = clr_r;
      pixel[1] = clr_g;
      pixel[2] = clr_b;

      pixel += 4;
    }
  }

  g_Frame += 4;
}

// ----------------------------------------------------------------------------

int main(int argc,char **argv)
{
  framework_loop(render);
  return 0;
}

// ----------------------------------------------------------------------------
