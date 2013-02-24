#include "EGL/egl.h"
#include "bcm_host.h"

namespace {
  struct initializer {
    initializer() {
      // bcm_host_init() must be called before anything else
      bcm_host_init();

      // Calling eglGetDisplay initializes EGL
      eglGetDisplay(EGL_DEFAULT_DISPLAY);
    }

    ~initializer() {
      bcm_host_deinit();
    }
  };
  static initializer i;
}
