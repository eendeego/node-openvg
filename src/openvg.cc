#define BUILDING_NODE_EXTENSION

#include "VG/openvg.h"
#include "VG/vgu.h"
#include <v8.h>

#include "openvg.h"
#include "egl.h"
#include "argchecks.h"

using namespace node;
using namespace v8;

extern "C" void
init(Handle<Object> target)
{
  NODE_SET_METHOD(target, "startUp"        , openvg::StartUp);
  NODE_SET_METHOD(target, "shutdown"       , openvg::Shutdown);

  NODE_SET_METHOD(target, "getError"       , openvg::GetError);

  NODE_SET_METHOD(target, "flush"          , openvg::Flush);
  NODE_SET_METHOD(target, "finish"         , openvg::Finish);

  /* Getters and Setters */
  NODE_SET_METHOD(target, "setF"           , openvg::SetF);
  NODE_SET_METHOD(target, "setI"           , openvg::SetI);
  NODE_SET_METHOD(target, "setFV"          , openvg::SetFV);
  NODE_SET_METHOD(target, "setIV"          , openvg::SetIV);

  NODE_SET_METHOD(target, "getF"           , openvg::GetF);
  NODE_SET_METHOD(target, "getI"           , openvg::GetI);
  NODE_SET_METHOD(target, "getVectorSize"  , openvg::GetVectorSize);
  NODE_SET_METHOD(target, "getFV"          , openvg::GetFV);
  NODE_SET_METHOD(target, "getIV"          , openvg::GetIV);

  NODE_SET_METHOD(target, "setParameterF"           , openvg::SetParameterF);
  NODE_SET_METHOD(target, "setParameterI"           , openvg::SetParameterI);
  NODE_SET_METHOD(target, "setParameterFV"          , openvg::SetParameterFV);
  NODE_SET_METHOD(target, "setParameterIV"          , openvg::SetParameterIV);

  NODE_SET_METHOD(target, "getParameterF"           , openvg::GetParameterF);
  NODE_SET_METHOD(target, "getParameterI"           , openvg::GetParameterI);
  NODE_SET_METHOD(target, "getParameterVectorSize"  , openvg::GetParameterVectorSize);
  NODE_SET_METHOD(target, "getParameterFV"          , openvg::GetParameterFV);
  NODE_SET_METHOD(target, "getParameterIV"          , openvg::GetParameterIV);

  /* Matrix Manipulation */
  NODE_SET_METHOD(target, "loadIdentity"   , openvg::LoadIdentity);
  NODE_SET_METHOD(target, "loadMatrix"     , openvg::LoadMatrix);
  NODE_SET_METHOD(target, "getMatrix"      , openvg::GetMatrix);
  NODE_SET_METHOD(target, "multMatrix"     , openvg::MultMatrix);
  NODE_SET_METHOD(target, "translate"      , openvg::Translate);
  NODE_SET_METHOD(target, "scale"          , openvg::Scale);
  NODE_SET_METHOD(target, "shear"          , openvg::Shear);
  NODE_SET_METHOD(target, "rotate"         , openvg::Rotate);

  /* Masking and Clearing */
  NODE_SET_METHOD(target, "mask"            , openvg::Mask);
  NODE_SET_METHOD(target, "renderToMask"    , openvg::RenderToMask);
  NODE_SET_METHOD(target, "createMaskLayer" , openvg::CreateMaskLayer);
  NODE_SET_METHOD(target, "destroyMaskLayer", openvg::DestroyMaskLayer);
  NODE_SET_METHOD(target, "fillMaskLayer"   , openvg::FillMaskLayer);
  NODE_SET_METHOD(target, "copyMask"        , openvg::CopyMask);
  NODE_SET_METHOD(target, "clear"           , openvg::Clear);

  /* Paths */
  NODE_SET_METHOD(target, "createPath"            , openvg::CreatePath);
  NODE_SET_METHOD(target, "clearPath"             , openvg::ClearPath);
  NODE_SET_METHOD(target, "destroyPath"           , openvg::DestroyPath);
  NODE_SET_METHOD(target, "removePathCapabilities", openvg::RemovePathCapabilities);
  NODE_SET_METHOD(target, "getPathCapabilities"   , openvg::GetPathCapabilities);
  NODE_SET_METHOD(target, "appendPath"            , openvg::AppendPath);
  NODE_SET_METHOD(target, "appendPathData"        , openvg::AppendPathData);
  NODE_SET_METHOD(target, "modifyPathCoords"      , openvg::ModifyPathCoords);
  NODE_SET_METHOD(target, "transformPath"         , openvg::TransformPath);
  NODE_SET_METHOD(target, "interpolatePath"       , openvg::InterpolatePath);
  NODE_SET_METHOD(target, "pathLength"            , openvg::PathLength);
  NODE_SET_METHOD(target, "pointAlongPath"        , openvg::PointAlongPath);
  NODE_SET_METHOD(target, "pathBounds"            , openvg::PathBounds);
  NODE_SET_METHOD(target, "pathTransformedBounds" , openvg::PathTransformedBounds);
  NODE_SET_METHOD(target, "drawPath"              , openvg::DrawPath);

  /* Paint */
  NODE_SET_METHOD(target, "createPaint" , openvg::CreatePaint);
  NODE_SET_METHOD(target, "destroyPaint", openvg::DestroyPaint);
  NODE_SET_METHOD(target, "setPaint"    , openvg::SetPaint);
  NODE_SET_METHOD(target, "getPaint"    , openvg::GetPaint);
  NODE_SET_METHOD(target, "setColor"    , openvg::SetColor);
  NODE_SET_METHOD(target, "getColor"    , openvg::GetColor);
  NODE_SET_METHOD(target, "paintPattern", openvg::PaintPattern);

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

  CheckArgs1(startUp, screen, Object);

  egl::Init();
  CHECK_VG_ERROR;

  Local<Object> screen = args[0].As<Object>();
  screen->Set(String::NewSymbol("width" ), Integer::New(egl::State.screen_width));
  screen->Set(String::NewSymbol("height"), Integer::New(egl::State.screen_height));

  return Undefined();
}

Handle<Value> openvg::Shutdown(const Arguments& args) {
  HandleScope scope;

  CheckArgs0(shutdown);

  egl::Finish();

  return Undefined();
}


Handle<Value> openvg::GetError(const Arguments& args) {
  HandleScope scope;

  CheckArgs0(getError);

  return Integer::New(vgGetError());
}


Handle<Value> openvg::Flush(const Arguments& args) {
  HandleScope scope;

  CheckArgs0(flush);

  vgFlush();

  return Undefined();
}

Handle<Value> openvg::Finish(const Arguments& args) {
  HandleScope scope;

  CheckArgs0(finish);

  vgFinish();

  return Undefined();
}


/* Getters and Setters */


Handle<Value> openvg::SetF(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(setF, type, Int32, value, Number);

  vgSetf((VGParamType) args[0]->Int32Value(),
         (VGfloat) args[1]->NumberValue());

  return Undefined();
}

Handle<Value> openvg::SetI(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(setI, type, Int32, value, Int32);

  vgSeti((VGParamType) args[0]->Int32Value(),
         (VGint) args[1]->Int32Value());

  return Undefined();
}

Handle<Value> openvg::SetFV(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(setFV, type, Int32, Float32Array, Object);

  Local<Object> array = args[1]->ToObject();
  Handle<Object> buffer = array->Get(String::New("buffer"))->ToObject();

  vgSetfv((VGParamType) args[0]->Int32Value(),
          (VGint) array->Get(String::New("length"))->Uint32Value(),
          (VGfloat*) buffer->GetIndexedPropertiesExternalArrayData());

  return Undefined();
}

Handle<Value> openvg::SetIV(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(setIV, type, Int32, Int32Array, Object);

  Local<Object> array = args[1]->ToObject();
  Handle<Object> buffer = array->Get(String::New("buffer"))->ToObject();

  vgSetiv((VGParamType) args[0]->Int32Value(),
          (VGint) array->Get(String::New("length"))->Uint32Value(),
          (VGint*) buffer->GetIndexedPropertiesExternalArrayData());

  return Undefined();
}

Handle<Value> openvg::GetF(const Arguments& args) {
  HandleScope scope;

  CheckArgs1(getF, type, Int32);

  return Number::New(vgGetf((VGParamType) args[0]->Int32Value()));
}

Handle<Value> openvg::GetI(const Arguments& args) {
  HandleScope scope;

  CheckArgs1(getI, type, Int32);

  return Integer::New(vgGeti((VGParamType) args[0]->Int32Value()));
}

Handle<Value> openvg::GetVectorSize(const Arguments& args) {
  HandleScope scope;

  CheckArgs1(getVectorSize, type, Int32);

  return Integer::New(vgGetVectorSize((VGParamType) args[0]->Int32Value()));
}

Handle<Value> openvg::GetFV(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(getFV, type, Int32, Float32Array, Object);

  Local<Object> array = args[1]->ToObject();
  Handle<Object> buffer = array->Get(String::New("buffer"))->ToObject();

  vgGetfv((VGParamType) args[0]->Int32Value(),
          (VGint) array->Get(String::New("length"))->Uint32Value(),
          (VGfloat*) buffer->GetIndexedPropertiesExternalArrayData());

  return Undefined();
}

Handle<Value> openvg::GetIV(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(getIV, type, Int32, Float32Array, Object);

  Local<Object> array = args[1]->ToObject();
  Handle<Object> buffer = array->Get(String::New("buffer"))->ToObject();

  vgGetiv((VGParamType) args[0]->Int32Value(),
          (VGint) array->Get(String::New("length"))->Uint32Value(),
          (VGint*) buffer->GetIndexedPropertiesExternalArrayData());

  return Undefined();
}


Handle<Value> openvg::SetParameterF(const Arguments& args) {
  HandleScope scope;

  CheckArgs3(setParameterF, VGHandle, Int32, VGParamType, Int32, value, Number);

  vgSetParameterf((VGHandle) args[0]->Int32Value(),
                  (VGParamType) args[1]->Int32Value(),
                  (VGfloat) args[2]->NumberValue());

  return Undefined();
}

Handle<Value> openvg::SetParameterI(const Arguments& args) {
  HandleScope scope;

  CheckArgs3(setParameterI, VGHandle, Int32, VGParamType, Int32, value, Int32);

  vgSetParameteri((VGHandle) args[0]->Int32Value(),
                  (VGParamType) args[1]->Int32Value(),
                  (VGint) args[2]->Int32Value());

  return Undefined();
}

Handle<Value> openvg::SetParameterFV(const Arguments& args) {
  HandleScope scope;

  CheckArgs3(setParameterFV, VGHandle, Int32, VGParamType, Int32, Float32Array, Object);

  Local<Object> array = args[2]->ToObject();
  Handle<Object> buffer = array->Get(String::New("buffer"))->ToObject();

  vgSetParameterfv((VGHandle) args[0]->Int32Value(),
                   (VGParamType) args[1]->Int32Value(),
                   (VGint) array->Get(String::New("length"))->Uint32Value(),
                   (VGfloat*) buffer->GetIndexedPropertiesExternalArrayData());

  return Undefined();
}

Handle<Value> openvg::SetParameterIV(const Arguments& args) {
  HandleScope scope;

  CheckArgs3(setParameterIV, VGHandle, Int32, VGParamType, Int32, Int32Array, Object);

  Local<Object> array = args[2]->ToObject();
  Handle<Object> buffer = array->Get(String::New("buffer"))->ToObject();

  vgSetParameteriv((VGHandle) args[0]->Int32Value(),
                   (VGParamType) args[1]->Int32Value(),
                   (VGint) array->Get(String::New("length"))->Uint32Value(),
                   (VGint*) buffer->GetIndexedPropertiesExternalArrayData());

  return Undefined();
}

Handle<Value> openvg::GetParameterF(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(getParameterF, VGHandle, Int32, VGParamType, Int32);

  return Number::New(vgGetParameterf((VGHandle) args[0]->Int32Value(),
                                     (VGParamType) args[1]->Int32Value()));
}

Handle<Value> openvg::GetParameterI(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(getParameterI, VGHandle, Int32, VGParamType, Int32);

  return Integer::New(vgGetParameteri((VGHandle) args[0]->Int32Value(),
                                      (VGParamType) args[1]->Int32Value()));
}

Handle<Value> openvg::GetParameterVectorSize(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(getParameterVectorSize, VGHandle, Int32, VGParamType, Int32);

  return Integer::New(vgGetParameterVectorSize((VGHandle) args[0]->Int32Value(),
                                               (VGParamType) args[1]->Int32Value()));
}

Handle<Value> openvg::GetParameterFV(const Arguments& args) {
  HandleScope scope;

  CheckArgs3(getParameterFV, VGHandle, Int32, VGParamType, Int32, Float32Array, Object);

  Local<Object> array = args[2]->ToObject();
  Handle<Object> buffer = array->Get(String::New("buffer"))->ToObject();

  vgGetParameterfv((VGHandle) args[0]->Int32Value(),
                   (VGParamType) args[1]->Int32Value(),
                   (VGint) array->Get(String::New("length"))->Uint32Value(),
                   (VGfloat*) buffer->GetIndexedPropertiesExternalArrayData());

  return Undefined();
}

Handle<Value> openvg::GetParameterIV(const Arguments& args) {
  HandleScope scope;

  CheckArgs3(getParameterIV, VGHandle, Int32, VGParamType, Int32, Int32Array, Object);

  Local<Object> array = args[2]->ToObject();
  Handle<Object> buffer = array->Get(String::New("buffer"))->ToObject();

  vgGetParameteriv((VGHandle) args[0]->Int32Value(),
                   (VGParamType) args[1]->Int32Value(),
                   (VGint) array->Get(String::New("length"))->Uint32Value(),
                   (VGint*) buffer->GetIndexedPropertiesExternalArrayData());

  return Undefined();
}


/* Matrix Manipulation */


Handle<Value> openvg::LoadIdentity(const Arguments& args) {
  HandleScope scope;

  CheckArgs0(loadIdentity);

  vgLoadIdentity();

  return Undefined();
}

Handle<Value> openvg::LoadMatrix(const Arguments& args) {
  HandleScope scope;

  CheckArgs1(loadIdentity, Float32Array, Object);

  Local<Object> array = args[0]->ToObject();
  Handle<Object> buffer = array->Get(String::New("buffer"))->ToObject();

  vgLoadMatrix((VGfloat*) buffer->GetIndexedPropertiesExternalArrayData());

  return Undefined();
}

Handle<Value> openvg::GetMatrix(const Arguments& args) {
  HandleScope scope;

  CheckArgs1(getMatrix, Float32Array, Object);

  Local<Object> array = args[0]->ToObject();
  Handle<Object> buffer = array->Get(String::New("buffer"))->ToObject();

  vgGetMatrix((VGfloat*) buffer->GetIndexedPropertiesExternalArrayData());

  return Undefined();
}

Handle<Value> openvg::MultMatrix(const Arguments& args) {
  HandleScope scope;

  CheckArgs1(multMatrix, Float32Array, Object);

  Local<Object> array = args[0]->ToObject();
  Handle<Object> buffer = array->Get(String::New("buffer"))->ToObject();

  vgMultMatrix((VGfloat*) buffer->GetIndexedPropertiesExternalArrayData());

  return Undefined();
}

Handle<Value> openvg::Translate(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(translate, x, Number, y, Number);

  vgTranslate((VGfloat) args[0]->NumberValue(),
              (VGfloat) args[1]->NumberValue());

  return Undefined();
}

Handle<Value> openvg::Scale(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(scale, x, Number, y, Number);

  vgScale((VGfloat) args[0]->NumberValue(),
          (VGfloat) args[1]->NumberValue());

  return Undefined();
}

Handle<Value> openvg::Shear(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(shear, x, Number, y, Number);

  vgShear((VGfloat) args[0]->NumberValue(),
          (VGfloat) args[1]->NumberValue());

  return Undefined();
}

Handle<Value> openvg::Rotate(const Arguments& args) {
  HandleScope scope;

  CheckArgs1(shear, angle, Number);

  vgRotate((VGfloat) args[0]->NumberValue());

  return Undefined();
}


/* Masking and Clearing */


Handle<Value> openvg::Mask(const Arguments& args) {
  HandleScope scope;

  CheckArgs6(mask,
             VGHandle, Uint32, VGMaskOperation, Uint32,
             x, Int32, y, Int32, width, Int32, height, Int32);

  vgMask((VGHandle) args[0]->Uint32Value(),
         static_cast<VGMaskOperation>(args[1]->Uint32Value()),
         (VGint) args[2]->Int32Value(),
         (VGint) args[3]->Int32Value(),
         (VGint) args[4]->Int32Value(),
         (VGint) args[5]->Int32Value());

  return Undefined();
}

Handle<Value> openvg::RenderToMask(const Arguments& args) {
  HandleScope scope;

  CheckArgs3(renderToMask,
             VGPath, Uint32,
             VGbitfield, Uint32,
             VGMaskOperation, Uint32);

  vgRenderToMask((VGPath) args[0]->Uint32Value(),
                 (VGbitfield) args[1]->Uint32Value(),
                 (VGMaskOperation) args[2]->Uint32Value());

  return Undefined();
}

Handle<Value> openvg::CreateMaskLayer(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(createMaskLayer, width, Int32, height, Int32);

  return Integer::New(vgCreateMaskLayer((VGint) args[0]->Int32Value(),
                                        (VGint) args[1]->Int32Value()));
}

Handle<Value> openvg::DestroyMaskLayer(const Arguments& args) {
  HandleScope scope;

  CheckArgs1(destroyMaskLayer, VGMaskLayer, Uint32);

  vgDestroyMaskLayer((VGMaskLayer) args[0]->Uint32Value());

  return Undefined();
}

Handle<Value> openvg::FillMaskLayer(const Arguments& args) {
  HandleScope scope;

  CheckArgs6(fillMaskLayer,
             VGMaskLayer, Uint32,
             x, Int32, y, Int32, width, Int32, height, Int32,
             value, Number);

  vgFillMaskLayer((VGMaskLayer) args[0]->Uint32Value(),
                  (VGint) args[1]->Int32Value(),
                  (VGint) args[2]->Int32Value(),
                  (VGint) args[3]->Int32Value(),
                  (VGint) args[4]->Int32Value(),
                  (VGfloat) args[5]->NumberValue());

  return Undefined();
}

Handle<Value> openvg::CopyMask(const Arguments& args) {
  HandleScope scope;

  CheckArgs7(fillMaskLayer,
             VGMaskLayer, Uint32,
             dx, Int32, dy, Int32, sx, Int32, sy, Int32,
             width, Int32, height, Int32);

  vgCopyMask((VGMaskLayer) args[0]->Uint32Value(),
             (VGint) args[1]->Int32Value(), (VGint) args[2]->Int32Value(),
             (VGint) args[3]->Int32Value(), (VGint) args[4]->Int32Value(),
             (VGint) args[5]->Int32Value(), (VGint) args[6]->Int32Value());

  return Undefined();
}

Handle<Value> openvg::Clear(const Arguments& args) {
  HandleScope scope;

  CheckArgs4(clear, x, Int32, y, Int32, width, Int32, height, Int32);

  vgClear((VGint) args[0]->Int32Value(), (VGint) args[1]->Int32Value(),
          (VGint) args[2]->Int32Value(), (VGint) args[3]->Int32Value());

  return Undefined();
}


/* Paths */


Handle<Value> openvg::CreatePath(const Arguments& args) {
  HandleScope scope;

  CheckArgs7(createPath,
             pathFormat, Int32, VGPathDatatype, Uint32,
             scale, Number, bias, Number, segmentCapacityHint, Int32,
             coordCapacityHint, Int32, capabilities, Uint32);

  return Uint32::New(vgCreatePath((VGint) args[0]->Int32Value(),
                                  static_cast<VGPathDatatype>(args[1]->Uint32Value()),
                                  (VGfloat) args[2]->NumberValue(),
                                  (VGfloat) args[3]->NumberValue(),
                                  (VGint) args[4]->Int32Value(),
                                  (VGint) args[5]->Int32Value(),
                                  (VGbitfield) args[6]->Uint32Value()));
}

Handle<Value> openvg::ClearPath(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(clearPath, VGPath, Number, capabilities, Uint32);

  vgClearPath((VGPath) args[0]->Uint32Value(),
              (VGbitfield) args[1]->Uint32Value());

  return Undefined();
}

Handle<Value> openvg::DestroyPath(const Arguments& args) {
  HandleScope scope;

  CheckArgs1(destroyPath, VGPath, Number);

  vgDestroyPath((VGPath) args[0]->Uint32Value());

  return Undefined();
}

Handle<Value> openvg::RemovePathCapabilities(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(removePathCapabilities, VGPath, Number, capabilities, Uint32);

  vgRemovePathCapabilities((VGPath) args[0]->Uint32Value(),
                           (VGbitfield) args[1]->Uint32Value());

  return Undefined();
}

Handle<Value> openvg::GetPathCapabilities(const Arguments& args) {
  HandleScope scope;

  CheckArgs1(getPathCapabilities, VGPath, Number);

  return Uint32::New(vgGetPathCapabilities((VGPath) args[0]->Int32Value()));
}

Handle<Value> openvg::AppendPath(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(appendPath, dstPath, Number, srcPath, Number);

  vgAppendPath((VGPath) args[0]->Uint32Value(),
               (VGPath) args[1]->Uint32Value());

  return Undefined();
}

Handle<Value> openvg::AppendPathData(const Arguments& args) {
  HandleScope scope;

  CheckArgs4(appendPathData, dstPath, Number, numSegments, Int32, Uint8Array, Object, pathData, Object);

  Local<Object> segmentsArray = args[2]->ToObject();
  Handle<Object> segmentsBuffer = segmentsArray->Get(String::New("buffer"))->ToObject();

  Local<Object> dataArray = args[3]->ToObject();
  Handle<Object> dataBuffer = dataArray->Get(String::New("buffer"))->ToObject();

  vgAppendPathData((VGPath) args[0]->Uint32Value(),
                   (VGint) args[1]->Int32Value(),
                   (VGubyte*) segmentsBuffer->GetIndexedPropertiesExternalArrayData(),
                   (void*) dataBuffer->GetIndexedPropertiesExternalArrayData());

  return Undefined();
}

Handle<Value> openvg::ModifyPathCoords(const Arguments& args) {
  HandleScope scope;

  CheckArgs4(modifyPathCoords, VGPath, Number, startIndex, Int32, numSegments, Int32, pathData, Object);

  Local<Object> dataArray = args[3]->ToObject();
  Handle<Object> dataBuffer = dataArray->Get(String::New("buffer"))->ToObject();

  vgModifyPathCoords((VGPath) args[0]->Uint32Value(),
                     (VGint) args[1]->Int32Value(),
                     (VGint) args[2]->Int32Value(),
                     (void*) dataBuffer->GetIndexedPropertiesExternalArrayData());

  return Undefined();
}

Handle<Value> openvg::TransformPath(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(transformPath, dstPath, Number, srcPath, Number);

  vgTransformPath((VGPath) args[0]->Uint32Value(),
                  (VGPath) args[1]->Uint32Value());

  return Undefined();
}

Handle<Value> openvg::InterpolatePath(const Arguments& args) {
  HandleScope scope;

  CheckArgs4(interpolatePath,
             dstPath, Number, startPath, Number, endPath, Number,
             amount, Number);

  return Boolean::New(vgInterpolatePath((VGPath) args[0]->Uint32Value(),
                                        (VGPath) args[1]->Uint32Value(),
                                        (VGPath) args[2]->Uint32Value(),
                                        (VGfloat) args[3]->NumberValue()));
}

Handle<Value> openvg::PathLength(const Arguments& args) {
  HandleScope scope;

  CheckArgs3(pathLength, path, Number,
             startSegment, Int32, numSegments, Int32);

  return Number::New(vgPathLength((VGPath) args[0]->Uint32Value(),
                                  (VGint) args[1]->Int32Value(),
                                  (VGint) args[2]->Int32Value()));
}

Handle<Value> openvg::PointAlongPath(const Arguments& args) {
  HandleScope scope;

  CheckArgs5(pointAlongPath, path, Number,
             startSegment, Int32, numSegments, Int32,
             distance, Number, point, Object);

  VGfloat x, y, tx, ty;

  vgPointAlongPath((VGPath) args[0]->Uint32Value(),
                   (VGint) args[1]->Int32Value(),
                   (VGint) args[2]->Int32Value(),
                   (VGfloat) args[3]->NumberValue(),
                   &x, &y, &tx, &ty);

  Local<Object> point = args[4].As<Object>();
  point->Set(String::NewSymbol("x"), Number::New(x));
  point->Set(String::NewSymbol("y"), Number::New(y));
  point->Set(String::NewSymbol("tx"), Number::New(tx));
  point->Set(String::NewSymbol("ty"), Number::New(ty));

  return Undefined();
}

Handle<Value> openvg::PathBounds(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(pathBounds, VGPath, Number, bounds, Object);

  VGfloat minX, minY, width, height;

  vgPathBounds((VGPath) args[0]->Uint32Value(),
               &minX, &minY, &width, &height);

  Local<Object> bounds = args[1].As<Object>();
  bounds->Set(String::NewSymbol("x"), Number::New(minX));
  bounds->Set(String::NewSymbol("y"), Number::New(minY));
  bounds->Set(String::NewSymbol("w"), Number::New(width));
  bounds->Set(String::NewSymbol("h"), Number::New(height));

  return Undefined();
}

Handle<Value> openvg::PathTransformedBounds(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(pathTransformedBounds, VGPath, Number, bounds, Object);

  VGfloat minX, minY, width, height;

  vgPathTransformedBounds((VGPath) args[0]->Uint32Value(),
                          &minX, &minY, &width, &height);

  Local<Object> bounds = args[1].As<Object>();
  bounds->Set(String::NewSymbol("x"), Number::New(minX));
  bounds->Set(String::NewSymbol("y"), Number::New(minY));
  bounds->Set(String::NewSymbol("w"), Number::New(width));
  bounds->Set(String::NewSymbol("h"), Number::New(height));

  return Undefined();
}

Handle<Value> openvg::DrawPath(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(drawPath, VGPath, Number, paintModes, Number);

  vgDrawPath((VGPath) args[0]->Uint32Value(),
             (VGbitfield) args[1]->Uint32Value());

  return Undefined();
}


/* Paint */


Handle<Value> openvg::CreatePaint(const Arguments& args) {
  HandleScope scope;

  CheckArgs0(createPaint);

  return Uint32::New(vgCreatePaint());
}

Handle<Value> openvg::DestroyPaint(const Arguments& args) {
  HandleScope scope;

  CheckArgs1(destroyPaint, VGPaint, Uint32);

  vgDestroyPaint((VGPaint) args[0]->Uint32Value());

  return Undefined();
}

Handle<Value> openvg::SetPaint(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(setPaint, VGPaint, Uint32, paintModes, Uint32);

  vgSetPaint((VGPaint) args[0]->Uint32Value(),
             (VGbitfield) args[1]->Uint32Value());

  return Undefined();
}

Handle<Value> openvg::GetPaint(const Arguments& args) {
  HandleScope scope;

  CheckArgs1(getPaint, VGPaint, Uint32);

  return Uint32::New(vgGetPaint(static_cast<VGPaintMode>(args[0]->Uint32Value())));
}

Handle<Value> openvg::SetColor(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(setColor, VGPaint, Uint32, rgba, Uint32);

  vgSetColor((VGPaint) args[0]->Uint32Value(),
             (VGuint) args[1]->Uint32Value());

  return Undefined();
}

Handle<Value> openvg::GetColor(const Arguments& args) {
  HandleScope scope;

  CheckArgs1(getColor, VGPaint, Uint32);

  return Uint32::New(vgGetColor((VGPaint) args[0]->Uint32Value()));
}

Handle<Value> openvg::PaintPattern(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(paintPattern, VGPaint, Uint32, VGImage, Uint32);

  vgPaintPattern((VGPaint) args[0]->Uint32Value(),
                 (VGImage) args[1]->Uint32Value());

  return Undefined();
}





Handle<Value> openvg::Start(const Arguments& args) {
  HandleScope scope;

  CheckArgs0(start);

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

  CheckArgs0(end);

  CHECK_VG_ERROR;
  egl::swapBuffers(egl::State.display, egl::State.surface);
  assert(eglGetError() == EGL_SUCCESS);

  return Undefined();
}

Handle<Value> openvg::Rect(const Arguments& args) {
  CheckArgs4(rect, x, Number, y, Number, w, Number, h, Number);

  VGfloat x = (VGfloat) args[0]->NumberValue();
  VGfloat y = (VGfloat) args[1]->NumberValue();
  VGfloat w = (VGfloat) args[2]->NumberValue();
  VGfloat h = (VGfloat) args[3]->NumberValue();

  rect(x, y, w, h);

  return Undefined();
}

Handle<Value> openvg::Fill(const Arguments& args) {
  HandleScope scope;

  CheckArgs4(fill, r, Int32, g, Int32, b, Int32, a, Number);

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
  CheckArgs4(ellipse, x, Number, y, Number, w, Number, h, Number);

  VGfloat x = (VGfloat) args[0]->NumberValue();
  VGfloat y = (VGfloat) args[1]->NumberValue();
  VGfloat w = (VGfloat) args[2]->NumberValue();
  VGfloat h = (VGfloat) args[3]->NumberValue();
  ellipse(x, y, w, h);

  return Undefined();
}

Handle<Value> openvg::TextMiddle(const Arguments& args) {
  HandleScope scope;

  CheckArgs5(textMiddle, x, Number, y, Number, text, String, typeface, String, pointsize, Number);

  return Undefined();
}
