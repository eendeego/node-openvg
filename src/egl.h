#ifndef NODE_OPENVG_EGL_H_
#define NODE_OPENVG_EGL_H_

#include <node.h>
#include <v8.h>
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

extern void InitBindings(Handle<Object> target);

void Init();
void InitOpenGLES();
void Finish();

Handle<Value> GetError(const Arguments& args);
Handle<Value> SwapBuffers(const Arguments& args);
Handle<Value> CreatePbufferFromClientBuffer(const Arguments& args);
Handle<Value> DestroySurface(const Arguments& args);

Handle<Value> CreateContext(const Arguments& args);
Handle<Value> DestroyContext(const Arguments& args);
Handle<Value> MakeCurrent(const Arguments& args);

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
