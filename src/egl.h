#ifndef NODE_OPENVG_EGL_H_
#define NODE_OPENVG_EGL_H_

#include <node.h>
#include <v8.h>
#include "EGL/egl.h"
#include "openvg.h"

using namespace v8;

namespace egl {
  void Init();
  void Finish();
}

#endif
