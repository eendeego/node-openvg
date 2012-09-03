#define BUILDING_NODE_EXTENSION

#include "VG/openvg.h"
#include "VG/vgu.h"
#include <v8.h>

#include "openvg.h"
#include "egl.h"

using namespace node;
using namespace v8;

extern "C" void
init(Handle<Object> target)
{
  NODE_SET_METHOD(target, "startUp"        , openvg::StartUp);
  NODE_SET_METHOD(target, "shutdown"       , openvg::Shutdown);

  NODE_SET_METHOD(target, "start"          , openvg::Start);
  NODE_SET_METHOD(target, "end"            , openvg::End);

  NODE_SET_METHOD(target, "rect"           , openvg::Rect);
  NODE_SET_METHOD(target, "fill"           , openvg::Fill);
  NODE_SET_METHOD(target, "ellipse"        , openvg::Ellipse);
  NODE_SET_METHOD(target, "textMiddle"     , openvg::TextMiddle);
}

#define CHECK_VG_ERROR {\
    VGErrorCode errorCode = vgGetError();\
    if(errorCode != VG_NO_ERROR) {\
      char buffer[100];\
      snprintf(&buffer[0], sizeof(buffer), "vgGetError: 0x%04x", errorCode);\
      __assert_fail (buffer, __FILE__, __LINE__, __PRETTY_FUNCTION__);\
    }\
  }

// newPath creates path data
VGPath newPath() {
  return vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F,
                      1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
}

// RGBA fills a color vectors from a RGBA quad.
void RGBA(unsigned int r, unsigned int g, unsigned int b, VGfloat a, VGfloat color[4]) {
  if (r > 255) { r = 0; }
  if (g > 255) { g = 0; }
  if (b > 255) { b = 0; }
  if (a < 0.0 || a > 1.0) { a = 1.0; }

  color[0] = (VGfloat) r / 255.0f;
  color[1] = (VGfloat) g / 255.0f;
  color[2] = (VGfloat) b / 255.0f;
  color[3] = a;
}

// setFill sets the fill color
void setFill(VGfloat color[4]) {
  VGPaint fillPaint = vgCreatePaint();
  vgSetParameteri(fillPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
  vgSetParameterfv(fillPaint, VG_PAINT_COLOR, 4, color);
  vgSetPaint(fillPaint, VG_FILL_PATH);
  vgDestroyPaint(fillPaint);
}

// setStroke sets the stroke color
void setStroke(VGfloat color[4]) {
  VGPaint strokePaint = vgCreatePaint();
  vgSetParameteri(strokePaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
  vgSetParameterfv(strokePaint, VG_PAINT_COLOR, 4, color);
  vgSetPaint(strokePaint, VG_STROKE_PATH);
  vgDestroyPaint(strokePaint);
}

// strokeWidth sets the stroke width
void strokeWidth(VGfloat width) {
  vgSetf(VG_STROKE_LINE_WIDTH, width);
  vgSeti(VG_STROKE_CAP_STYLE, VG_CAP_BUTT);
  vgSeti(VG_STROKE_JOIN_STYLE, VG_JOIN_MITER);
}

// Rect makes a rectangle at the specified location and dimensions
void rect(VGfloat x, VGfloat y, VGfloat w, VGfloat h) {
  VGPath path = newPath();
  vguRect(path, x, y, w, h);
  vgDrawPath(path, VG_FILL_PATH | VG_STROKE_PATH);
  vgDestroyPath(path);
}

// Ellipse makes an ellipse at the specified location and dimensions
void ellipse(VGfloat x, VGfloat y, VGfloat w, VGfloat h) {
  VGPath path = newPath();
  vguEllipse(path, x, y, w, h);
  vgDrawPath(path, VG_FILL_PATH | VG_STROKE_PATH);
  vgDestroyPath(path);
}

Handle<Value> openvg::StartUp(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsObject())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected StartUp(screen)")));
  }

  egl::Init();
  CHECK_VG_ERROR;

  Local<Object> screen = args[0].As<Object>();
  screen->Set(String::NewSymbol("width" ), Integer::New(egl::State.screen_width));
  screen->Set(String::NewSymbol("height"), Integer::New(egl::State.screen_height));

  return Undefined();
}

Handle<Value> openvg::Shutdown(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 0)) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected Shutdown()")));
  }

  egl::Finish();

  return Undefined();
}


Handle<Value> openvg::Start(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 0)) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected Start()")));
  }

  uint32_t width, height;
  width  = egl::State.screen_width;
  height = egl::State.screen_height;

  VGfloat color[4] = { 255, 255, 255, 1 };
  vgSetfv(VG_CLEAR_COLOR, 4, color);
  vgClear(0, 0, width, height);
  color[0] = 0, color[1] = 0, color[2] = 0;
  setFill(color);
  setStroke(color);
  strokeWidth(0);
  vgLoadIdentity();

  return Undefined();
}

Handle<Value> openvg::End(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 0)) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected End()")));
  }

  CHECK_VG_ERROR;
  egl::swapBuffers(egl::State.display, egl::State.surface);
  assert(eglGetError() == EGL_SUCCESS);

  return Undefined();
}

Handle<Value> openvg::Rect(const Arguments& args) {
  if (!(args.Length() == 4 && args[0]->IsNumber() &&
        args[1]->IsNumber() && args[2]->IsNumber() &&
        args[3]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected Rect(x, y, w, h)")));
  }

  VGfloat x = (VGfloat) args[0]->NumberValue();
  VGfloat y = (VGfloat) args[1]->NumberValue();
  VGfloat w = (VGfloat) args[2]->NumberValue();
  VGfloat h = (VGfloat) args[3]->NumberValue();

  rect(x, y, w, h);

  return Undefined();
}

Handle<Value> openvg::Fill(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 4 && args[0]->IsNumber() &&
        args[1]->IsNumber() && args[2]->IsNumber() &&
        args[3]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected Fill(r, g, b, a)")));
  }

  VGfloat color[4];
  int r = args[0]->Int32Value();
  int g = args[1]->Int32Value();
  int b = args[2]->Int32Value();
  VGfloat a = (VGfloat) args[3]->NumberValue();

  RGBA(r, g, b, a, color);
  setFill(color);

  return Undefined();
}

Handle<Value> openvg::Ellipse(const Arguments& args) {
  if (!(args.Length() == 4 && args[0]->IsNumber() &&
        args[1]->IsNumber() && args[2]->IsNumber() &&
        args[3]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected Ellipse(x, y, w, h)")));
  }

  VGfloat x = (VGfloat) args[0]->NumberValue();
  VGfloat y = (VGfloat) args[1]->NumberValue();
  VGfloat w = (VGfloat) args[2]->NumberValue();
  VGfloat h = (VGfloat) args[3]->NumberValue();
  ellipse(x, y, w, h);

  return Undefined();
}

Handle<Value> openvg::TextMiddle(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 5)) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected TextMiddle(x, y, text, typeface, pointsize)")));
  }

  return Undefined();
}
