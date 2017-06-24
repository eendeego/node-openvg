#include "EGL/egl.h"
#include "GLES/gl.h"

#include "egl.h"
#include "bcm_host.h"

// TODO REMOVE
#define ARG_CHECKS

#include "node-common.h"
#include "argchecks.h"

using namespace v8;
using namespace node;

egl::state_t egl::State;
EGLConfig egl::Config;

extern void egl::InitBindings(napi_env env, napi_value target) {
  napi_property_descriptor descriptors[] = {
    DECLARE_NAPI_PROPERTY("getError", egl::GetError),
    DECLARE_NAPI_PROPERTY("swapBuffers", egl::SwapBuffers),
    DECLARE_NAPI_PROPERTY(
      "createPbufferFromClientBuffer",
      egl::CreatePbufferFromClientBuffer
    ),
    DECLARE_NAPI_PROPERTY("destroySurface", egl::DestroySurface),

    DECLARE_NAPI_PROPERTY("createContext", egl::CreateContext),
    DECLARE_NAPI_PROPERTY("destroyContext", egl::DestroyContext),
    DECLARE_NAPI_PROPERTY("makeCurrent", egl::MakeCurrent)
  };

  NAPI_CALL_RETURN_VOID(
    env,
    napi_define_properties(
      env,
      target,
      sizeof(descriptors) / sizeof(*descriptors),
      descriptors
    )
  );
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
    vc_dispmanx_element_add(dispman_update, dispman_display, 0 /*layer */ ,
                            &dst_rect, 0 /*src */ , &src_rect,
                            DISPMANX_PROTECTION_NONE,
                            0 /*alpha */ , 0 /*clamp */ ,
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


napi_value egl::GetError(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 0);
  CheckArgs0(env, info, getError, argc, args);

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, eglGetError(), &result));
  return result;
}


napi_value egl::SwapBuffers(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, swapBuffers, argc, args, external, external);

  void* display;
  NAPI_CALL(env, napi_get_value_external(env, args[0], &display));
  void* surface;
  NAPI_CALL(env, napi_get_value_external(env, args[1], &surface));

  EGLBoolean swapResult = eglSwapBuffers(display, surface);

  napi_value result;
  NAPI_CALL(env, napi_get_boolean(env, swapResult, &result));
  return result;
}

napi_value egl::CreatePbufferFromClientBuffer(napi_env env, napi_callback_info info) {
  // According to the spec (sec. 4.2.2 EGL Functions)
  // The buffer is a VGImage: "The VGImage to be targeted is cast to the
  // EGLClientBuffer type and passed as the buffer parameter."
  // So, check for a Number (as VGImages are checked on openvg.cc) and
  // cast to a EGLClientBuffer.

  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, CreatePbufferFromClientBuffer, argc, args, number);

  uint32_t clientBuffer;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &clientBuffer));

  EGLClientBuffer buffer = reinterpret_cast<EGLClientBuffer>(clientBuffer);

  static const EGLint attribute_list[] = {
    EGL_TEXTURE_FORMAT, EGL_TEXTURE_RGBA,
    EGL_TEXTURE_TARGET, EGL_TEXTURE_2D,
    EGL_MIPMAP_TEXTURE, EGL_FALSE,
    EGL_NONE
  };

  EGLSurface surface = eglCreatePbufferFromClientBuffer(
    State.display,
    EGL_OPENVG_IMAGE,
    buffer,
    egl::Config,
    attribute_list
  );

  napi_value result;
  NAPI_CALL(env, napi_create_external(env, surface, NULL, NULL, &result));
  return result;
}

napi_value egl::DestroySurface(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, destroySurface, argc, args, external);

  EGLSurface surface;
  NAPI_CALL(env, napi_get_value_external(env, args[0], &surface));

  EGLBoolean errorCode = eglDestroySurface(State.display, surface);

  napi_value result;
  NAPI_CALL(env, napi_get_boolean(env, errorCode, &result));
  return result;
}

napi_value egl::MakeCurrent(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, makeCurrent, argc, args, external, external);

  EGLSurface surface;
  NAPI_CALL(env, napi_get_value_external(env, args[0], &surface));
  EGLContext context;
  NAPI_CALL(env, napi_get_value_external(env, args[1], &context));

  // According to EGL 1.4 spec, 3.7.3, for OpenVG contexts, draw and read
  // surfaces must be the same
  EGLBoolean errorCode = eglMakeCurrent(
    State.display,
    surface,
    surface,
    context
  );

  napi_value result;
  NAPI_CALL(env, napi_get_boolean(env, errorCode, &result));
  return result;
}

napi_value egl::CreateContext(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  // No arg checks

  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  EGLContext shareContext = EGL_NO_CONTEXT;

  if (argc != 0) {
    NAPI_CALL(env, napi_get_value_external(env, args[0], &shareContext));
  }

  // According to EGL 1.4 spec, 3.7.3, for OpenVG contexts, draw and read
  // surfaces must be the same
  EGLContext context =
    eglCreateContext(State.display, egl::Config, shareContext, NULL);

  napi_value result;
  NAPI_CALL(env, napi_create_external(env, context, NULL, NULL, &result));
  return result;
}

napi_value egl::DestroyContext(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, destroyContext, argc, args, external);

  EGLContext context;
  NAPI_CALL(env, napi_get_value_external(env, args[0], &context));

  EGLBoolean destroyResult = eglDestroyContext(State.display, context);

  napi_value result;
  NAPI_CALL(env, napi_get_boolean(env, destroyResult, &result));
  return result;
}
