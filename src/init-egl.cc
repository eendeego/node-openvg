#include "EGL/egl.h"

namespace {
  struct initializer {
    initializer() {
#if defined(BCM_HOST_H) // FIXME Find something less brittle than BCM_HOST_H
      // bcm_host_init() must be called before anything else
      bcm_host_init();
#endif
      // Calling eglGetDisplay initializes EGL
      eglGetDisplay(EGL_DEFAULT_DISPLAY);
    }

    ~initializer() {
#if defined(BCM_HOST_H) // FIXME Find something less brittle than BCM_HOST_H
      bcm_host_deinit();
#endif
    }
  };
  static initializer i;
}
