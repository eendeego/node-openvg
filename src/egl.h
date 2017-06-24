#ifndef NODE_OPENVG_EGL_H_
#define NODE_OPENVG_EGL_H_

#include <v8.h>
#include <node.h>
#include <node_api.h>

#include "EGL/egl.h"

using namespace v8;

namespace egl {

struct state_t {
  EGLDisplay display;
  EGLContext context;
  EGLSurface surface;

  uint32_t screen_width;
  uint32_t screen_height;
};

extern state_t State;
extern EGLConfig Config;

extern void InitBindings(napi_env env, napi_value target);

void Init();
void InitOpenGLES();
void Finish();

napi_value GetError(napi_env env, napi_callback_info info);
napi_value SwapBuffers(napi_env env, napi_callback_info info);
napi_value CreatePbufferFromClientBuffer(napi_env env, napi_callback_info info);
napi_value DestroySurface(napi_env env, napi_callback_info info);

napi_value CreateContext(napi_env env, napi_callback_info info);
napi_value DestroyContext(napi_env env, napi_callback_info info);
napi_value MakeCurrent(napi_env env, napi_callback_info info);

}

#define CHECK_EGL_ERROR {\
    EGLint errorCode = eglGetError();\
    if(errorCode != EGL_SUCCESS) {\
      char buffer[100];\
      snprintf(&buffer[0], sizeof(buffer), "eglGetError: 0x%x", errorCode);\
      __assert_fail (buffer, __FILE__, __LINE__, __PRETTY_FUNCTION__);\
    }\
  }

#endif
