#include "nan.h"

#include "EGL/egl.h"
#include "GLES/gl.h"

#include "egl.h"
#include "bcm_host.h"

#include "argchecks.h"

using namespace v8;
using namespace node;

egl::state_t egl::State;
EGLConfig egl::Config;

extern void egl::InitBindings(Handle<Object> target) {
  NODE_SET_METHOD(target, "getError"      , egl::GetError);
  NODE_SET_METHOD(target, "swapBuffers"   , egl::SwapBuffers);
  NODE_SET_METHOD(target, "createPbufferFromClientBuffer",
                          egl::CreatePbufferFromClientBuffer);
  NODE_SET_METHOD(target, "destroySurface", egl::DestroySurface);

  NODE_SET_METHOD(target, "createContext" , egl::CreateContext);
  NODE_SET_METHOD(target, "destroyContext", egl::DestroyContext);
  NODE_SET_METHOD(target, "makeCurrent"   , egl::MakeCurrent);
}

extern void egl::Init() {
  EGLBoolean result;
  int32_t success = 0;

  static const EGLint attribute_list[] = {
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_ALPHA_SIZE, 8,
    EGL_ALPHA_MASK_SIZE, 8,
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT & EGL_SWAP_BEHAVIOR_PRESERVED_BIT,
    EGL_NONE
  };

  EGLint num_config;

  VC_RECT_T dst_rect;
  VC_RECT_T src_rect;
  DISPMANX_ELEMENT_HANDLE_T dispman_element;
  DISPMANX_DISPLAY_HANDLE_T dispman_display;
  DISPMANX_UPDATE_HANDLE_T  dispman_update;

  static EGL_DISPMANX_WINDOW_T nativewindow;

  static VC_DISPMANX_ALPHA_T alpha = {
    DISPMANX_FLAGS_ALPHA_FROM_SOURCE,
    255, 0
  };

  // bcm_host_init() must be called before anything else
  bcm_host_init();

  // get an EGL display connection
  State.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

  result = eglInitialize(State.display, NULL, NULL);
  assert(EGL_FALSE != result);

  // bind OpenVG API
  eglBindAPI(EGL_OPENVG_API);

  // get an appropriate EGL frame buffer configuration
  result = eglChooseConfig(State.display, attribute_list,
                           &egl::Config, 1, &num_config);
  assert(EGL_FALSE != result);

  // create an EGL rendering context
  State.context =
    eglCreateContext(State.display, egl::Config, EGL_NO_CONTEXT, NULL);
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

  dispman_element =
    vc_dispmanx_element_add(dispman_update, dispman_display, 1 /*layer */ ,
                            &dst_rect, 0 /*src */ , &src_rect,
                            DISPMANX_PROTECTION_NONE,
                            &alpha /*alpha */ , 0 /*clamp */ ,
                            DISPMANX_NO_ROTATE /*transform */);

  nativewindow.element = dispman_element;
  nativewindow.width   = State.screen_width;
  nativewindow.height  = State.screen_height;
  vc_dispmanx_update_submit_sync(dispman_update);

  State.surface =
    eglCreateWindowSurface(State.display, egl::Config, &nativewindow, NULL);
  assert(State.surface != EGL_NO_SURFACE);

  // connect the context to the surface
  result =
    eglMakeCurrent(State.display, State.surface, State.surface, State.context);
  assert(EGL_FALSE != result);

  // preserve color buffer when swapping
  eglSurfaceAttrib(State.display, State.surface,
                   EGL_SWAP_BEHAVIOR, EGL_BUFFER_PRESERVED);
}

// Code from https://github.com/ajstarks/openvg/blob/master/oglinit.c doesn't
// seem necessary.
extern void egl::InitOpenGLES() {
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

#ifdef __VIDEOCORE__
  bcm_host_deinit();
#endif
}


NAN_METHOD(egl::GetError) {
  NanScope();

  CheckArgs0(getError);

  NanReturnValue(NanNew<Integer>(eglGetError()));
}


NAN_METHOD(egl::SwapBuffers) {
  NanScope();

  CheckArgs1(swapBuffers, surface, External);

  EGLSurface surface = (EGLSurface) External::Cast(*args[0])->Value();

  EGLBoolean result = eglSwapBuffers(State.display, surface);

  NanReturnValue(NanNew<Boolean>(result));
}

NAN_METHOD(egl::CreatePbufferFromClientBuffer) {
  NanScope();

  // According to the spec (sec. 4.2.2 EGL Functions)
  // The buffer is a VGImage: "The VGImage to be targeted is cast to the
  // EGLClientBuffer type and passed as the buffer parameter."
  // So, check for a Number (as VGImages are checked on openvg.cc) and
  // cast to a EGLClientBuffer.

  CheckArgs1(CreatePbufferFromClientBuffer, vgImage, Number);

  EGLClientBuffer buffer =
    reinterpret_cast<EGLClientBuffer>(args[0]->Uint32Value());

  static const EGLint attribute_list[] = {
    EGL_TEXTURE_FORMAT, EGL_TEXTURE_RGBA,
    EGL_TEXTURE_TARGET, EGL_TEXTURE_2D,
    EGL_MIPMAP_TEXTURE, EGL_FALSE,
    EGL_NONE
  };

  EGLSurface surface =
    eglCreatePbufferFromClientBuffer(State.display,
                                     EGL_OPENVG_IMAGE,
                                     buffer,
                                     egl::Config,
                                     attribute_list);

  NanReturnValue(NanNew<External>(surface));
}

NAN_METHOD(egl::DestroySurface) {
  NanScope();

  CheckArgs1(destroySurface, surface, External);

  EGLSurface surface = (EGLSurface) External::Cast(*args[0])->Value();

  EGLBoolean result = eglDestroySurface(State.display, surface);

  NanReturnValue(NanNew<Boolean>(result));
}

NAN_METHOD(egl::MakeCurrent) {
  NanScope();

  CheckArgs2(makeCurrent, surface, External, context, External);

  EGLSurface surface = (EGLSurface) External::Cast(*args[0])->Value();
  EGLContext context = (EGLContext) External::Cast(*args[1])->Value();

  // According to EGL 1.4 spec, 3.7.3, for OpenVG contexts, draw and read
  // surfaces must be the same
  EGLBoolean result = eglMakeCurrent(State.display, surface, surface, context);

  NanReturnValue(NanNew<Boolean>(result));
}

NAN_METHOD(egl::CreateContext) {
  NanScope();

  // No arg checks

  EGLContext shareContext = args.Length() == 0 ?
    EGL_NO_CONTEXT :
    (EGLContext) External::Cast(*args[0])->Value();

  // According to EGL 1.4 spec, 3.7.3, for OpenVG contexts, draw and read
  // surfaces must be the same
  EGLContext result =
    eglCreateContext(State.display, egl::Config, shareContext, NULL);

  NanReturnValue(NanNew<External>(result));
}

NAN_METHOD(egl::DestroyContext) {
  NanScope();

  CheckArgs1(destroyContext, context, External);

  EGLContext context = (EGLContext) External::Cast(*args[0])->Value();

  EGLBoolean result = eglDestroyContext(State.display, context);

  NanReturnValue(NanNew<Boolean>(result));
}
