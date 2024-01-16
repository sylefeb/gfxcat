// @sylefeb 2024-01, GPL v3

#pragma once

int            framework_width();
int            framework_height();
unsigned char *framework_pixels();
void           framework_loop(void (*f_render)());
