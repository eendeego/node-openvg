#include "EGL/egl.h"
#include "GLES/gl.h"

#include "egl.h"

#include "argchecks.h"

using namespace v8;
using namespace node;

egl::state_t egl::State;

extern void egl::InitBindings(Handle<Object> target) {
  NODE_SET_METHOD(target, "swapBuffers", egl::swapBuffers);
}

extern void egl::Init() {
  EGLBoolean result;
  int32_t success = 0;

  static const EGLint attribute_list[] = {
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_ALPHA_SIZE, 8,
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT & EGL_SWAP_BEHAVIOR_PRESERVED_BIT,
    EGL_NONE
  };

  EGLConfig config;
  EGLint num_config;

  VC_RECT_T dst_rect;
  VC_RECT_T src_rect;
  DISPMANX_ELEMENT_HANDLE_T dispman_element;
  DISPMANX_DISPLAY_HANDLE_T dispman_display;
  DISPMANX_UPDATE_HANDLE_T  dispman_update;

  static EGL_DISPMANX_WINDOW_T nativewindow;

  // get an EGL display connection
  State.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

  result = eglInitialize(State.display, NULL, NULL);
  assert(EGL_FALSE != result);

  // bind OpenVG API
  eglBindAPI(EGL_OPENVG_API);

  // get an appropriate EGL frame buffer configuration
  result = eglChooseConfig(State.display, attribute_list, &config, 1, &num_config);
  assert(EGL_FALSE != result);

  // create an EGL rendering context
  State.context = eglCreateContext(State.display, config, EGL_NO_CONTEXT, NULL);
  assert(State.context != EGL_NO_CONTEXT);

  // create an EGL window surface
  success = graphics_get_display_size(0 /* LCD */ , &State.screen_width,
                                      &State.screen_height);
  assert(success >= 0);

  dst_rect.x = 0;
  dst_rect.y = 0;
  dst_rect.width  = State.screen_width;
  dst_rect.height = State.screen_height;

  src_rect.x = 0;
  src_rect.y = 0;
  src_rect.width  = State.screen_width  << 16;
  src_rect.height = State.screen_height << 16;

  dispman_display = vc_dispmanx_display_open(0 /* LCD */ );
  dispman_update  = vc_dispmanx_update_start(0);

  dispman_element = vc_dispmanx_element_add(dispman_update, dispman_display, 0 /*layer */ , &dst_rect, 0 /*src */ ,
                                            &src_rect, DISPMANX_PROTECTION_NONE, 0 /*alpha */ , 0 /*clamp */ ,
                                            DISPMANX_NO_ROTATE /*transform */ );

  nativewindow.element = dispman_element;
  nativewindow.width   = State.screen_width;
  nativewindow.height  = State.screen_height;
  vc_dispmanx_update_submit_sync(dispman_update);

  State.surface = eglCreateWindowSurface(State.display, config, &nativewindow, NULL);
  assert(State.surface != EGL_NO_SURFACE);

  // connect the context to the surface
  result = eglMakeCurrent(State.display, State.surface, State.surface, State.context);
  assert(EGL_FALSE != result);

  // preserve color buffer when swapping
  eglSurfaceAttrib(State.display, State.surface, EGL_SWAP_BEHAVIOR, EGL_BUFFER_PRESERVED);

  //DAVE - Set up screen ratio
  glViewport(0, 0, (GLsizei) State.screen_width, (GLsizei) State.screen_height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  float ratio = (float)State.screen_width / (float)State.screen_height;
  glFrustumf(-ratio, ratio, -1.0f, 1.0f, 1.0f, 10.0f);
}

extern void egl::Finish() {
  glClear(GL_COLOR_BUFFER_BIT);
  eglSwapBuffers(State.display, State.surface);
  eglMakeCurrent(State.display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
  eglDestroySurface(State.display, State.surface);
  eglDestroyContext(State.display, State.context);
  eglTerminate(State.display);
}

Handle<Value> egl::swapBuffers(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(swapBuffers, display, External, surface, External);

  EGLDisplay display = (EGLDisplay) External::Unwrap(args[0]);
  EGLSurface surface = (EGLSurface) External::Unwrap(args[1]);

  eglSwapBuffers(display, surface);

  return Undefined();
}
