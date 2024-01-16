// @sylefeb 2024-01, GPL v3

#include <GLFW/glfw3.h>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

#include <stdio.h>

#define g_Width  320
#define g_Height 240

static unsigned char g_Pixels[g_Width*g_Height*4];

unsigned char *framework_pixels()
{
  return g_Pixels;
}

int framework_width()
{
  return g_Width;
}

int framework_height()
{
  return g_Height;
}

static void        (*g_RenderFunc)();
static GLFWwindow*   g_Window = NULL;

void framework_loop_step()
{
  int width, height;
  glfwGetFramebufferSize(g_Window, &width, &height);
  glViewport(0, 0, width, height);
  // call user render function
  g_RenderFunc();
  // update texture
  glTexSubImage2D( GL_TEXTURE_2D,0,0,0,
            framework_width(),framework_height(),
            GL_RGBA,GL_UNSIGNED_BYTE,
            framework_pixels());
  // draw frame
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
  glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 0.0f);
  glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
  glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 1.0f);
  glEnd();
  // swap
  glfwSwapBuffers(g_Window);
  glfwPollEvents();
}

void framework_loop(void (*f_render)())
{
  g_RenderFunc = f_render;
  // glfw window
  if (!glfwInit()) {
    fprintf(stderr,"ERROR: cannot initialize graphics\n");
    return;
  }
  glfwWindowHint(GLFW_DECORATED,GLFW_FALSE);
  g_Window = glfwCreateWindow(framework_width()*2,
                              framework_height()*2,
                              "GFXCat framework", NULL, NULL);
  glfwMakeContextCurrent(g_Window);
  // prepare texture
  GLuint fbTexture;
  glGenTextures(1,&fbTexture);
  glBindTexture(GL_TEXTURE_2D,fbTexture);
  glTexImage2D( GL_TEXTURE_2D,0, GL_RGBA,
                framework_width(),framework_height(),0,
                GL_RGBA,GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  // setup rendering
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glDisable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_2D);
  glColor3f(1.0f,1.0f,1.0f);
  // setup view
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  // enter main loop
#ifndef EMSCRIPTEN
  glfwSwapInterval(1);
  while (!glfwWindowShouldClose(g_Window)) {
    framework_loop_step();
  }
#else
  emscripten_set_main_loop(framework_loop_step, 0, 1);
#endif
  // terminate
  glfwDestroyWindow(g_Window);
  glfwTerminate();
}
