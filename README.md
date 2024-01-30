# A catalog of my old-school GFX effects
*Demoparty time!*

![thumbnails](thumbnails.png)

These are fun and compact graphics effects that I explored as I was developing
[Silice](https://github.com/sylefeb/Silice/tree/master/projects). Most were
implemented directly in hardware, or as risc-v demos for
[my little CPU cores and SOCs](https://github.com/sylefeb/Silice/tree/master/projects/ice-v).

This repository contains C versions using a minimalistic framework. They should
be very easy to port to run on your own SOC, to port to your own language, or to
revisit in hardware (which was the most fun for me :) ).

I list below each effect. __You can try them from your browser__, as they can
all be compiled with [Emscripten](https://emscripten.org/).

I'll keep expending the list, and if there's interest may do a few deep dives to
explain some of them.

I also include in the list pointers to the hardware or risc-v implementations
in the Silice repo.

## List

- [`Run doomfire`](https://htmlpreview.github.io/?https://github.com/sylefeb/gfxcat/blob/main/runtime/gfxcat_doomfire.html) [see code](doomfire/doomfire.c)
  - Silice version is a demo of how to easily integrate a risc-v core in a design, [see detailed README here](https://github.com/sylefeb/Silice/tree/draft/projects/kbfcrabe).


- [`Run msponge`](https://htmlpreview.github.io/?https://github.com/sylefeb/gfxcat/blob/main/runtime/gfxcat_msponge.html) [see code](msponge/msponge.c)
  - Silice version is a 300+ stages pipeline racing the beam! No CPU, no framebuffer, [see design](https://github.com/sylefeb/Silice/blob/master/projects/vga_demo/vga_msponge.si) and [demo](https://twitter.com/sylefeb/status/1588215408531742721).

- [`Run lotus`](https://htmlpreview.github.io/?https://github.com/sylefeb/gfxcat/blob/main/runtime/gfxcat_lotus.html) [see code](lotus/lotus.c).
  - Silice version runs on a dual-core RV32I risc-v that fits on the icestick! (1.3K LUT), [dual-core code](https://github.com/sylefeb/Silice/blob/master/projects/ice-v/src/dual_lotus.c) and [demo](https://twitter.com/sylefeb/status/1423772410025762825).

- [`Run starfield`](https://htmlpreview.github.io/?https://github.com/sylefeb/gfxcat/blob/main/runtime/gfxcat_starfield.html) [see code](starfield/starfield.c)
  - Another risc-v demo for a tiny SOC, see [here](https://github.com/sylefeb/Silice/blob/master/projects/ice-v/src/starfield.c). This one is easy to port in hardware, racing the beam, see the [snow effect here](https://github.com/sylefeb/Silice/blob/8842f221b155fdd255e8e517853c5c80e94db991/projects/vga_text_buffer/vga_text_buffer.si#L183).

- [`Run raytrace`](https://htmlpreview.github.io/?https://github.com/sylefeb/gfxcat/blob/main/runtime/gfxcat_raytrace.html) [see code](raytrace/raytrace.c).
  - A simple (and likely totally incorrect) fixed point 32 bits raytracer! I used this one for a different hardware project, yet to be released, the important point being it does not need floats.

- [`Run julia`](https://htmlpreview.github.io/?https://github.com/sylefeb/gfxcat/blob/main/runtime/gfxcat_julia.html) [see code](julia/julia.c)
  - Another dual-core risc-v  demo for the icestick core, see [dual-core code](https://github.com/sylefeb/Silice/blob/master/projects/ice-v/src/dual_fractal.c) and [demo](https://twitter.com/sylefeb/status/1435528911685918723). A fun aspect of that one is that it emulates mul using a lookup table! Yup.

- [`Run flyover3d`](https://htmlpreview.github.io/?https://github.com/sylefeb/gfxcat/blob/main/runtime/gfxcat_flyover3d.html) [see code](flyover3d/flyover3d.c).
  - A classic two plane perspective with one division per line. This one races the beam in hardware, see [here](https://github.com/sylefeb/Silice/blob/master/projects/vga_demo/vga_flyover3d.si). The division for the next line [happens in parallel](https://github.com/sylefeb/Silice/blob/8842f221b155fdd255e8e517853c5c80e94db991/projects/vga_demo/vga_flyover3d.si#L78) to rendering the current line.

- [`Run tunnel`](https://htmlpreview.github.io/?https://github.com/sylefeb/gfxcat/blob/main/runtime/gfxcat_tunnel.html) [see code](tunnel/tunnel.c)
  - This one was great fun. It is a demo for a more advanced SOC using external QPSRAM, with dual cores capable to independently access fast/slow memory. The video ram is in BRAM and limited in size, hence the 1 bit per pixel output! Because there's ton of slower memory in the SOC however, the demo can use large lookup tables. This was a classic for tunnel effects. See [design detailed README here](https://github.com/sylefeb/Silice/blob/master/projects/ice-v/IceVDualFermata.md) and [demo here](https://twitter.com/sylefeb/status/1509525004978860049)

## Credits

I made all of these effects based on my (not so fresh) memories from my demoscene
days, but all are classics and have been shown many times before! Source code
includes links to clever tricks by others used in each of the demos
(like the neat [fractal mul trick](http://cowlark.com/2018-05-26-bogomandel/index.html) and [integer sqrt](https://github.com/chmike/fpsqrt/blob/master/fpsqrt.c)).