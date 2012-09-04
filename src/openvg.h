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

  /* Matrix Manipulation */
  static Handle<Value> LoadIdentity(const Arguments& args);
  static Handle<Value> LoadMatrix(const Arguments& args);
  static Handle<Value> GetMatrix(const Arguments& args);
  static Handle<Value> MultMatrix(const Arguments& args);
  static Handle<Value> Translate(const Arguments& args);
  static Handle<Value> Scale(const Arguments& args);
  static Handle<Value> Shear(const Arguments& args);
  static Handle<Value> Rotate(const Arguments& args);

  /* Masking and Clearing */
  static Handle<Value> Mask(const Arguments& args);
  static Handle<Value> RenderToMask(const Arguments& args);
  static Handle<Value> CreateMaskLayer(const Arguments& args);
  static Handle<Value> DestroyMaskLayer(const Arguments& args);
  static Handle<Value> FillMaskLayer(const Arguments& args);
  static Handle<Value> CopyMask(const Arguments& args);
  static Handle<Value> Clear(const Arguments& args);

  static Handle<Value> Start(const Arguments& args);
  static Handle<Value> End(const Arguments& args);
  static Handle<Value> Rect(const Arguments& args);
  static Handle<Value> Fill(const Arguments& args);
  static Handle<Value> Ellipse(const Arguments& args);
  static Handle<Value> TextMiddle(const Arguments& args);
}

#endif
