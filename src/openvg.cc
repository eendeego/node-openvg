#include <stdio.h>

#include "VG/openvg.h"
#include "VG/vgu.h"
#include "VG/vgext.h"

#include <v8.h>
#include <node.h>
#include <node_buffer.h>

#include "openvg.h"
#include "egl.h"
#include "argchecks.h"

#include "v8_helpers.h"

const bool kInitOpenGLES = false;

ISOLATE(openvg_isolate)

using namespace node;
using namespace v8;

namespace openvg {

static Persistent<FunctionTemplate> handlewrap_constructor;

void DeclareClass(Handle<Object> target, const char* name,
                  Persistent<FunctionTemplate> &constructorTemplate,
                  InvocationCallback constructorCallback,
                  InvocationCallback destructorCallback) {
  SCOPE(openvg_isolate);

  // Prepare constructor template
  Local<FunctionTemplate> constructor = FunctionTemplate::New(constructorCallback);
  constructor->SetClassName(String::NewSymbol(name));
  // constructor->InstanceTemplate()->SetInternalFieldCount(1);

  SET_TEMPLATE(constructorTemplate, openvg_isolate, constructor);

  // Prototype
  Local<Template> proto = constructor->PrototypeTemplate();
  proto->Set("destroy", FunctionTemplate::New(destructorCallback));
}

}

void init(Handle<Object> target) {
  ISOLATE_INIT(openvg_isolate)
  SCOPE(openvg_isolate);

  NODE_SET_METHOD(target, "startUp"          , openvg::StartUp);
  NODE_SET_METHOD(target, "shutdown"         , openvg::Shutdown);

  NODE_SET_METHOD(target, "getError"         , openvg::GetError);

  NODE_SET_METHOD(target, "flush"            , openvg::Flush);
  NODE_SET_METHOD(target, "finish"           , openvg::Finish);

  /* Getters and Setters */
  NODE_SET_METHOD(target, "setF"             , openvg::SetF);
  NODE_SET_METHOD(target, "setI"             , openvg::SetI);
  NODE_SET_METHOD(target, "setFV"            , openvg::SetFV);
  NODE_SET_METHOD(target, "setIV"            , openvg::SetIV);
  NODE_SET_METHOD(target, "setFVOL"          , openvg::SetFVOL); // Offset + Length
  NODE_SET_METHOD(target, "setIVOL"          , openvg::SetIVOL); // Offset + Length

  NODE_SET_METHOD(target, "getF"             , openvg::GetF);
  NODE_SET_METHOD(target, "getI"             , openvg::GetI);
  NODE_SET_METHOD(target, "getVectorSize"    , openvg::GetVectorSize);
  NODE_SET_METHOD(target, "getFV"            , openvg::GetFV);
  NODE_SET_METHOD(target, "getIV"            , openvg::GetIV);
  NODE_SET_METHOD(target, "getFVOL"          , openvg::GetFVOL); // Offset + Length
  NODE_SET_METHOD(target, "getIVOL"          , openvg::GetIVOL); // Offset + Length

  NODE_SET_METHOD(target, "setParameterF"    , openvg::SetParameterF);
  NODE_SET_METHOD(target, "setParameterI"    , openvg::SetParameterI);
  NODE_SET_METHOD(target, "setParameterFV"   , openvg::SetParameterFV);
  NODE_SET_METHOD(target, "setParameterIV"   , openvg::SetParameterIV);
  NODE_SET_METHOD(target, "setParameterFVOL" , openvg::SetParameterFVOL);
  NODE_SET_METHOD(target, "setParameterIVOL" , openvg::SetParameterIVOL);

  NODE_SET_METHOD(target, "getParameterF"    , openvg::GetParameterF);
  NODE_SET_METHOD(target, "getParameterI"    , openvg::GetParameterI);
  NODE_SET_METHOD(target, "getParameterVectorSize", openvg::GetParameterVectorSize);
  NODE_SET_METHOD(target, "getParameterFV"   , openvg::GetParameterFV);
  NODE_SET_METHOD(target, "getParameterIV"   , openvg::GetParameterIV);
  NODE_SET_METHOD(target, "getParameterFVOL" , openvg::GetParameterFVOL); // Offset + Length
  NODE_SET_METHOD(target, "getParameterIVOL" , openvg::GetParameterIVOL); // Offset + Length

  /* Matrix Manipulation */
  NODE_SET_METHOD(target, "loadIdentity"     , openvg::LoadIdentity);
  NODE_SET_METHOD(target, "loadMatrix"       , openvg::LoadMatrix);
  NODE_SET_METHOD(target, "getMatrix"        , openvg::GetMatrix);
  NODE_SET_METHOD(target, "multMatrix"       , openvg::MultMatrix);
  NODE_SET_METHOD(target, "translate"        , openvg::Translate);
  NODE_SET_METHOD(target, "scale"            , openvg::Scale);
  NODE_SET_METHOD(target, "shear"            , openvg::Shear);
  NODE_SET_METHOD(target, "rotate"           , openvg::Rotate);

  /* Masking and Clearing */
  NODE_SET_METHOD(target, "mask"             , openvg::Mask);
  NODE_SET_METHOD(target, "renderToMask"     , openvg::RenderToMask);
  NODE_SET_METHOD(target, "createMaskLayer"  , openvg::CreateMaskLayer);
  NODE_SET_METHOD(target, "destroyMaskLayer" , openvg::DestroyMaskLayer);
  NODE_SET_METHOD(target, "fillMaskLayer"    , openvg::FillMaskLayer);
  NODE_SET_METHOD(target, "copyMask"         , openvg::CopyMask);
  NODE_SET_METHOD(target, "clear"            , openvg::Clear);

  /* Paths */
  NODE_SET_METHOD(target, "createPath"       , openvg::CreatePath);
  NODE_SET_METHOD(target, "clearPath"        , openvg::ClearPath);
  NODE_SET_METHOD(target, "destroyPath"      , openvg::DestroyPath);
  NODE_SET_METHOD(target, "removePathCapabilities",
                          openvg::RemovePathCapabilities);
  NODE_SET_METHOD(target, "getPathCapabilities",
                          openvg::GetPathCapabilities);
  NODE_SET_METHOD(target, "appendPath"       , openvg::AppendPath);
  NODE_SET_METHOD(target, "appendPathData"   , openvg::AppendPathData);
  NODE_SET_METHOD(target, "appendPathDataO"  , openvg::AppendPathDataO); // Offsets
  NODE_SET_METHOD(target, "modifyPathCoords" , openvg::ModifyPathCoords);
  NODE_SET_METHOD(target, "transformPath"    , openvg::TransformPath);
  NODE_SET_METHOD(target, "interpolatePath"  , openvg::InterpolatePath);
  NODE_SET_METHOD(target, "pathLength"       , openvg::PathLength);
  NODE_SET_METHOD(target, "pointAlongPath"   , openvg::PointAlongPath);
  NODE_SET_METHOD(target, "pathBounds"       , openvg::PathBounds);
  NODE_SET_METHOD(target, "pathTransformedBounds",
                          openvg::PathTransformedBounds);
  NODE_SET_METHOD(target, "drawPath"         , openvg::DrawPath);

  /* Paint */
  NODE_SET_METHOD(target, "createPaint"      , openvg::CreatePaint);
  NODE_SET_METHOD(target, "destroyPaint"     , openvg::DestroyPaint);
  NODE_SET_METHOD(target, "setPaint"         , openvg::SetPaint);
  NODE_SET_METHOD(target, "getPaint"         , openvg::GetPaint);
  NODE_SET_METHOD(target, "setColor"         , openvg::SetColor);
  NODE_SET_METHOD(target, "getColor"         , openvg::GetColor);
  NODE_SET_METHOD(target, "paintPattern"     , openvg::PaintPattern);

  /* Images */
  NODE_SET_METHOD(target, "createImage"      , openvg::CreateImage);
  NODE_SET_METHOD(target, "destroyImage"     , openvg::DestroyImage);
  NODE_SET_METHOD(target, "clearImage"       , openvg::ClearImage);
  NODE_SET_METHOD(target, "imageSubData"     , openvg::ImageSubData);
  NODE_SET_METHOD(target, "getImageSubData"  , openvg::GetImageSubData);
  NODE_SET_METHOD(target, "childImage"       , openvg::ChildImage);
  NODE_SET_METHOD(target, "getParent"        , openvg::GetParent);
  NODE_SET_METHOD(target, "copyImage"        , openvg::CopyImage);
  NODE_SET_METHOD(target, "drawImage"        , openvg::DrawImage);
  NODE_SET_METHOD(target, "setPixels"        , openvg::SetPixels);
  NODE_SET_METHOD(target, "writePixels"      , openvg::WritePixels);
  NODE_SET_METHOD(target, "getPixels"        , openvg::GetPixels);
  NODE_SET_METHOD(target, "readPixels"       , openvg::ReadPixels);
  NODE_SET_METHOD(target, "copyPixels"       , openvg::CopyPixels);

  /* Text */
  NODE_SET_METHOD(target, "createFont"       , openvg::CreateFont);
  NODE_SET_METHOD(target, "destroyFont"      , openvg::DestroyFont);
  NODE_SET_METHOD(target, "setGlyphToPath"   , openvg::SetGlyphToPath);
  NODE_SET_METHOD(target, "setGlyphToImage"  , openvg::SetGlyphToImage);
  NODE_SET_METHOD(target, "clearGlyph"       , openvg::ClearGlyph);
  NODE_SET_METHOD(target, "drawGlyph"        , openvg::DrawGlyph);
  NODE_SET_METHOD(target, "drawGlyphs"       , openvg::DrawGlyphs);

  /* Image Filters */
  NODE_SET_METHOD(target, "colorMatrix"      , openvg::ColorMatrix);
  NODE_SET_METHOD(target, "convolve"         , openvg::Convolve);
  NODE_SET_METHOD(target, "separableConvolve", openvg::SeparableConvolve);
  NODE_SET_METHOD(target, "gaussianBlur"     , openvg::GaussianBlur);
  NODE_SET_METHOD(target, "lookup"           , openvg::Lookup);
  NODE_SET_METHOD(target, "lookupSingle"     , openvg::LookupSingle);

  /* Hardware Queries */
  NODE_SET_METHOD(target, "hardwareQuery"    , openvg::HardwareQuery);

  /* Renderer and Extension Information */
  NODE_SET_METHOD(target, "getString"        , openvg::GetString);

  /* Utilities */
  Local<Object> VGU = Object::New();
  target->Set(String::New("vgu"), VGU);

  NODE_SET_METHOD(VGU, "line"                   , openvg::vgu::Line);
  NODE_SET_METHOD(VGU, "polygon"                , openvg::vgu::Polygon);
  NODE_SET_METHOD(VGU, "rect"                   , openvg::vgu::Rect);
  NODE_SET_METHOD(VGU, "roundRect"              , openvg::vgu::RoundRect);
  NODE_SET_METHOD(VGU, "ellipse"                , openvg::vgu::Ellipse);
  NODE_SET_METHOD(VGU, "arc"                    , openvg::vgu::Arc);
  NODE_SET_METHOD(VGU, "computeWarpQuadToSquare",
                       openvg::vgu::ComputeWarpQuadToSquare);
  NODE_SET_METHOD(VGU, "computeWarpSquareToQuad",
                       openvg::vgu::ComputeWarpSquareToQuad);
  NODE_SET_METHOD(VGU, "computeWarpQuadToQuad",
                       openvg::vgu::ComputeWarpQuadToQuad);

  /* KHR extensions */
  Local<Object> ext = Object::New();
  target->Set(String::New("ext"), ext);

  NODE_SET_METHOD(ext, "createEGLImageTargetKHR",
                       openvg::ext::CreateEGLImageTargetKHR);

  NODE_SET_METHOD(ext, "iterativeAverageBlurKHR",
                       openvg::ext::IterativeAverageBlurKHR);

  NODE_SET_METHOD(ext, "parametricFilterKHR", openvg::ext::ParametricFilterKHR);
  NODE_SET_METHOD(ext, "dropShadowKHR"      , openvg::ext::DropShadowKHR);
  NODE_SET_METHOD(ext, "glowKHR"            , openvg::ext::GlowKHR);
  NODE_SET_METHOD(ext, "bevelKHR"           , openvg::ext::BevelKHR);
  NODE_SET_METHOD(ext, "gradientGlowKHR"    , openvg::ext::GradientGlowKHR);
  NODE_SET_METHOD(ext, "gradientBevelKHR"   , openvg::ext::GradientBevelKHR);

  NODE_SET_METHOD(ext, "projectiveMatrixNDS",
                       openvg::ext::ProjectiveMatrixNDS);
  NODE_SET_METHOD(ext, "transformClipLineNDS",
                       openvg::ext::TransformClipLineNDS);

  /* EGL */
  Local<Object> egl = Object::New();
  target->Set(String::New("egl"), egl);
  egl::InitBindings(egl);

  DeclareClass(target, "VGHandleWrap",
               openvg::handlewrap_constructor,
               openvg::VGHandleWrap::New,
               openvg::VGHandleWrap::Destroy);
}
NODE_MODULE(openvg, init)

#define CHECK_VG_ERROR {\
    VGErrorCode errorCode = vgGetError();\
    if(errorCode != VG_NO_ERROR) {\
      char buffer[100];\
      snprintf(&buffer[0], sizeof(buffer), "vgGetError: 0x%04x", errorCode);\
      __assert_fail (buffer, __FILE__, __LINE__, __PRETTY_FUNCTION__);\
    }\
  }


// Handle wrapper


void openvg::VGNoopDestroy(VGHandle handle) {};

// Constructor, never used directly
openvg::VGHandleWrap::VGHandleWrap() {
}

// GC Destroy
openvg::VGHandleWrap::~VGHandleWrap() {
  SCOPE(openvg_isolate);
  Destroy();
}

// Return the Handle
VGHandle openvg::VGHandleWrap::Handle() {
  SCOPE(openvg_isolate);
  return vghandle_;
}

// JS Constructor function (template)
// Only for internal usage (not from JS space)
// TODO check for args.IsConstructCall()
V8_METHOD(openvg::VGHandleWrap::New) {
  SCOPE(openvg_isolate);

  openvg::VGHandleWrap* wrapper = new openvg::VGHandleWrap();
  wrapper->Wrap(args.This());

  V8_RETURN(args.This());
}

// Explicit destroy on handle via JS
V8_METHOD(openvg::VGHandleWrap::Destroy) {
  SCOPE(openvg_isolate);

  openvg::VGHandleWrap *wrap = ObjectWrap::Unwrap<openvg::VGHandleWrap>(args.This());
  wrap->Destroy();

  V8_RETURN(Undefined());
}

// Creation (internal only)
openvg::VGHandleWrap* openvg::VGHandleWrap::New(VGHandle handle, VGDestroyFn destructor) {
  SCOPE(openvg_isolate);

  Local<Object> jsObj = NEW_INSTANCE(openvg_isolate, handlewrap_constructor);

  openvg::VGHandleWrap *wrap = ObjectWrap::Unwrap<openvg::VGHandleWrap>(jsObj);
  wrap->vghandle_     = handle;
  wrap->vgdestructor_ = destructor;

  return wrap;
}

// Explicit destroy on VG api
void openvg::VGHandleWrap::Destroy() {
  SCOPE(openvg_isolate);

  if (vghandle_) {
    vgdestructor_(vghandle_);
    vghandle_ = 0;
  }
}


V8_METHOD(openvg::StartUp) {
  HandleScope scope;

  CheckArgs1(startUp, screen, Object);

  egl::Init();

  if (kInitOpenGLES) {
    egl::InitOpenGLES();
  }

  CHECK_VG_ERROR;

  Local<Object> screen = args[0].As<Object>();
  screen->Set(String::NewSymbol("width"  ), Integer::New(egl::State.screen_width));
  screen->Set(String::NewSymbol("height" ), Integer::New(egl::State.screen_height));
  screen->Set(String::NewSymbol("display"), External::New(egl::State.display));
  screen->Set(String::NewSymbol("surface"), External::New(egl::State.surface));
  screen->Set(String::NewSymbol("context"), External::New(egl::State.context));

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::Shutdown) {
  HandleScope scope;

  CheckArgs0(shutdown);

  egl::Finish();

  V8_RETURN(Undefined());
}


V8_METHOD(openvg::GetError) {
  HandleScope scope;

  CheckArgs0(getError);

  V8_RETURN(Integer::New(vgGetError()));
}


V8_METHOD(openvg::Flush) {
  HandleScope scope;

  CheckArgs0(flush);

  vgFlush();

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::Finish) {
  HandleScope scope;

  CheckArgs0(finish);

  vgFinish();

  V8_RETURN(Undefined());
}


/* Getters and Setters */


V8_METHOD(openvg::SetF) {
  HandleScope scope;

  CheckArgs2(setF, type, Int32, value, Number);

  vgSetf((VGParamType) args[0]->Int32Value(),
         (VGfloat) args[1]->NumberValue());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::SetI) {
  HandleScope scope;

  CheckArgs2(setI, type, Int32, value, Int32);

  vgSeti((VGParamType) args[0]->Int32Value(),
         (VGint) args[1]->Int32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::SetFV) {
  HandleScope scope;

  CheckArgs2(setFV, type, Int32, Float32Array, Object);

  TypedArrayWrapper<VGfloat> values(args[1]);

  vgSetfv((VGParamType) args[0]->Int32Value(),
          values.length(),
          values.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::SetIV) {
  HandleScope scope;

  CheckArgs2(setIV, type, Int32, Int32Array, Object);

  TypedArrayWrapper<VGint> values(args[1]);

  vgSetiv((VGParamType) args[0]->Int32Value(),
          values.length(),
          values.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::SetFVOL) {
  HandleScope scope;

  CheckArgs4(setFVOL, type, Int32, Float32Array, Object, offset, Int32, length, Int32);

  TypedArrayWrapper<VGfloat> values(args[1]);

  vgSetfv((VGParamType) args[0]->Int32Value(),
          (VGint) args[3]->Int32Value(),
          values.pointer(args[2]->Int32Value()));

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::SetIVOL) {
  HandleScope scope;

  CheckArgs4(setIV, type, Int32, Int32Array, Object, offset, Int32, length, Int32);

  TypedArrayWrapper<VGint> values(args[1]);

  vgSetiv((VGParamType) args[0]->Int32Value(),
          (VGint) args[3]->Int32Value(),
          values.pointer(args[2]->Int32Value()));

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::GetF) {
  HandleScope scope;

  CheckArgs1(getF, type, Int32);

  V8_RETURN(Number::New(vgGetf((VGParamType) args[0]->Int32Value())));
}

V8_METHOD(openvg::GetI) {
  HandleScope scope;

  CheckArgs1(getI, type, Int32);

  V8_RETURN(Integer::New(vgGeti((VGParamType) args[0]->Int32Value())));
}

V8_METHOD(openvg::GetVectorSize) {
  HandleScope scope;

  CheckArgs1(getVectorSize, type, Int32);

  V8_RETURN(Integer::New(vgGetVectorSize((VGParamType) args[0]->Int32Value())));
}

V8_METHOD(openvg::GetFV) {
  HandleScope scope;

  CheckArgs2(getFV, type, Int32, Float32Array, Object);

  TypedArrayWrapper<VGfloat> values(args[1]);

  vgGetfv((VGParamType) args[0]->Int32Value(),
          values.length(),
          values.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::GetIV) {
  HandleScope scope;

  CheckArgs2(getIV, type, Int32, Float32Array, Object);

  TypedArrayWrapper<VGint> values(args[1]);

  vgGetiv((VGParamType) args[0]->Int32Value(),
          values.length(),
          values.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::GetFVOL) {
  HandleScope scope;

  CheckArgs4(getFV, type, Int32, Float32Array, Object, offset, Int32, length, Int32);

  TypedArrayWrapper<VGfloat> values(args[1]);

  vgGetfv((VGParamType) args[0]->Int32Value(),
          (VGint) args[3]->Int32Value(),
          values.pointer(args[2]->Int32Value()));

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::GetIVOL) {
  HandleScope scope;

  CheckArgs4(getIV, type, Int32, Float32Array, Object, offset, Int32, length, Int32);

  TypedArrayWrapper<VGint> values(args[1]);

  vgGetiv((VGParamType) args[0]->Int32Value(),
          (VGint) args[3]->Int32Value(),
          values.pointer(args[2]->Int32Value()));

  V8_RETURN(Undefined());
}


V8_METHOD(openvg::SetParameterF) {
  HandleScope scope;

  CheckArgs3(setParameterF, VGHandle, Object, VGParamType, Int32, value, Number);

  vgSetParameterf(UNWRAPPED_HANDLE(VGHandle, args[0]->ToObject()),
                  (VGParamType) args[1]->Int32Value(),
                  (VGfloat) args[2]->NumberValue());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::SetParameterI) {
  HandleScope scope;

  CheckArgs3(setParameterI, VGHandle, Object, VGParamType, Int32, value, Int32);

  vgSetParameteri(UNWRAPPED_HANDLE(VGHandle, args[0]->ToObject()),
                  (VGParamType) args[1]->Int32Value(),
                  (VGint) args[2]->Int32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::SetParameterFV) {
  HandleScope scope;

  CheckArgs3(setParameterFV,
             VGHandle, Object, VGParamType, Int32, Float32Array, Object);

  TypedArrayWrapper<VGfloat> values(args[2]);

  vgSetParameterfv(UNWRAPPED_HANDLE(VGHandle, args[0]->ToObject()),
                   (VGParamType) args[1]->Int32Value(),
                   values.length(),
                   values.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::SetParameterIV) {
  HandleScope scope;

  CheckArgs3(setParameterIV,
             VGHandle, Object, VGParamType, Int32, Int32Array, Object);

  TypedArrayWrapper<VGint> values(args[2]);

  vgSetParameteriv(UNWRAPPED_HANDLE(VGHandle, args[0]->ToObject()),
                   (VGParamType) args[1]->Int32Value(),
                   values.length(),
                   values.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::SetParameterFVOL) {
  HandleScope scope;

  CheckArgs5(setParameterFV,
             VGHandle, Object, VGParamType, Int32, Float32Array, Object,
             offset, Int32, length, Int32);

  TypedArrayWrapper<VGfloat> values(args[2]);

  vgSetParameterfv(UNWRAPPED_HANDLE(VGHandle, args[0]->ToObject()),
                   (VGParamType) args[1]->Int32Value(),
                   (VGint) args[4]->Int32Value(),
                   values.pointer(args[3]->Int32Value()));

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::SetParameterIVOL) {
  HandleScope scope;

  CheckArgs5(setParameterIV,
             VGHandle, Object, VGParamType, Int32, Int32Array, Object,
             offset, Int32, length, Int32);

  TypedArrayWrapper<VGint> values(args[2]);

  vgSetParameteriv(UNWRAPPED_HANDLE(VGHandle, args[0]->ToObject()),
                   (VGParamType) args[1]->Int32Value(),
                   (VGint) args[4]->Int32Value(),
                   values.pointer(args[3]->Int32Value()));

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::GetParameterF) {
  HandleScope scope;

  CheckArgs2(getParameterF, VGHandle, Object, VGParamType, Int32);

  V8_RETURN(Number::New(vgGetParameterf(UNWRAPPED_HANDLE(VGHandle, args[0]->ToObject()),
                                        (VGParamType) args[1]->Int32Value())));
}

V8_METHOD(openvg::GetParameterI) {
  HandleScope scope;

  CheckArgs2(getParameterI, VGHandle, Object, VGParamType, Int32);

  V8_RETURN(Integer::New(vgGetParameteri(UNWRAPPED_HANDLE(VGHandle, args[0]->ToObject()),
                                         (VGParamType) args[1]->Int32Value())));
}

V8_METHOD(openvg::GetParameterVectorSize) {
  HandleScope scope;

  CheckArgs2(getParameterVectorSize, VGHandle, Object, VGParamType, Int32);

  V8_RETURN(Integer::New(vgGetParameterVectorSize(UNWRAPPED_HANDLE(VGHandle, args[0]->ToObject()),
                                                  (VGParamType) args[1]->Int32Value())));
}

V8_METHOD(openvg::GetParameterFV) {
  HandleScope scope;

  CheckArgs3(getParameterFV,
             VGHandle, Object, VGParamType, Int32, Float32Array, Object);

  TypedArrayWrapper<VGfloat> values(args[2]);

  vgGetParameterfv(UNWRAPPED_HANDLE(VGHandle, args[0]->ToObject()),
                   (VGParamType) args[1]->Int32Value(),
                   values.length(),
                   values.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::GetParameterIV) {
  HandleScope scope;

  CheckArgs3(getParameterIV,
             VGHandle, Object, VGParamType, Int32, Int32Array, Object);

  TypedArrayWrapper<VGint> values(args[2]);

  vgGetParameteriv(UNWRAPPED_HANDLE(VGHandle, args[0]->ToObject()),
                   (VGParamType) args[1]->Int32Value(),
                   values.length(),
                   values.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::GetParameterFVOL) {
  HandleScope scope;

  CheckArgs5(getParameterFV,
             VGHandle, Object, VGParamType, Int32, Float32Array, Object,
             offset, Int32, length, Int32);

  TypedArrayWrapper<VGfloat> values(args[2]);

  vgGetParameterfv(UNWRAPPED_HANDLE(VGHandle, args[0]->ToObject()),
                   (VGParamType) args[1]->Int32Value(),
                   (VGint) args[4]->Int32Value(),
                   values.pointer(args[3]->Int32Value()));

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::GetParameterIVOL) {
  HandleScope scope;

  CheckArgs5(getParameterIV,
             VGHandle, Object, VGParamType, Int32, Int32Array, Object,
             offset, Int32, length, Int32);

  TypedArrayWrapper<VGint> values(args[2]);

  vgGetParameteriv(UNWRAPPED_HANDLE(VGHandle, args[0]->ToObject()),
                   (VGParamType) args[1]->Int32Value(),
                   (VGint) args[4]->Int32Value(),
                   values.pointer(args[3]->Int32Value()));

  V8_RETURN(Undefined());
}


/* Matrix Manipulation */


V8_METHOD(openvg::LoadIdentity) {
  HandleScope scope;

  CheckArgs0(loadIdentity);

  vgLoadIdentity();

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::LoadMatrix) {
  HandleScope scope;

  CheckArgs1(loadIdentity, Float32Array, Object);

  TypedArrayWrapper<VGfloat> matrix(args[0]);

  vgLoadMatrix(matrix.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::GetMatrix) {
  HandleScope scope;

  CheckArgs1(getMatrix, Float32Array, Object);

  TypedArrayWrapper<VGfloat> matrix(args[0]);

  vgGetMatrix(matrix.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::MultMatrix) {
  HandleScope scope;

  CheckArgs1(multMatrix, Float32Array, Object);

  TypedArrayWrapper<VGfloat> matrix(args[0]);

  vgMultMatrix(matrix.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::Translate) {
  HandleScope scope;

  CheckArgs2(translate, x, Number, y, Number);

  vgTranslate((VGfloat) args[0]->NumberValue(),
              (VGfloat) args[1]->NumberValue());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::Scale) {
  HandleScope scope;

  CheckArgs2(scale, x, Number, y, Number);

  vgScale((VGfloat) args[0]->NumberValue(),
          (VGfloat) args[1]->NumberValue());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::Shear) {
  HandleScope scope;

  CheckArgs2(shear, x, Number, y, Number);

  vgShear((VGfloat) args[0]->NumberValue(),
          (VGfloat) args[1]->NumberValue());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::Rotate) {
  HandleScope scope;

  CheckArgs1(shear, angle, Number);

  vgRotate((VGfloat) args[0]->NumberValue());

  V8_RETURN(Undefined());
}


/* Masking and Clearing */


V8_METHOD(openvg::Mask) {
  HandleScope scope;

  CheckArgs6(mask,
             VGHandle, Object, VGMaskOperation, Uint32,
             x, Int32, y, Int32, width, Int32, height, Int32);

  vgMask(UNWRAPPED_HANDLE(VGHandle, args[0]->ToObject()),
         static_cast<VGMaskOperation>(args[1]->Uint32Value()),
         (VGint) args[2]->Int32Value(),
         (VGint) args[3]->Int32Value(),
         (VGint) args[4]->Int32Value(),
         (VGint) args[5]->Int32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::RenderToMask) {
  HandleScope scope;

  CheckArgs3(renderToMask,
             VGPath, Object,
             VGbitfield, Uint32,
             VGMaskOperation, Uint32);

  vgRenderToMask(UNWRAPPED_HANDLE(VGPath, args[0]->ToObject()),
                 (VGbitfield) args[1]->Uint32Value(),
                 (VGMaskOperation) args[2]->Uint32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::CreateMaskLayer) {
  HandleScope scope;

  CheckArgs2(createMaskLayer, width, Int32, height, Int32);

  VGMaskLayer mask = vgCreateMaskLayer((VGint) args[0]->Int32Value(),
                                       (VGint) args[1]->Int32Value());

  V8_RETURN(V8_PERSISTENT(VGHandleWrap::New(mask, (VGDestroyFn) &vgDestroyMaskLayer)));
}

V8_METHOD(openvg::DestroyMaskLayer) {
  HandleScope scope;

  CheckArgs1(destroyMaskLayer, VGMaskLayer, Object);

  ObjectWrap::Unwrap<VGHandleWrap>(args[0]->ToObject())->Destroy();

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::FillMaskLayer) {
  HandleScope scope;

  CheckArgs6(fillMaskLayer,
             VGMaskLayer, Object,
             x, Int32, y, Int32, width, Int32, height, Int32,
             value, Number);

  vgFillMaskLayer(UNWRAPPED_HANDLE(VGMaskLayer, args[0]->ToObject()),
                  (VGint) args[1]->Int32Value(),
                  (VGint) args[2]->Int32Value(),
                  (VGint) args[3]->Int32Value(),
                  (VGint) args[4]->Int32Value(),
                  (VGfloat) args[5]->NumberValue());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::CopyMask) {
  HandleScope scope;

  CheckArgs7(fillMaskLayer,
             VGMaskLayer, Object,
             dx, Int32, dy, Int32, sx, Int32, sy, Int32,
             width, Int32, height, Int32);

  vgCopyMask(UNWRAPPED_HANDLE(VGMaskLayer, args[0]->ToObject()),
             (VGint) args[1]->Int32Value(), (VGint) args[2]->Int32Value(),
             (VGint) args[3]->Int32Value(), (VGint) args[4]->Int32Value(),
             (VGint) args[5]->Int32Value(), (VGint) args[6]->Int32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::Clear) {
  HandleScope scope;

  CheckArgs4(clear, x, Int32, y, Int32, width, Int32, height, Int32);

  vgClear((VGint) args[0]->Int32Value(), (VGint) args[1]->Int32Value(),
          (VGint) args[2]->Int32Value(), (VGint) args[3]->Int32Value());

  V8_RETURN(Undefined());
}


/* Paths */


V8_METHOD(openvg::CreatePath) {
  HandleScope scope;

  CheckArgs7(createPath,
             pathFormat, Int32, VGPathDatatype, Uint32,
             scale, Number, bias, Number, segmentCapacityHint, Int32,
             coordCapacityHint, Int32, capabilities, Uint32);

  VGPath path = vgCreatePath((VGint) args[0]->Int32Value(),
                             static_cast<VGPathDatatype>(args[1]->Uint32Value()),
                             (VGfloat) args[2]->NumberValue(),
                             (VGfloat) args[3]->NumberValue(),
                             (VGint) args[4]->Int32Value(),
                             (VGint) args[5]->Int32Value(),
                             (VGbitfield) args[6]->Uint32Value());

  V8_RETURN(V8_PERSISTENT(VGHandleWrap::New(path, (VGDestroyFn) &vgDestroyPath)));
}

V8_METHOD(openvg::ClearPath) {
  HandleScope scope;

  CheckArgs2(clearPath, VGPath, Object, capabilities, Uint32);

  vgClearPath(UNWRAPPED_HANDLE(VGMaskLayer, args[0]->ToObject()),
              (VGbitfield) args[1]->Uint32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::DestroyPath) {
  HandleScope scope;

  CheckArgs1(destroyPath, VGPath, Object);

  ObjectWrap::Unwrap<VGHandleWrap>(args[0]->ToObject())->Destroy();

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::RemovePathCapabilities) {
  HandleScope scope;

  CheckArgs2(removePathCapabilities, VGPath, Object, capabilities, Uint32);

  vgRemovePathCapabilities(UNWRAPPED_HANDLE(VGPath, args[0]->ToObject()),
                           (VGbitfield) args[1]->Uint32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::GetPathCapabilities) {
  HandleScope scope;

  CheckArgs1(getPathCapabilities, VGPath, Object);

  V8_RETURN(Uint32::New(vgGetPathCapabilities(UNWRAPPED_HANDLE(VGPath, args[0]->ToObject()))));
}

V8_METHOD(openvg::AppendPath) {
  HandleScope scope;

  CheckArgs2(appendPath, dstPath, Object, srcPath, Object);

  vgAppendPath(UNWRAPPED_HANDLE(VGPath, args[0]->ToObject()),
               UNWRAPPED_HANDLE(VGPath, args[1]->ToObject()));

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::AppendPathData) {
  HandleScope scope;

  CheckArgs4(appendPathData,
             dstPath, Object, numSegments, Int32, Uint8Array, Object,
             pathData, Object);

  TypedArrayWrapper<VGubyte> segments(args[2]);
  TypedArrayWrapper<void> data(args[3]);

  vgAppendPathData(UNWRAPPED_HANDLE(VGPath, args[0]->ToObject()),
                   (VGint) args[1]->Int32Value(),
                   segments.pointer(),
                   data.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::AppendPathDataO) {
  HandleScope scope;

  CheckArgs4(appendPathData,
             dstPath, Object, numSegments, Int32, Uint8Array, Object,
             pathData, Object);

  TypedArrayWrapper<VGubyte> segments(args[2]);
  TypedArrayWrapper<void> data(args[4]);

  vgAppendPathData(UNWRAPPED_HANDLE(VGPath, args[0]->ToObject()),
                   (VGint) args[1]->Int32Value(),
                   segments.pointer(args[3]->Uint32Value()),
                   data.pointer(args[5]->Int32Value()));

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::ModifyPathCoords) {
  HandleScope scope;

  CheckArgs4(modifyPathCoords,
             VGPath, Object, startIndex, Int32, numSegments, Int32,
             pathData, Object);

  TypedArrayWrapper<void> data(args[3]);

  vgModifyPathCoords(UNWRAPPED_HANDLE(VGPath, args[0]->ToObject()),
                     (VGint) args[1]->Int32Value(),
                     (VGint) args[2]->Int32Value(),
                     data.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::TransformPath) {
  HandleScope scope;

  CheckArgs2(transformPath, dstPath, Object, srcPath, Object);

  vgTransformPath(UNWRAPPED_HANDLE(VGPath, args[0]->ToObject()),
                  UNWRAPPED_HANDLE(VGPath, args[1]->ToObject()));

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::InterpolatePath) {
  HandleScope scope;

  CheckArgs4(interpolatePath,
             dstPath, Object, startPath, Object, endPath, Object,
             amount, Number);

  V8_RETURN(Boolean::New(vgInterpolatePath(UNWRAPPED_HANDLE(VGPath, args[0]->ToObject()),
                                           UNWRAPPED_HANDLE(VGPath, args[1]->ToObject()),
                                           UNWRAPPED_HANDLE(VGPath, args[2]->ToObject()),
                                           (VGfloat) args[3]->NumberValue())));
}

V8_METHOD(openvg::PathLength) {
  HandleScope scope;

  CheckArgs3(pathLength, path, Object,
             startSegment, Int32, numSegments, Int32);

  V8_RETURN(Number::New(vgPathLength(UNWRAPPED_HANDLE(VGPath, args[0]->ToObject()),
                                     (VGint) args[1]->Int32Value(),
                                     (VGint) args[2]->Int32Value())));
}

V8_METHOD(openvg::PointAlongPath) {
  HandleScope scope;

  CheckArgs5(pointAlongPath, path, Object,
             startSegment, Int32, numSegments, Int32,
             distance, Number, point, Object);

  VGfloat x, y, tx, ty;

  vgPointAlongPath(UNWRAPPED_HANDLE(VGPath, args[0]->ToObject()),
                   (VGint) args[1]->Int32Value(),
                   (VGint) args[2]->Int32Value(),
                   (VGfloat) args[3]->NumberValue(),
                   &x, &y, &tx, &ty);

  Local<Object> point = args[4].As<Object>();
  point->Set(String::NewSymbol("x"), Number::New(x));
  point->Set(String::NewSymbol("y"), Number::New(y));
  point->Set(String::NewSymbol("tx"), Number::New(tx));
  point->Set(String::NewSymbol("ty"), Number::New(ty));

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::PathBounds) {
  HandleScope scope;

  CheckArgs2(pathBounds, VGPath, Object, bounds, Object);

  VGfloat minX, minY, width, height;

  vgPathBounds(UNWRAPPED_HANDLE(VGPath, args[0]->ToObject()),
               &minX, &minY, &width, &height);

  Local<Object> bounds = args[1].As<Object>();
  bounds->Set(String::NewSymbol("x"), Number::New(minX));
  bounds->Set(String::NewSymbol("y"), Number::New(minY));
  bounds->Set(String::NewSymbol("w"), Number::New(width));
  bounds->Set(String::NewSymbol("h"), Number::New(height));

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::PathTransformedBounds) {
  HandleScope scope;

  CheckArgs2(pathTransformedBounds, VGPath, Object, bounds, Object);

  VGfloat minX, minY, width, height;

  vgPathTransformedBounds(UNWRAPPED_HANDLE(VGPath, args[0]->ToObject()),
                          &minX, &minY, &width, &height);

  Local<Object> bounds = args[1].As<Object>();
  bounds->Set(String::NewSymbol("x"), Number::New(minX));
  bounds->Set(String::NewSymbol("y"), Number::New(minY));
  bounds->Set(String::NewSymbol("w"), Number::New(width));
  bounds->Set(String::NewSymbol("h"), Number::New(height));

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::DrawPath) {
  HandleScope scope;

  CheckArgs2(drawPath, VGPath, Object, paintModes, Number);

  vgDrawPath(UNWRAPPED_HANDLE(VGPath, args[0]->ToObject()),
             (VGbitfield) args[1]->Uint32Value());

  V8_RETURN(Undefined());
}


/* Paint */


V8_METHOD(openvg::CreatePaint) {
  HandleScope scope;

  CheckArgs0(createPaint);

  V8_RETURN(V8_PERSISTENT(VGHandleWrap::New(vgCreatePaint(), (VGDestroyFn) &vgDestroyPaint)));
}

V8_METHOD(openvg::DestroyPaint) {
  HandleScope scope;

  CheckArgs1(destroyPaint, VGPaint, Object);

  ObjectWrap::Unwrap<VGHandleWrap>(args[0]->ToObject())->Destroy();

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::SetPaint) {
  HandleScope scope;

  CheckArgs2(setPaint, VGPaint, Object, paintModes, Number);

  vgSetPaint(UNWRAPPED_HANDLE(VGPaint, args[0]->ToObject()),
             (VGbitfield) args[1]->Uint32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::GetPaint) {
  HandleScope scope;

  CheckArgs1(getPaint, VGPaint, Object);

  V8_RETURN(Uint32::New(vgGetPaint(static_cast<VGPaintMode>(args[0]->Uint32Value()))));
}

V8_METHOD(openvg::SetColor) {
  HandleScope scope;

  CheckArgs2(setColor, VGPaint, Object, rgba, Uint32);

  vgSetColor(UNWRAPPED_HANDLE(VGPaint, args[0]->ToObject()),
             (VGuint) args[1]->Uint32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::GetColor) {
  HandleScope scope;

  CheckArgs1(getColor, VGPaint, Object);

  V8_RETURN(Uint32::New(vgGetColor(UNWRAPPED_HANDLE(VGPaint, args[0]->ToObject()))));
}

V8_METHOD(openvg::PaintPattern) {
  HandleScope scope;

  CheckArgs2(paintPattern, VGPaint, Object, VGImage, Uint32);

  vgPaintPattern(UNWRAPPED_HANDLE(VGPaint, args[0]->ToObject()),
                 UNWRAPPED_HANDLE(VGImage, args[1]->ToObject()));

  V8_RETURN(Undefined());
}


/* Images */


V8_METHOD(openvg::CreateImage) {
  HandleScope scope;

  CheckArgs4(createImage,
             VGImageFormat, Uint32, width, Int32, height, Int32,
             allowedQuality, Uint32);

  VGImage image = vgCreateImage(static_cast<VGImageFormat>(args[0]->Uint32Value()),
                                (VGint) args[1]->Int32Value(),
                                (VGint) args[2]->Int32Value(),
                                (VGuint) args[3]->Uint32Value());

  V8_RETURN(V8_PERSISTENT(VGHandleWrap::New(image, (VGDestroyFn) &vgDestroyImage)));
}

V8_METHOD(openvg::DestroyImage) {
  HandleScope scope;

  CheckArgs1(destroyImage, VGImage, Object);

  ObjectWrap::Unwrap<VGHandleWrap>(args[0]->ToObject())->Destroy();

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::ClearImage) {
  HandleScope scope;

  CheckArgs5(clearImage,
             VGImage, Object, x, Int32, y, Int32, width, Int32, height, Int32);

  vgClearImage(UNWRAPPED_HANDLE(VGImage, args[0]->ToObject()),
               (VGint) args[1]->Int32Value(),
               (VGint) args[2]->Int32Value(),
               (VGint) args[3]->Int32Value(),
               (VGint) args[4]->Int32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::ImageSubData) {
  HandleScope scope;

  CheckArgs8(imageSubData,
             VGImage, Object, data, Object, dataStride, Int32,
             dataFormat, Uint32,
             x, Int32, y, Int32, width, Int32, height, Int32);

  Local<Object> data = args[1]->ToObject();
  void *dataPointer;

  Local<Value> nativeBuffer = data->Get(String::New("buffer"));
  if (!nativeBuffer->IsUndefined()) {
    // Native array
    Handle<Object> dataBuffer = nativeBuffer->ToObject();
    dataPointer = (void*) dataBuffer->GetIndexedPropertiesExternalArrayData();
  } else {
    // Node buffer
    dataPointer = (void *) Buffer::Data(data);
  }

  vgImageSubData(UNWRAPPED_HANDLE(VGImage, args[0]->ToObject()),
                 dataPointer,
                 (VGint) args[2]->Int32Value(),
                 static_cast<VGImageFormat>(args[3]->Uint32Value()),
                 (VGint) args[4]->Int32Value(),
                 (VGint) args[5]->Int32Value(),
                 (VGint) args[6]->Int32Value(),
                 (VGint) args[7]->Int32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::GetImageSubData) {
  HandleScope scope;

  CheckArgs8(getImageSubData,
             VGImage, Object, data, Object, dataStride, Int32,
             dataFormat, Uint32,
             x, Int32, y, Int32, width, Int32, height, Int32);

  TypedArrayWrapper<void> data(args[1]);

  vgGetImageSubData(UNWRAPPED_HANDLE(VGImage, args[0]->ToObject()),
                    data.pointer(),
                    (VGint) args[2]->Int32Value(),
                    static_cast<VGImageFormat>(args[3]->Uint32Value()),
                    (VGint) args[4]->Int32Value(),
                    (VGint) args[5]->Int32Value(),
                    (VGint) args[6]->Int32Value(),
                    (VGint) args[7]->Int32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::ChildImage) {
  HandleScope scope;

  CheckArgs5(childImage,
             VGImage, Object, x, Int32, y, Int32, width, Int32, height, Int32);

  VGImage image = vgChildImage(UNWRAPPED_HANDLE(VGImage, args[0]->ToObject()),
                               (VGint) args[1]->Int32Value(),
                               (VGint) args[2]->Int32Value(),
                               (VGint) args[3]->Int32Value(),
                               (VGint) args[4]->Int32Value());

  V8_RETURN(V8_PERSISTENT(VGHandleWrap::New(image, (VGDestroyFn) &vgDestroyImage)));
}

// TODO Implement this in a === compatible way
V8_METHOD(openvg::GetParent) {
  HandleScope scope;

  CheckArgs1(getParent, VGImage, Object);

  VGImage image = vgGetParent(UNWRAPPED_HANDLE(VGImage, args[0]->ToObject()));

  V8_RETURN(V8_PERSISTENT(VGHandleWrap::New(image, (VGDestroyFn) &VGNoopDestroy)));
}

V8_METHOD(openvg::CopyImage) {
  HandleScope scope;

  CheckArgs9(copyImage,
             dstImage, Object, dx, Int32, dy, Int32,
             srcImage, Object, sx, Int32, sy, Int32,
             width, Int32, height, Int32, dither, Boolean);

  vgCopyImage(UNWRAPPED_HANDLE(VGImage, args[0]->ToObject()),
              (VGint) args[1]->Int32Value(),
              (VGint) args[2]->Int32Value(),
              UNWRAPPED_HANDLE(VGImage, args[3]->ToObject()),
              (VGint) args[4]->Int32Value(),
              (VGint) args[5]->Int32Value(),
              (VGint) args[6]->Int32Value(),
              (VGint) args[7]->Int32Value(),
              (VGboolean) args[8]->BooleanValue());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::DrawImage) {
  HandleScope scope;

  CheckArgs1(drawImage, VGImage, Object);

  vgDrawImage(UNWRAPPED_HANDLE(VGImage, args[0]->ToObject()));

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::SetPixels) {
  HandleScope scope;

  CheckArgs7(setPixels,
             dx, Int32, dy, Int32,
             srcImage, Object, dx, Int32, dy, Int32,
             width, Int32, height, Int32);

  vgSetPixels((VGint) args[0]->Int32Value(),
              (VGint) args[1]->Int32Value(),
              UNWRAPPED_HANDLE(VGImage, args[2]->ToObject()),
              (VGint) args[3]->Int32Value(),
              (VGint) args[4]->Int32Value(),
              (VGint) args[5]->Int32Value(),
              (VGint) args[6]->Int32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::WritePixels) {
  HandleScope scope;

  CheckArgs7(writePixels, data, Object, dataStride, Int32,
             dataFormat, Uint32,
             dx, Int32, dy, Int32, width, Int32, height, Int32);

  TypedArrayWrapper<void> data(args[0]);

  vgWritePixels(data.pointer(),
                (VGint) args[1]->Int32Value(),
                static_cast<VGImageFormat>(args[2]->Uint32Value()),
                (VGint) args[3]->Int32Value(),
                (VGint) args[4]->Int32Value(),
                (VGint) args[5]->Int32Value(),
                (VGint) args[6]->Int32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::GetPixels) {
  HandleScope scope;

  CheckArgs7(getPixels,
             VGImage, Object,
             dx, Int32, dy, Int32,
             sx, Int32, sy, Int32,
             width, Int32, height, Int32);

  vgGetPixels(UNWRAPPED_HANDLE(VGImage, args[0]->ToObject()),
              (VGint) args[1]->Int32Value(),
              (VGint) args[2]->Int32Value(),
              (VGint) args[3]->Int32Value(),
              (VGint) args[4]->Int32Value(),
              (VGint) args[5]->Int32Value(),
              (VGint) args[6]->Int32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::ReadPixels) {
  HandleScope scope;

  CheckArgs7(readPixels,
             data, Object, dataStride, Int32, dataFormat, Uint32,
             sx, Int32, sy, Int32, width, Int32, height, Int32);

  TypedArrayWrapper<void> data(args[0]);

  vgReadPixels(data.pointer(),
               (VGint) args[1]->Int32Value(),
               static_cast<VGImageFormat>(args[2]->Uint32Value()),
               (VGint) args[3]->Int32Value(),
               (VGint) args[4]->Int32Value(),
               (VGint) args[5]->Int32Value(),
               (VGint) args[6]->Int32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::CopyPixels) {
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

  V8_RETURN(Undefined());
}


/* Text */


V8_METHOD(openvg::CreateFont) {
  HandleScope scope;

  CheckArgs1(createFont, glyphCapacityHint, Int32);

  VGFont font = vgCreateFont((VGint) args[0]->Int32Value());

  V8_RETURN(V8_PERSISTENT(VGHandleWrap::New(font, (VGDestroyFn) &vgDestroyFont)));
}

V8_METHOD(openvg::DestroyFont) {
  HandleScope scope;

  CheckArgs1(destroyFont, VGFont, Object);

  ObjectWrap::Unwrap<VGHandleWrap>(args[0]->ToObject())->Destroy();

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::SetGlyphToPath) {
  HandleScope scope;

  CheckArgs6(setGlyphToPath, VGFont, Object, glyphIndex, Number,
             VGPath, Object, isHinted, Boolean,
             glyphOrigin, Object, escapement, Object);

  TypedArrayWrapper<VGfloat> glyphOrigin(args[4]);
  TypedArrayWrapper<VGfloat> escapement(args[5]);

  vgSetGlyphToPath(UNWRAPPED_HANDLE(VGFont, args[0]->ToObject()),
                   (VGuint) args[1]->Uint32Value(),
                   UNWRAPPED_HANDLE(VGPath, args[2]->ToObject()),
                   (VGboolean) args[3]->BooleanValue(),
                   glyphOrigin.pointer(),
                   escapement.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::SetGlyphToImage) {
  HandleScope scope;

  CheckArgs5(setGlyphToImage, VGFont, Object, glyphIndex, Number,
             VGImage, Object,
             glyphOrigin, Object, escapement, Object);

  TypedArrayWrapper<VGfloat> glyphOrigin(args[3]);
  TypedArrayWrapper<VGfloat> escapement(args[4]);

  vgSetGlyphToImage(UNWRAPPED_HANDLE(VGFont, args[0]->ToObject()),
                    (VGuint) args[1]->Uint32Value(),
                    UNWRAPPED_HANDLE(VGImage, args[2]->ToObject()),
                    glyphOrigin.pointer(),
                    escapement.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::ClearGlyph) {
  HandleScope scope;

  CheckArgs2(clearGlyph, VGFont, Object, glyphIndex, Uint32);

  vgClearGlyph(UNWRAPPED_HANDLE(VGFont, args[0]->ToObject()),
               (VGuint) args[1]->Uint32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::DrawGlyph) {
  HandleScope scope;

  CheckArgs4(drawGlyph, VGFont, Object, glyphIndex, Uint32,
             paintModes, Uint32, allowAutoHinting, Boolean);

  vgDrawGlyph(UNWRAPPED_HANDLE(VGFont, args[0]->ToObject()),
              (VGuint) args[1]->Uint32Value(),
              (VGbitfield) args[2]->Uint32Value(),
              (VGboolean) args[3]->BooleanValue());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::DrawGlyphs) {
  HandleScope scope;

  CheckArgs7(drawGlyphs, VGFont, Object, glyphCount, Int32,
             glyphIndices, Object, adjustments_x, Object, adjustments_y, Object,
             paintModes, Uint32, allowAutoHinting, Boolean);

  TypedArrayWrapper<VGuint> glyphIndices(args[2]);
  TypedArrayWrapper<VGfloat> adjustments_x(args[3]);
  TypedArrayWrapper<VGfloat> adjustments_y(args[4]);

  vgDrawGlyphs(UNWRAPPED_HANDLE(VGFont, args[0]->ToObject()),
               (VGuint) args[1]->Uint32Value(),
               glyphIndices.pointer(),
               adjustments_x.pointer(),
               adjustments_y.pointer(),
               (VGbitfield) args[5]->Uint32Value(),
               (VGboolean) args[6]->BooleanValue());

  V8_RETURN(Undefined());
}


/* Image Filters */


V8_METHOD(openvg::ColorMatrix) {
  HandleScope scope;

  CheckArgs3(colorMatrix,
             dstVGImage, Object, srcVGImage, Object, matrix, Object);

  TypedArrayWrapper<VGfloat> matrix(args[2]);

  vgColorMatrix(UNWRAPPED_HANDLE(VGImage, args[0]->ToObject()),
                UNWRAPPED_HANDLE(VGImage, args[1]->ToObject()),
                matrix.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::Convolve) {
  HandleScope scope;

  CheckArgs10(convolve, dstVGImage, Object, srcVGImage, Object,
              kernelWidth, Int32, kernelHeight, Int32,
              shiftX, Int32, shiftY, Int32,
              kernel, Object, scale, Number, bias, Number,
              tilingMode, Uint32);

  TypedArrayWrapper<VGshort> kernel(args[6]);

  vgConvolve(UNWRAPPED_HANDLE(VGImage, args[0]->ToObject()),
             UNWRAPPED_HANDLE(VGImage, args[1]->ToObject()),
             (VGint) args[2]->Int32Value(),
             (VGint) args[3]->Int32Value(),
             (VGint) args[4]->Int32Value(),
             (VGint) args[5]->Int32Value(),
             kernel.pointer(),
             (VGfloat) args[7]->NumberValue(),
             (VGfloat) args[8]->NumberValue(),
             static_cast<VGTilingMode>(args[9]->Uint32Value()));

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::SeparableConvolve) {
  HandleScope scope;

  CheckArgs11(separableConvolve, dstVGImage, Object, srcVGImage, Object,
              kernelWidth, Int32, kernelHeight, Int32,
              shiftX, Int32, shiftY, Int32,
              kernelX, Object, kernelY, Object,
              scale, Number, bias, Number,
              tilingMode, Uint32);

  TypedArrayWrapper<VGshort> kernelX(args[6]);
  TypedArrayWrapper<VGshort> kernelY(args[7]);

  vgSeparableConvolve(UNWRAPPED_HANDLE(VGImage, args[0]->ToObject()),
                      UNWRAPPED_HANDLE(VGImage, args[1]->ToObject()),
                      (VGint) args[2]->Int32Value(),
                      (VGint) args[3]->Int32Value(),
                      (VGint) args[4]->Int32Value(),
                      (VGint) args[5]->Int32Value(),
                      kernelX.pointer(),
                      kernelY.pointer(),
                      (VGfloat) args[8]->NumberValue(),
                      (VGfloat) args[9]->NumberValue(),
                      static_cast<VGTilingMode>(args[10]->Uint32Value()));

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::GaussianBlur) {
  HandleScope scope;

  CheckArgs5(gaussianBlur, dstVGImage, Object, srcVGImage, Object,
             stdDeviationX, Number, stdDeviationY, Number,
             tilingMode, Uint32);

  vgGaussianBlur(UNWRAPPED_HANDLE(VGImage, args[0]->ToObject()),
                 UNWRAPPED_HANDLE(VGImage, args[1]->ToObject()),
                 (VGfloat) args[2]->NumberValue(),
                 (VGfloat) args[3]->NumberValue(),
                 static_cast<VGTilingMode>(args[4]->Uint32Value()));

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::Lookup) {
  HandleScope scope;

  CheckArgs8(lookup, dstVGImage, Object, srcVGImage, Object,
             redLUT, Object, greenLUT, Object, blueLUT, Object,
             alphaLUT, Object,
             outputLinear, Boolean, outputPremultiplied, Boolean);

  TypedArrayWrapper<VGubyte> redLUT(args[2]);
  TypedArrayWrapper<VGubyte> greenLUT(args[3]);
  TypedArrayWrapper<VGubyte> blueLUT(args[4]);
  TypedArrayWrapper<VGubyte> alphaLUT(args[5]);

  vgLookup(UNWRAPPED_HANDLE(VGImage, args[0]->ToObject()),
           UNWRAPPED_HANDLE(VGImage, args[1]->ToObject()),
           redLUT.pointer(),
           greenLUT.pointer(),
           blueLUT.pointer(),
           alphaLUT.pointer(),
           (VGboolean) args[6]->BooleanValue(),
           (VGboolean) args[7]->BooleanValue());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::LookupSingle) {
  HandleScope scope;

  CheckArgs6(lookupSingle, dstVGImage, Object, srcVGImage, Object,
             lookupTable, Object, sourceChannel, Uint32,
             outputLinear, Boolean, outputPremultiplied, Boolean);

  TypedArrayWrapper<VGuint> lookupTable(args[2]);

  vgLookupSingle(UNWRAPPED_HANDLE(VGImage, args[0]->ToObject()),
                 UNWRAPPED_HANDLE(VGImage, args[1]->ToObject()),
                 lookupTable.pointer(),
                 static_cast<VGImageChannel>(args[3]->Uint32Value()),
                 (VGboolean) args[4]->BooleanValue(),
                 (VGboolean) args[5]->BooleanValue());

  V8_RETURN(Undefined());
}


/* Hardware Queries */
V8_METHOD(openvg::HardwareQuery) {
  HandleScope scope;

  CheckArgs2(hardwareQuery, key, Uint32, setting, Int32);

  V8_RETURN(Uint32::New(vgHardwareQuery(static_cast<VGHardwareQueryType>(args[0]->Uint32Value()),
                                        (VGint) args[1]->Int32Value())));
}


/* Renderer and Extension Information */
VG_API_CALL const VGubyte * VG_API_ENTRY vgGetString(VGStringID name) VG_API_EXIT;

V8_METHOD(openvg::GetString) {
  HandleScope scope;

  CheckArgs1(getString, key, Uint32);

  V8_RETURN(String::New((char*) vgGetString(static_cast<VGStringID>(args[0]->Uint32Value()))));
}


/* Utilities */


V8_METHOD(openvg::vgu::Line) {
  HandleScope scope;

  CheckArgs5(line, VGPath, Object,
             x0, Number, y0, Number, x1, Number, y1, Number);

  V8_RETURN(Uint32::New(vguLine(UNWRAPPED_HANDLE(VGPath, args[0]->ToObject()),
                                (VGfloat) args[1]->NumberValue(),
                                (VGfloat) args[2]->NumberValue(),
                                (VGfloat) args[3]->NumberValue(),
                                (VGfloat) args[4]->NumberValue())));
}

V8_METHOD(openvg::vgu::Polygon) {
  HandleScope scope;

  CheckArgs4(polygon, VGPath, Object,
             Float32Array, Object, count, Int32, closed, Boolean);

  TypedArrayWrapper<VGfloat> points(args[1]);

  V8_RETURN(Uint32::New(vguPolygon(UNWRAPPED_HANDLE(VGPath, args[0]->ToObject()),
                                   points.pointer(),
                                   (VGint) args[2]->Int32Value(),
                                   (VGboolean) args[3]->BooleanValue())));
}

V8_METHOD(openvg::vgu::Rect) {
  HandleScope scope;

  CheckArgs5(rect, VGPath, Object,
             x, Number, y, Number,
             width, Number, height, Number);

  V8_RETURN(Uint32::New(vguRect(UNWRAPPED_HANDLE(VGPath, args[0]->ToObject()),
                                (VGfloat) args[1]->NumberValue(),
                                (VGfloat) args[2]->NumberValue(),
                                (VGfloat) args[3]->NumberValue(),
                                (VGfloat) args[4]->NumberValue())));
}

V8_METHOD(openvg::vgu::RoundRect) {
  HandleScope scope;

  CheckArgs7(rect, VGPath, Object,
             x, Number, y, Number, width, Number, height, Number,
             arcWidth, Number, arcHeight, Number);

  V8_RETURN(Uint32::New(vguRoundRect(UNWRAPPED_HANDLE(VGPath, args[0]->ToObject()),
                                     (VGfloat) args[1]->NumberValue(),
                                     (VGfloat) args[2]->NumberValue(),
                                     (VGfloat) args[3]->NumberValue(),
                                     (VGfloat) args[4]->NumberValue(),
                                     (VGfloat) args[5]->NumberValue(),
                                     (VGfloat) args[6]->NumberValue())));
}

V8_METHOD(openvg::vgu::Ellipse) {
  HandleScope scope;

  CheckArgs5(ellipse, VGPath, Object,
             x, Number, y, Number, width, Number, height, Number);

  V8_RETURN(Uint32::New(vguEllipse(UNWRAPPED_HANDLE(VGPath, args[0]->ToObject()),
                                   (VGfloat) args[1]->NumberValue(),
                                   (VGfloat) args[2]->NumberValue(),
                                   (VGfloat) args[3]->NumberValue(),
                                   (VGfloat) args[4]->NumberValue())));
}

V8_METHOD(openvg::vgu::Arc) {
  HandleScope scope;

  CheckArgs8(arc, VGPath, Object,
             x, Number, y, Number, width, Number, height, Number,
             startAngle, Number, angleExtent, Number, VGUArcType, Uint32);

  V8_RETURN(Uint32::New(vguArc(UNWRAPPED_HANDLE(VGPath, args[0]->ToObject()),
                               (VGfloat) args[1]->NumberValue(),
                               (VGfloat) args[2]->NumberValue(),
                               (VGfloat) args[3]->NumberValue(),
                               (VGfloat) args[4]->NumberValue(),
                               (VGfloat) args[5]->NumberValue(),
                               (VGfloat) args[6]->NumberValue(),
                               static_cast<VGUArcType>(args[7]->Uint32Value()))));
}

V8_METHOD(openvg::vgu::ComputeWarpQuadToSquare) {
  HandleScope scope;

  CheckArgs9(computeWarpQuadToSquare,
             sx0, Number, sy0, Number, sx1, Number, sy1, Number,
             sx2, Number, sy2, Number, sx3, Number, sy3, Number,
             Float32Array, Object);

  TypedArrayWrapper<VGfloat> matrix(args[8]);

  V8_RETURN(Uint32::New(vguComputeWarpQuadToSquare((VGfloat) args[0]->NumberValue(),
                                                   (VGfloat) args[1]->NumberValue(),
                                                   (VGfloat) args[2]->NumberValue(),
                                                   (VGfloat) args[3]->NumberValue(),
                                                   (VGfloat) args[4]->NumberValue(),
                                                   (VGfloat) args[5]->NumberValue(),
                                                   (VGfloat) args[6]->NumberValue(),
                                                   (VGfloat) args[7]->NumberValue(),
                                                   matrix.pointer())));
}

V8_METHOD(openvg::vgu::ComputeWarpSquareToQuad) {
  HandleScope scope;

  CheckArgs9(computeWarpSquareToQuad,
             sx0, Number, sy0, Number, sx1, Number, sy1, Number,
             sx2, Number, sy2, Number, sx3, Number, sy3, Number,
             Float32Array, Object);

  TypedArrayWrapper<VGfloat> matrix(args[8]);

  V8_RETURN(Uint32::New(vguComputeWarpSquareToQuad((VGfloat) args[0]->NumberValue(),
                                                   (VGfloat) args[1]->NumberValue(),
                                                   (VGfloat) args[2]->NumberValue(),
                                                   (VGfloat) args[3]->NumberValue(),
                                                   (VGfloat) args[4]->NumberValue(),
                                                   (VGfloat) args[5]->NumberValue(),
                                                   (VGfloat) args[6]->NumberValue(),
                                                   (VGfloat) args[7]->NumberValue(),
                                                   matrix.pointer())));
}

V8_METHOD(openvg::vgu::ComputeWarpQuadToQuad) {
  HandleScope scope;

  // No arg check -> Would be a 17 arg macro

  TypedArrayWrapper<VGfloat> matrix(args[16]);

  V8_RETURN(Uint32::New(vguComputeWarpQuadToQuad((VGfloat) args[ 0]->NumberValue(),
                                                 (VGfloat) args[ 1]->NumberValue(),
                                                 (VGfloat) args[ 2]->NumberValue(),
                                                 (VGfloat) args[ 3]->NumberValue(),
                                                 (VGfloat) args[ 4]->NumberValue(),
                                                 (VGfloat) args[ 5]->NumberValue(),
                                                 (VGfloat) args[ 6]->NumberValue(),
                                                 (VGfloat) args[ 7]->NumberValue(),
                                                 (VGfloat) args[ 8]->NumberValue(),
                                                 (VGfloat) args[ 9]->NumberValue(),
                                                 (VGfloat) args[10]->NumberValue(),
                                                 (VGfloat) args[11]->NumberValue(),
                                                 (VGfloat) args[12]->NumberValue(),
                                                 (VGfloat) args[13]->NumberValue(),
                                                 (VGfloat) args[14]->NumberValue(),
                                                 (VGfloat) args[15]->NumberValue(),
                                                 matrix.pointer())));
}


/* KHR extensions */


V8_METHOD(openvg::ext::CreateEGLImageTargetKHR) {
  HandleScope scope;

  CheckArgs1(createEGLImageTargetKHR, VGeglImageKHR, Object);

#ifdef VG_VGEXT_PROTOTYPES
  VGeglImageKHR image = node::ObjectWrap::Unwrap<VGeglImageKHR>(args[0]->ToObject());
  V8_RETURN(scope.Close(External::New((void*) vgCreateEGLImageTargetKHR(image))));
#else
  V8_RETURN(Undefined());
#endif
}

V8_METHOD(openvg::ext::IterativeAverageBlurKHR) {
  HandleScope scope;

  CheckArgs6(iterativeAverageBlurKHR,
             dstVGImage, Object, srcVGImage, Object,
             dimX, Number, dimY, Number, iterative, Number,
             tilingMode, Object);

#ifdef VG_VGEXT_PROTOTYPES
  vgIterativeAverageBlurKHR(UNWRAPPED_HANDLE(VGImage, args[0]->ToObject()),
                            UNWRAPPED_HANDLE(VGImage, args[1]->ToObject()),
                            (VGfloat) args[2]->NumberValue(),
                            (VGfloat) args[3]->NumberValue(),
                            UNWRAPPED_HANDLE(VGImage, args[4]->ToObject()),
                            static_cast<VGTilingMode>(args[5]->Uint32Value()));
#endif
  V8_RETURN(Undefined());
}

V8_METHOD(openvg::ext::ParametricFilterKHR) {
  HandleScope scope;

  CheckArgs9(iterativeAverageBlurKHR,
             dstVGImage, Object, srcVGImage, Object, blurVGImage, Object,
             strength, Number, offsetX, Number, offsetY, Number,
             filterFlags, Number, highlightPaint, Number, shadowPaint, Number);

#ifdef VG_VGEXT_PROTOTYPES
  vgParametricFilterKHR(UNWRAPPED_HANDLE(VGImage, args[0]->ToObject()),
                        UNWRAPPED_HANDLE(VGImage, args[1]->ToObject()),
                        UNWRAPPED_HANDLE(VGImage, args[2]->ToObject()),
                        (VGfloat) args[3]->NumberValue(),
                        (VGfloat) args[4]->NumberValue(),
                        (VGfloat) args[5]->NumberValue(),
                        (VGbitfield) args[6]->Uint32Value(),
                        UNWRAPPED_HANDLE(VGPaint, args[7]->ToObject()),
                        UNWRAPPED_HANDLE(VGPaint, args[8]->ToObject()));
#endif

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::ext::DropShadowKHR) {
  HandleScope scope;

  CheckArgs11(dropShadowKHR,
              dstVGImage, Object, srcVGImage, Object,
              dimX, Number, dimY, Number, iterative, Number,
              strength, Number, distance, Number, angle, Number,
              filterFlags, Number, allowedQuality, Number,
              shadowColorRGBA, Number);

#ifdef VG_VGEXT_PROTOTYPES
  // Returns a VGUErrorCode
  V8_RETURN(Uint32::New(vguDropShadowKHR(UNWRAPPED_HANDLE(VGImage, args[0]->ToObject()),
                                         UNWRAPPED_HANDLE(VGImage, args[1]->ToObject()),
                                         (VGfloat) args[2]->NumberValue(),
                                         (VGfloat) args[3]->NumberValue(),
                                         (VGuint) args[4]->Uint32Value(),
                                         (VGfloat) args[5]->NumberValue(),
                                         (VGfloat) args[6]->NumberValue(),
                                         (VGfloat) args[7]->NumberValue(),
                                         (VGbitfield) args[8]->Uint32Value(),
                                         (VGbitfield) args[9]->Uint32Value(),
                                         (VGuint) args[10]->Uint32Value())));
#else
  V8_RETURN(Undefined());
#endif
}

V8_METHOD(openvg::ext::GlowKHR) {
  HandleScope scope;

  CheckArgs9(glowKHR,
             dstVGImage, Object, srcVGImage, Object,
             dimX, Number, dimY, Number, iterative, Number,
             strength, Number,
             filterFlags, Number, allowedQuality, Number,
             glowColorRGBA, Number);

#ifdef VG_VGEXT_PROTOTYPES
  // Returns a VGUErrorCode
  V8_RETURN(Uint32::New(vguGlowKHR(UNWRAPPED_HANDLE(VGImage, args[0]->ToObject()),
                                   UNWRAPPED_HANDLE(VGImage, args[1]->ToObject()),
                                   (VGfloat) args[2]->NumberValue(),
                                   (VGfloat) args[3]->NumberValue(),
                                   (VGuint) args[4]->Uint32Value(),
                                   (VGfloat) args[5]->NumberValue(),
                                   (VGbitfield) args[6]->Uint32Value(),
                                   (VGbitfield) args[7]->Uint32Value(),
                                   (VGuint) args[8]->Uint32Value())));
#else
  V8_RETURN(Undefined());
#endif
}

V8_METHOD(openvg::ext::BevelKHR) {
  HandleScope scope;

  CheckArgs12(bevelKHR,
              dstVGImage, Object, srcVGImage, Object,
              dimX, Number, dimY, Number, iterative, Number,
              strength, Number, distance, Number, angle, Number,
              filterFlags, Number, allowedQuality, Number,
              highlightColorRGBA, Number, shadowColorRGBA, Number);

#ifdef VG_VGEXT_PROTOTYPES
  // Returns a VGUErrorCode
  V8_RETURN(Uint32::New(vguBevelKHR(UNWRAPPED_HANDLE(VGImage, args[0]->ToObject()),
                                    UNWRAPPED_HANDLE(VGImage, args[1]->ToObject()),
                                    (VGfloat) args[2]->NumberValue(),
                                    (VGfloat) args[3]->NumberValue(),
                                    (VGuint) args[4]->Uint32Value(),
                                    (VGfloat) args[5]->NumberValue(),
                                    (VGfloat) args[6]->NumberValue(),
                                    (VGfloat) args[7]->NumberValue(),
                                    (VGbitfield) args[8]->Uint32Value(),
                                    (VGbitfield) args[9]->Uint32Value(),
                                    (VGuint) args[10]->Uint32Value(),
                                    (VGuint) args[11]->Uint32Value())));
#else
  V8_RETURN(Undefined());
#endif
}

V8_METHOD(openvg::ext::GradientGlowKHR) {
  HandleScope scope;

  CheckArgs12(gradientGlowKHR,
              dstVGImage, Object, srcVGImage, Object,
              dimX, Number, dimY, Number, iterative, Number,
              strength, Number, distance, Number, angle, Number,
              filterFlags, Number, allowedQuality, Number,
              stopsCount, Number, Float32Array, Object);

#ifdef VG_VGEXT_PROTOTYPES
  TypedArrayWrapper<VGfloat> glowColorRampStops(args[11]);

  // Returns a VGUErrorCode
  V8_RETURN(Uint32::New(vguGradientGlowKHR(UNWRAPPED_HANDLE(VGImage, args[0]->ToObject()),
                                           UNWRAPPED_HANDLE(VGImage, args[1]->ToObject()),
                                           (VGfloat) args[2]->NumberValue(),
                                           (VGfloat) args[3]->NumberValue(),
                                           (VGuint) args[4]->Uint32Value(),
                                           (VGfloat) args[5]->NumberValue(),
                                           (VGfloat) args[6]->NumberValue(),
                                           (VGfloat) args[7]->NumberValue(),
                                           (VGbitfield) args[8]->Uint32Value(),
                                           (VGbitfield) args[9]->Uint32Value(),
                                           (VGuint) args[10]->Uint32Value(),
                                           glowColorRampStops.pointer())));
#else
  V8_RETURN(Undefined());
#endif
}

V8_METHOD(openvg::ext::GradientBevelKHR) {
  HandleScope scope;

  CheckArgs12(gradientBevelKHR,
              dstVGImage, Object, srcVGImage, Object,
              dimX, Number, dimY, Number, iterative, Number,
              strength, Number, distance, Number, angle, Number,
              filterFlags, Number, allowedQuality, Number,
              stopsCount, Number, Float32Array, Object);

#ifdef VG_VGEXT_PROTOTYPES
  TypedArrayWrapper<VGfloat> bevelColorRampStops(args[11]);

  // Returns a VGUErrorCode
  V8_RETURN(Uint32::New(vguGradientBevelKHR(UNWRAPPED_HANDLE(VGImage, args[0]->ToObject()),
                                            UNWRAPPED_HANDLE(VGImage, args[1]->ToObject()),
                                            (VGfloat) args[2]->NumberValue(),
                                            (VGfloat) args[3]->NumberValue(),
                                            (VGuint) args[4]->Uint32Value(),
                                            (VGfloat) args[5]->NumberValue(),
                                            (VGfloat) args[6]->NumberValue(),
                                            (VGfloat) args[7]->NumberValue(),
                                            (VGbitfield) args[8]->Uint32Value(),
                                            (VGbitfield) args[9]->Uint32Value(),
                                            (VGuint) args[10]->Uint32Value(),
                                            bevelColorRampStops.pointer())));
#else
  V8_RETURN(Undefined());
#endif
}

V8_METHOD(openvg::ext::ProjectiveMatrixNDS) {
  HandleScope scope;

  CheckArgs1(projectiveMatrixNDS, enable, Boolean);

#ifdef VG_VGEXT_PROTOTYPES
  vgProjectiveMatrixNDS((VGboolean) args[0]->BooleanValue());
#endif

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::ext::TransformClipLineNDS) {
  HandleScope scope;

  CheckArgs8(gradientBevelKHR,
             Ain, Number, Bin, Number, Cin, Number,
             Float32Array, Object, inverse, Boolean,
             Float32Array, Object, Float32Array, Object, Float32Array, Object);

#ifdef VG_VGEXT_PROTOTYPES
  TypedArrayWrapper<VGfloat> matrix(args[3]);
  TypedArrayWrapper<VGfloat> Aout(args[5]);
  TypedArrayWrapper<VGfloat> Bout(args[6]);
  TypedArrayWrapper<VGfloat> Cout(args[7]);

  V8_RETURN(Uint32::New(vguTransformClipLineNDS((VGfloat) args[0]->NumberValue(),
                                                (VGfloat) args[1]->NumberValue(),
                                                (VGfloat) args[2]->NumberValue(),
                                                matrix.pointer(),
                                                (VGboolean) args[4]->BooleanValue(),
                                                Aout.pointer(),
                                                Bout.pointer(),
                                                Cout.pointer())));
#else
  V8_RETURN(Undefined());
#endif
}
