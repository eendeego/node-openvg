#ifndef NODE_OPENVG_H_
#define NODE_OPENVG_H_

#include <node.h>
#include <v8.h>
#include "EGL/egl.h"

using namespace v8;

namespace openvg {
  static Handle<Value> Init(const Arguments& args);
  static Handle<Value> Finish(const Arguments& args);
}

#endif
