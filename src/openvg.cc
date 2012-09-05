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

  /* Images */
  NODE_SET_METHOD(target, "createImage"    , openvg::CreateImage);
  NODE_SET_METHOD(target, "destroyImage"   , openvg::DestroyImage);
  NODE_SET_METHOD(target, "clearImage"     , openvg::ClearImage);
  NODE_SET_METHOD(target, "imageSubData"   , openvg::ImageSubData);
  NODE_SET_METHOD(target, "getImageSubData", openvg::GetImageSubData);
  NODE_SET_METHOD(target, "childImage"     , openvg::ChildImage);
  NODE_SET_METHOD(target, "getParent"      , openvg::GetParent);
  NODE_SET_METHOD(target, "copyImage"      , openvg::CopyImage);
  NODE_SET_METHOD(target, "drawImage"      , openvg::DrawImage);
  NODE_SET_METHOD(target, "setPixels"      , openvg::SetPixels);
  NODE_SET_METHOD(target, "writePixels"    , openvg::WritePixels);
  NODE_SET_METHOD(target, "getPixels"      , openvg::GetPixels);
  NODE_SET_METHOD(target, "readPixels"     , openvg::ReadPixels);
  NODE_SET_METHOD(target, "copyPixels"     , openvg::CopyPixels);

  /* Text */
  NODE_SET_METHOD(target, "createFont"     , openvg::CreateFont);
  NODE_SET_METHOD(target, "destroyFont"    , openvg::DestroyFont);
  NODE_SET_METHOD(target, "setGlyphToPath" , openvg::SetGlyphToPath);
  NODE_SET_METHOD(target, "setGlyphToImage", openvg::SetGlyphToImage);
  NODE_SET_METHOD(target, "clearGlyph"     , openvg::ClearGlyph);
  NODE_SET_METHOD(target, "drawGlyph"      , openvg::DrawGlyph);
  NODE_SET_METHOD(target, "drawGlyphs"     , openvg::DrawGlyphs);


  NODE_SET_METHOD(target, "end"            , openvg::End);

  NODE_SET_METHOD(target, "rect"           , openvg::Rect);
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

  CheckArgs1(destroyPaint, VGPaint, Number);

  vgDestroyPaint((VGPaint) args[0]->Uint32Value());

  return Undefined();
}

Handle<Value> openvg::SetPaint(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(setPaint, VGPaint, Number, paintModes, Number);

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


/* Images */


Handle<Value> openvg::CreateImage(const Arguments& args) {
  HandleScope scope;

  CheckArgs4(createImage, VGImageFormat, Uint32, width, Int32, height, Int32, allowedQuality, Uint32);

  return Uint32::New(vgCreateImage(static_cast<VGImageFormat>(args[0]->Uint32Value()),
                                   (VGint) args[1]->Int32Value(),
                                   (VGint) args[2]->Int32Value(),
                                   (VGuint) args[3]->Uint32Value()));
}

Handle<Value> openvg::DestroyImage(const Arguments& args) {
  HandleScope scope;

  CheckArgs1(destroyImage, VGImage, Uint32);

  vgDestroyImage((VGImage) (VGPaint) args[0]->Uint32Value());

  return Undefined();
}

Handle<Value> openvg::ClearImage(const Arguments& args) {
  HandleScope scope;

  CheckArgs5(clearImage, VGImage, Uint32, x, Int32, y, Int32, width, Int32, height, Int32);

  vgClearImage((VGImage) args[0]->Uint32Value(),
               (VGint) args[1]->Int32Value(),
               (VGint) args[2]->Int32Value(),
               (VGint) args[3]->Int32Value(),
               (VGint) args[4]->Int32Value());

  return Undefined();
}

Handle<Value> openvg::ImageSubData(const Arguments& args) {
  HandleScope scope;

  CheckArgs8(imageSubData,
             VGImage, Uint32, data, Object, dataStride, Int32, dataFormat, Uint32,
             x, Int32, y, Int32, width, Int32, height, Int32);

  Local<Object> dataArray = args[1]->ToObject();
  Handle<Object> dataBuffer = dataArray->Get(String::New("buffer"))->ToObject();

  vgImageSubData((VGImage) args[0]->Uint32Value(),
                 (void*) dataBuffer->GetIndexedPropertiesExternalArrayData(),
                 (VGint) args[2]->Int32Value(),
                 static_cast<VGImageFormat>(args[3]->Uint32Value()),
                 (VGint) args[4]->Int32Value(),
                 (VGint) args[5]->Int32Value(),
                 (VGint) args[6]->Int32Value(),
                 (VGint) args[7]->Int32Value());

  return Undefined();
}

Handle<Value> openvg::GetImageSubData(const Arguments& args) {
  HandleScope scope;

  CheckArgs8(getImageSubData,
             VGImage, Uint32, data, Object, dataStride, Int32, dataFormat, Uint32,
             x, Int32, y, Int32, width, Int32, height, Int32);

  Local<Object> dataArray = args[1]->ToObject();
  Handle<Object> dataBuffer = dataArray->Get(String::New("buffer"))->ToObject();

  vgGetImageSubData((VGImage) args[0]->Uint32Value(),
                    (void*) dataBuffer->GetIndexedPropertiesExternalArrayData(),
                    (VGint) args[2]->Int32Value(),
                    static_cast<VGImageFormat>(args[3]->Uint32Value()),
                    (VGint) args[4]->Int32Value(),
                    (VGint) args[5]->Int32Value(),
                    (VGint) args[6]->Int32Value(),
                    (VGint) args[7]->Int32Value());

  return Undefined();
}

Handle<Value> openvg::ChildImage(const Arguments& args) {
  HandleScope scope;

  CheckArgs5(childImage, VGImage, Uint32, x, Int32, y, Int32, width, Int32, height, Int32);

  return Uint32::New(vgChildImage((VGImage) args[0]->Uint32Value(),
                                  (VGint) args[1]->Int32Value(),
                                  (VGint) args[2]->Int32Value(),
                                  (VGint) args[3]->Int32Value(),
                                  (VGint) args[4]->Int32Value()));
}

Handle<Value> openvg::GetParent(const Arguments& args) {
  HandleScope scope;

  CheckArgs1(getParent, VGImage, Int32);

  return Uint32::New(vgGetParent((VGImage) args[0]->Uint32Value()));
}

Handle<Value> openvg::CopyImage(const Arguments& args) {
  HandleScope scope;

  CheckArgs9(copyImage,
             dstImage, Uint32, dx, Int32, dy, Int32,
             srcImage, Uint32, sx, Int32, sy, Int32,
             width, Int32, height, Int32, dither, Boolean);

  vgCopyImage((VGImage) args[0]->Uint32Value(),
              (VGint) args[1]->Int32Value(),
              (VGint) args[2]->Int32Value(),
              (VGImage) args[3]->Uint32Value(),
              (VGint) args[4]->Int32Value(),
              (VGint) args[5]->Int32Value(),
              (VGint) args[6]->Int32Value(),
              (VGint) args[7]->Int32Value(),
              (VGboolean) args[8]->BooleanValue());

  return Undefined();
}

Handle<Value> openvg::DrawImage(const Arguments& args) {
  HandleScope scope;

  CheckArgs1(drawImage, VGImage, Uint32);

  vgDrawImage((VGImage) args[0]->Uint32Value());

  return Undefined();
}

Handle<Value> openvg::SetPixels(const Arguments& args) {
  HandleScope scope;

  CheckArgs7(setPixels,
             dx, Int32, dy, Int32,
             srcImage, Uint32, dx, Int32, dy, Int32,
             width, Int32, height, Int32);

  vgSetPixels((VGint) args[0]->Int32Value(),
              (VGint) args[1]->Int32Value(),
              (VGImage) args[2]->Uint32Value(),
              (VGint) args[3]->Int32Value(),
              (VGint) args[4]->Int32Value(),
              (VGint) args[5]->Int32Value(),
              (VGint) args[6]->Int32Value());

  return Undefined();
}

Handle<Value> openvg::WritePixels(const Arguments& args) {
  HandleScope scope;

  CheckArgs7(writePixels, data, Object, dataStride, Int32,
             dataFormat, Uint32,
             dx, Int32, dy, Int32, width, Int32, height, Int32);

  Local<Object> dataArray = args[0]->ToObject();
  Handle<Object> dataBuffer = dataArray->Get(String::New("buffer"))->ToObject();

  vgWritePixels((void*) dataBuffer->GetIndexedPropertiesExternalArrayData(),
                (VGint) args[1]->Int32Value(),
                static_cast<VGImageFormat>(args[2]->Uint32Value()),
                (VGint) args[3]->Int32Value(),
                (VGint) args[4]->Int32Value(),
                (VGint) args[5]->Int32Value(),
                (VGint) args[6]->Int32Value());

  return Undefined();
}

Handle<Value> openvg::GetPixels(const Arguments& args) {
  HandleScope scope;

  CheckArgs7(getPixels,
             VGImage, Uint32,
             dx, Int32, dy, Int32,
             sx, Int32, sy, Int32,
             width, Int32, height, Int32);

  vgGetPixels((VGImage) args[0]->Uint32Value(),
              (VGint) args[1]->Int32Value(),
              (VGint) args[2]->Int32Value(),
              (VGint) args[3]->Int32Value(),
              (VGint) args[4]->Int32Value(),
              (VGint) args[5]->Int32Value(),
              (VGint) args[6]->Int32Value());

  return Undefined();
}

Handle<Value> openvg::ReadPixels(const Arguments& args) {
  HandleScope scope;

  CheckArgs7(readPixels,
             data, Object, dataStride, Int32, dataFormat, Uint32,
             sx, Int32, sy, Int32, width, Int32, height, Int32);

  Local<Object> dataArray = args[0]->ToObject();
  Handle<Object> dataBuffer = dataArray->Get(String::New("buffer"))->ToObject();

  vgReadPixels((void*) dataBuffer->GetIndexedPropertiesExternalArrayData(),
               (VGint) args[1]->Int32Value(),
               static_cast<VGImageFormat>(args[2]->Uint32Value()),
               (VGint) args[3]->Int32Value(),
               (VGint) args[4]->Int32Value(),
               (VGint) args[5]->Int32Value(),
               (VGint) args[6]->Int32Value());

  return Undefined();
}

Handle<Value> openvg::CopyPixels(const Arguments& args) {
  HandleScope scope;

  CheckArgs6(copyPixels,
             dx, Int32, dy, Int32, sx, Int32, sy, Int32,
             width, Int32, height, Int32);

  vgCopyPixels((VGint) args[0]->Int32Value(),
               (VGint) args[1]->Int32Value(),
               (VGint) args[2]->Int32Value(),
               (VGint) args[3]->Int32Value(),
               (VGint) args[4]->Int32Value(),
               (VGint) args[5]->Int32Value());

  return Undefined();
}


/* Text */


Handle<Value> openvg::CreateFont(const Arguments& args) {
  HandleScope scope;

  CheckArgs1(createFont, glyphCapacityHint, Int32);

  return Uint32::New(vgCreateFont((VGint) args[0]->Int32Value()));
}

Handle<Value> openvg::DestroyFont(const Arguments& args) {
  HandleScope scope;

  CheckArgs1(destroyFont, VGFont, Number);

  vgDestroyFont((VGFont) args[0]->Uint32Value());

  return Undefined();
}

Handle<Value> openvg::SetGlyphToPath(const Arguments& args) {
  HandleScope scope;

  CheckArgs6(setGlyphToPath, VGFont, Number, glyphIndex, Uint32,
             VGPath, Uint32, isHinted, Boolean,
             glyphOrigin, Object, escapement, Object);

  Local<Object> glyphOriginArray = args[4]->ToObject();
  Handle<Object> glyphOriginBuffer = glyphOriginArray->Get(String::New("buffer"))->ToObject();

  Local<Object> escapementArray = args[5]->ToObject();
  Handle<Object> escapementBuffer = escapementArray->Get(String::New("buffer"))->ToObject();

  vgSetGlyphToPath((VGFont) args[0]->Uint32Value(),
                   (VGuint) args[1]->Uint32Value(),
                   (VGPath) args[2]->Uint32Value(),
                   (VGboolean) args[3]->BooleanValue(),
                   (VGfloat*) glyphOriginBuffer->GetIndexedPropertiesExternalArrayData(),
                   (VGfloat*) escapementBuffer->GetIndexedPropertiesExternalArrayData());

  return Undefined();
}

Handle<Value> openvg::SetGlyphToImage(const Arguments& args) {
  HandleScope scope;

  CheckArgs5(setGlyphToImage, VGFont, Number, glyphIndex, Uint32,
             VGImage, Uint32,
             glyphOrigin, Object, escapement, Object);

  Local<Object> glyphOriginArray = args[3]->ToObject();
  Handle<Object> glyphOriginBuffer = glyphOriginArray->Get(String::New("buffer"))->ToObject();

  Local<Object> escapementArray = args[4]->ToObject();
  Handle<Object> escapementBuffer = escapementArray->Get(String::New("buffer"))->ToObject();

  vgSetGlyphToImage((VGFont) args[0]->Uint32Value(),
                    (VGuint) args[1]->Uint32Value(),
                    (VGImage) args[2]->Uint32Value(),
                    (VGfloat*) glyphOriginBuffer->GetIndexedPropertiesExternalArrayData(),
                    (VGfloat*) escapementBuffer->GetIndexedPropertiesExternalArrayData());

  return Undefined();
}

Handle<Value> openvg::ClearGlyph(const Arguments& args) {
  HandleScope scope;

  CheckArgs2(clearGlyph, VGFont, Number, glyphIndex, Uint32);

  vgClearGlyph((VGFont) args[0]->Uint32Value(),
               (VGuint) args[1]->Uint32Value());

  return Undefined();
}

Handle<Value> openvg::DrawGlyph(const Arguments& args) {
  HandleScope scope;

  CheckArgs4(drawGlyph, VGFont, Number, glyphIndex, Uint32,
             paintModes, Uint32, allowAutoHinting, Boolean);

  vgDrawGlyph((VGFont) args[0]->Uint32Value(),
              (VGuint) args[1]->Uint32Value(),
              (VGbitfield) args[2]->Uint32Value(),
              (VGboolean) args[3]->BooleanValue());

  return Undefined();
}

Handle<Value> openvg::DrawGlyphs(const Arguments& args) {
  HandleScope scope;

  CheckArgs7(drawGlyphs, VGFont, Number, glyphCount, Int32,
             glyphIndices, Object, adjustments_x, Object, adjustments_y, Object,
             paintModes, Uint32, allowAutoHinting, Boolean);

  Local<Object> glyphIndicesArray = args[2]->ToObject();
  Handle<Object> glyphIndicesBuffer = glyphIndicesArray->Get(String::New("buffer"))->ToObject();

  Local<Object> adjustments_xArray = args[3]->ToObject();
  Handle<Object> adjustments_xBuffer = adjustments_xArray->Get(String::New("buffer"))->ToObject();

  Local<Object> adjustments_yArray = args[4]->ToObject();
  Handle<Object> adjustments_yBuffer = adjustments_yArray->Get(String::New("buffer"))->ToObject();

  vgDrawGlyphs((VGFont) args[0]->Uint32Value(),
               (VGuint) args[1]->Uint32Value(),
               (VGuint*) glyphIndicesBuffer->GetIndexedPropertiesExternalArrayData(),
               (VGfloat*) adjustments_xBuffer->GetIndexedPropertiesExternalArrayData(),
               (VGfloat*) adjustments_yBuffer->GetIndexedPropertiesExternalArrayData(),
               (VGbitfield) args[5]->Uint32Value(),
               (VGboolean) args[6]->BooleanValue());

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
