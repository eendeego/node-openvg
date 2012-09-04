#ifndef NODE_OPENVG_H_
#define NODE_OPENVG_H_

#include <node.h>
#include <v8.h>
#include "EGL/egl.h"

using namespace v8;

namespace openvg {
  static Handle<Value> StartUp(const Arguments& args);
  static Handle<Value> Shutdown(const Arguments& args);

  static Handle<Value> GetError(const Arguments& args);

  static Handle<Value> Flush(const Arguments& args);
  static Handle<Value> Finish(const Arguments& args);

  /* Getters and Setters */
  static Handle<Value> SetF(const Arguments& args);
  static Handle<Value> SetI(const Arguments& args);
  static Handle<Value> SetFV(const Arguments& args);
  static Handle<Value> SetIV(const Arguments& args);

  static Handle<Value> GetF(const Arguments& args);
  static Handle<Value> GetI(const Arguments& args);
  static Handle<Value> GetVectorSize(const Arguments& args);
  static Handle<Value> GetFV(const Arguments& args);
  static Handle<Value> GetIV(const Arguments& args);

  static Handle<Value> SetParameterF(const Arguments& args);
  static Handle<Value> SetParameterI(const Arguments& args);
  static Handle<Value> SetParameterFV(const Arguments& args);
  static Handle<Value> SetParameterIV(const Arguments& args);

  static Handle<Value> GetParameterF(const Arguments& args);
  static Handle<Value> GetParameterI(const Arguments& args);
  static Handle<Value> GetParameterVectorSize(const Arguments& args);
  static Handle<Value> GetParameterFV(const Arguments& args);
  static Handle<Value> GetParameterIV(const Arguments& args);


  static Handle<Value> Start(const Arguments& args);
  static Handle<Value> End(const Arguments& args);
  static Handle<Value> Rect(const Arguments& args);
  static Handle<Value> Fill(const Arguments& args);
  static Handle<Value> Ellipse(const Arguments& args);
  static Handle<Value> TextMiddle(const Arguments& args);
}

#endif
