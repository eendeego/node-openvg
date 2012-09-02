#include "EGL/egl.h"
#include "GLES/gl.h"
#include "openvg.h"
#include <assert.h>

#include "egl.h"

#include <stdio.h>

using namespace v8;

extern void
egl::Init() {
  EGLDisplay display;
  printf("Calling eglGetDisplay...\n");

  // get an EGL display connection
  display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  assert(display != EGL_NO_DISPLAY);

  printf("eglGetDisplay returned: %010p.\n", display);
}

extern void
egl::Finish() {
  // Nothing yet
}
