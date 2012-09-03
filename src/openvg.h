#ifndef NODE_OPENVG_H_
#define NODE_OPENVG_H_

#include <node.h>
#include <v8.h>
#include "EGL/egl.h"

using namespace v8;

namespace openvg {
  static Handle<Value> Init(const Arguments& args);
  static Handle<Value> Finish(const Arguments& args);

  static Handle<Value> Start(const Arguments& args);
  static Handle<Value> End(const Arguments& args);
  static Handle<Value> Rect(const Arguments& args);
  static Handle<Value> Background(const Arguments& args);
  static Handle<Value> Fill(const Arguments& args);
  static Handle<Value> Ellipse(const Arguments& args);
  static Handle<Value> Circle(const Arguments& args);
  static Handle<Value> TextMiddle(const Arguments& args);
}

#endif
