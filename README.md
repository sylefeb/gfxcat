# A catalog of my old-school GFX effects
*Demoparty time!*

![thumbnails](thumbnails.png)

These are fun graphics effects that I explored as I was developing [Silice](https://github.com/sylefeb/Silice/tree/master/projects). Most were implemented directly in hardware, or as risc-v demos for [my little CPU cores and SOCs](https://github.com/sylefeb/Silice/tree/master/projects/ice-v).

This repository contains C version using a minimalistic framework. They should be very easy to port to run on your own SOC, to port to your own language, or to revisit in hardware (which was the most fun for me :) ).

I list below each effect. __You can try them from your browser__, as they can all be compiled with Emscripten.

I'll keep expending the list, and if there's interest may do a few deep dives to explain some of them.

## List

- [`Run doomfire`](runtime/gfxcat_doomfire.html) [see code](doomfire/doomfire.c)

- [`Run msponge`](runtime/gfxcat_msponge.html) [see code](msponge/msponge.c)

- [`Run lotus`](runtime/gfxcat_lotus.html) [see code](lotus/lotus.c)

- [`Run starfield`](runtime/gfxcat_starfield.html) [see code](starfield/starfield.c)

- [`Run raytrace`](runtime/gfxcat_raytrace.html) [see code](raytrace/raytrace.c)

- [`Run julia`](runtime/gfxcat_julia.html) [see code](julia/julia.c)

- [`Run flyover3d`](runtime/gfxcat_flyover3d.html) [see code](flyover3d/flyover3d.c)

## Credits

I made all of these effects based on my (not so fresh) memories for my demo party days, but all are classics and have been shown many times before!