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

using namespace node;
using namespace v8;

void init(Handle<Object> target) {
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

#ifdef TYPED_ARRAY_TYPE_PRE_0_11
template<class C> class TypedArrayWrapper {
 private:
  Local<Object> array;
  Handle<Object> buffer;
  int byteOffset;
 public:
  inline __attribute__((always_inline)) TypedArrayWrapper(const Local<Value>& arg) :
    array(arg->ToObject()),
    buffer(array->Get(String::New("buffer"))->ToObject()),
    byteOffset(array->Get(String::New("byteOffset"))->Int32Value()) {
  }

  inline __attribute__((always_inline)) C* pointer(int offset = 0) {
    return (C*) &((char*) buffer->GetIndexedPropertiesExternalArrayData())[byteOffset + offset];
  }

  inline __attribute__((always_inline)) int length() {
    return array->Get(String::New("length"))->Uint32Value();
  }
};
#else
template<class C> class TypedArrayWrapper {
 private:
  Local<TypedArray> array;
 public:
  inline __attribute__((always_inline)) TypedArrayWrapper(const Local<Value>& arg) :
    array(Handle<TypedArray>::Cast(arg->ToObject())) {
  }

  inline __attribute__((always_inline)) C* pointer(int offset = 0) {
    return (C*) &((char*) array->BaseAddress())[offset];
  }

  inline __attribute__((always_inline)) int length() {
    return array->Length();
  }
};
#endif

V8_METHOD(openvg::StartUp) {
  HandleScope scope;

  CheckArgs1(startUp, screen, Object);

  egl::Init();

  if (kInitOpenGLES) {
    egl::InitOpenGLES();
  }

  CHECK_VG_ERROR;

  Local<Object> screen = args[0].As<Object>();
  screen->Set(String::NewSymbol("width" ),
              Integer::New(egl::State.screen_width));
  screen->Set(String::NewSymbol("height"),
              Integer::New(egl::State.screen_height));
  screen->Set(String::NewSymbol("display"),
              External::New(egl::State.display));
  screen->Set(String::NewSymbol("surface"),
              External::New(egl::State.surface));
  screen->Set(String::NewSymbol("context"),
              External::New(egl::State.context));

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

  CheckArgs3(setParameterF, VGHandle, Int32, VGParamType, Int32, value, Number);

  vgSetParameterf((VGHandle) args[0]->Int32Value(),
                  (VGParamType) args[1]->Int32Value(),
                  (VGfloat) args[2]->NumberValue());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::SetParameterI) {
  HandleScope scope;

  CheckArgs3(setParameterI, VGHandle, Int32, VGParamType, Int32, value, Int32);

  vgSetParameteri((VGHandle) args[0]->Int32Value(),
                  (VGParamType) args[1]->Int32Value(),
                  (VGint) args[2]->Int32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::SetParameterFV) {
  HandleScope scope;

  CheckArgs3(setParameterFV,
             VGHandle, Int32, VGParamType, Int32, Float32Array, Object);

  TypedArrayWrapper<VGfloat> values(args[2]);

  vgSetParameterfv((VGHandle) args[0]->Int32Value(),
                   (VGParamType) args[1]->Int32Value(),
                   values.length(),
                   values.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::SetParameterIV) {
  HandleScope scope;

  CheckArgs3(setParameterIV,
             VGHandle, Int32, VGParamType, Int32, Int32Array, Object);

  TypedArrayWrapper<VGint> values(args[2]);

  vgSetParameteriv((VGHandle) args[0]->Int32Value(),
                   (VGParamType) args[1]->Int32Value(),
                   values.length(),
                   values.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::SetParameterFVOL) {
  HandleScope scope;

  CheckArgs5(setParameterFV,
             VGHandle, Int32, VGParamType, Int32, Float32Array, Object,
             offset, Int32, length, Int32);

  TypedArrayWrapper<VGfloat> values(args[2]);

  vgSetParameterfv((VGHandle) args[0]->Int32Value(),
                   (VGParamType) args[1]->Int32Value(),
                   (VGint) args[4]->Int32Value(),
                   values.pointer(args[3]->Int32Value()));

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::SetParameterIVOL) {
  HandleScope scope;

  CheckArgs5(setParameterIV,
             VGHandle, Int32, VGParamType, Int32, Int32Array, Object,
             offset, Int32, length, Int32);

  TypedArrayWrapper<VGint> values(args[2]);

  vgSetParameteriv((VGHandle) args[0]->Int32Value(),
                   (VGParamType) args[1]->Int32Value(),
                   (VGint) args[4]->Int32Value(),
                   values.pointer(args[3]->Int32Value()));

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::GetParameterF) {
  HandleScope scope;

  CheckArgs2(getParameterF, VGHandle, Int32, VGParamType, Int32);

  V8_RETURN(Number::New(vgGetParameterf((VGHandle) args[0]->Int32Value(),
                                        (VGParamType) args[1]->Int32Value())));
}

V8_METHOD(openvg::GetParameterI) {
  HandleScope scope;

  CheckArgs2(getParameterI, VGHandle, Int32, VGParamType, Int32);

  V8_RETURN(Integer::New(vgGetParameteri((VGHandle) args[0]->Int32Value(),
                                         (VGParamType) args[1]->Int32Value())));
}

V8_METHOD(openvg::GetParameterVectorSize) {
  HandleScope scope;

  CheckArgs2(getParameterVectorSize, VGHandle, Int32, VGParamType, Int32);

  V8_RETURN(Integer::New(vgGetParameterVectorSize((VGHandle) args[0]->Int32Value(),
                                                  (VGParamType) args[1]->Int32Value())));
}

V8_METHOD(openvg::GetParameterFV) {
  HandleScope scope;

  CheckArgs3(getParameterFV,
             VGHandle, Int32, VGParamType, Int32, Float32Array, Object);

  TypedArrayWrapper<VGfloat> values(args[2]);

  vgGetParameterfv((VGHandle) args[0]->Int32Value(),
                   (VGParamType) args[1]->Int32Value(),
                   values.length(),
                   values.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::GetParameterIV) {
  HandleScope scope;

  CheckArgs3(getParameterIV,
             VGHandle, Int32, VGParamType, Int32, Int32Array, Object);

  TypedArrayWrapper<VGint> values(args[2]);

  vgGetParameteriv((VGHandle) args[0]->Int32Value(),
                   (VGParamType) args[1]->Int32Value(),
                   values.length(),
                   values.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::GetParameterFVOL) {
  HandleScope scope;

  CheckArgs5(getParameterFV,
             VGHandle, Int32, VGParamType, Int32, Float32Array, Object,
             offset, Int32, length, Int32);

  TypedArrayWrapper<VGfloat> values(args[2]);

  vgGetParameterfv((VGHandle) args[0]->Int32Value(),
                   (VGParamType) args[1]->Int32Value(),
                   (VGint) args[4]->Int32Value(),
                   values.pointer(args[3]->Int32Value()));

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::GetParameterIVOL) {
  HandleScope scope;

  CheckArgs5(getParameterIV,
             VGHandle, Int32, VGParamType, Int32, Int32Array, Object,
             offset, Int32, length, Int32);

  TypedArrayWrapper<VGint> values(args[2]);

  vgGetParameteriv((VGHandle) args[0]->Int32Value(),
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
             VGHandle, Uint32, VGMaskOperation, Uint32,
             x, Int32, y, Int32, width, Int32, height, Int32);

  vgMask((VGHandle) args[0]->Uint32Value(),
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
             VGPath, Uint32,
             VGbitfield, Uint32,
             VGMaskOperation, Uint32);

  vgRenderToMask((VGPath) args[0]->Uint32Value(),
                 (VGbitfield) args[1]->Uint32Value(),
                 (VGMaskOperation) args[2]->Uint32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::CreateMaskLayer) {
  HandleScope scope;

  CheckArgs2(createMaskLayer, width, Int32, height, Int32);

  V8_RETURN(Integer::New(vgCreateMaskLayer((VGint) args[0]->Int32Value(),
                                           (VGint) args[1]->Int32Value())));
}

V8_METHOD(openvg::DestroyMaskLayer) {
  HandleScope scope;

  CheckArgs1(destroyMaskLayer, VGMaskLayer, Uint32);

  vgDestroyMaskLayer((VGMaskLayer) args[0]->Uint32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::FillMaskLayer) {
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

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::CopyMask) {
  HandleScope scope;

  CheckArgs7(fillMaskLayer,
             VGMaskLayer, Uint32,
             dx, Int32, dy, Int32, sx, Int32, sy, Int32,
             width, Int32, height, Int32);

  vgCopyMask((VGMaskLayer) args[0]->Uint32Value(),
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

  V8_RETURN(Uint32::New(vgCreatePath((VGint) args[0]->Int32Value(),
                                     static_cast<VGPathDatatype>(args[1]->Uint32Value()),
                                     (VGfloat) args[2]->NumberValue(),
                                     (VGfloat) args[3]->NumberValue(),
                                     (VGint) args[4]->Int32Value(),
                                     (VGint) args[5]->Int32Value(),
                                     (VGbitfield) args[6]->Uint32Value())));
}

V8_METHOD(openvg::ClearPath) {
  HandleScope scope;

  CheckArgs2(clearPath, VGPath, Number, capabilities, Uint32);

  vgClearPath((VGPath) args[0]->Uint32Value(),
              (VGbitfield) args[1]->Uint32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::DestroyPath) {
  HandleScope scope;

  CheckArgs1(destroyPath, VGPath, Number);

  vgDestroyPath((VGPath) args[0]->Uint32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::RemovePathCapabilities) {
  HandleScope scope;

  CheckArgs2(removePathCapabilities, VGPath, Number, capabilities, Uint32);

  vgRemovePathCapabilities((VGPath) args[0]->Uint32Value(),
                           (VGbitfield) args[1]->Uint32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::GetPathCapabilities) {
  HandleScope scope;

  CheckArgs1(getPathCapabilities, VGPath, Number);

  V8_RETURN(Uint32::New(vgGetPathCapabilities((VGPath) args[0]->Int32Value())));
}

V8_METHOD(openvg::AppendPath) {
  HandleScope scope;

  CheckArgs2(appendPath, dstPath, Number, srcPath, Number);

  vgAppendPath((VGPath) args[0]->Uint32Value(),
               (VGPath) args[1]->Uint32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::AppendPathData) {
  HandleScope scope;

  CheckArgs4(appendPathData,
             dstPath, Number, numSegments, Int32, Uint8Array, Object,
             pathData, Object);

  TypedArrayWrapper<VGubyte> segments(args[2]);
  TypedArrayWrapper<void> data(args[3]);

  vgAppendPathData((VGPath) args[0]->Uint32Value(),
                   (VGint) args[1]->Int32Value(),
                   segments.pointer(),
                   data.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::AppendPathDataO) {
  HandleScope scope;

  CheckArgs4(appendPathData,
             dstPath, Number, numSegments, Int32, Uint8Array, Object,
             pathData, Object);

  TypedArrayWrapper<VGubyte> segments(args[2]);
  TypedArrayWrapper<void> data(args[4]);

  vgAppendPathData((VGPath) args[0]->Uint32Value(),
                   (VGint) args[1]->Int32Value(),
                   segments.pointer(args[3]->Uint32Value()),
                   data.pointer(args[5]->Int32Value()));

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::ModifyPathCoords) {
  HandleScope scope;

  CheckArgs4(modifyPathCoords,
             VGPath, Number, startIndex, Int32, numSegments, Int32,
             pathData, Object);

  TypedArrayWrapper<void> data(args[3]);

  vgModifyPathCoords((VGPath) args[0]->Uint32Value(),
                     (VGint) args[1]->Int32Value(),
                     (VGint) args[2]->Int32Value(),
                     data.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::TransformPath) {
  HandleScope scope;

  CheckArgs2(transformPath, dstPath, Number, srcPath, Number);

  vgTransformPath((VGPath) args[0]->Uint32Value(),
                  (VGPath) args[1]->Uint32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::InterpolatePath) {
  HandleScope scope;

  CheckArgs4(interpolatePath,
             dstPath, Number, startPath, Number, endPath, Number,
             amount, Number);

  V8_RETURN(Boolean::New(vgInterpolatePath((VGPath) args[0]->Uint32Value(),
                                           (VGPath) args[1]->Uint32Value(),
                                           (VGPath) args[2]->Uint32Value(),
                                           (VGfloat) args[3]->NumberValue())));
}

V8_METHOD(openvg::PathLength) {
  HandleScope scope;

  CheckArgs3(pathLength, path, Number,
             startSegment, Int32, numSegments, Int32);

  V8_RETURN(Number::New(vgPathLength((VGPath) args[0]->Uint32Value(),
                                     (VGint) args[1]->Int32Value(),
                                     (VGint) args[2]->Int32Value())));
}

V8_METHOD(openvg::PointAlongPath) {
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

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::PathBounds) {
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

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::PathTransformedBounds) {
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

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::DrawPath) {
  HandleScope scope;

  CheckArgs2(drawPath, VGPath, Number, paintModes, Number);

  vgDrawPath((VGPath) args[0]->Uint32Value(),
             (VGbitfield) args[1]->Uint32Value());

  V8_RETURN(Undefined());
}


/* Paint */


V8_METHOD(openvg::CreatePaint) {
  HandleScope scope;

  CheckArgs0(createPaint);

  V8_RETURN(Uint32::New(vgCreatePaint()));
}

V8_METHOD(openvg::DestroyPaint) {
  HandleScope scope;

  CheckArgs1(destroyPaint, VGPaint, Number);

  vgDestroyPaint((VGPaint) args[0]->Uint32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::SetPaint) {
  HandleScope scope;

  CheckArgs2(setPaint, VGPaint, Number, paintModes, Number);

  vgSetPaint((VGPaint) args[0]->Uint32Value(),
             (VGbitfield) args[1]->Uint32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::GetPaint) {
  HandleScope scope;

  CheckArgs1(getPaint, VGPaint, Uint32);

  V8_RETURN(Uint32::New(vgGetPaint(static_cast<VGPaintMode>(args[0]->Uint32Value()))));
}

V8_METHOD(openvg::SetColor) {
  HandleScope scope;

  CheckArgs2(setColor, VGPaint, Uint32, rgba, Uint32);

  vgSetColor((VGPaint) args[0]->Uint32Value(),
             (VGuint) args[1]->Uint32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::GetColor) {
  HandleScope scope;

  CheckArgs1(getColor, VGPaint, Uint32);

  V8_RETURN(Uint32::New(vgGetColor((VGPaint) args[0]->Uint32Value())));
}

V8_METHOD(openvg::PaintPattern) {
  HandleScope scope;

  CheckArgs2(paintPattern, VGPaint, Uint32, VGImage, Uint32);

  vgPaintPattern((VGPaint) args[0]->Uint32Value(),
                 (VGImage) args[1]->Uint32Value());

  V8_RETURN(Undefined());
}


/* Images */


V8_METHOD(openvg::CreateImage) {
  HandleScope scope;

  CheckArgs4(createImage,
             VGImageFormat, Uint32, width, Int32, height, Int32,
             allowedQuality, Uint32);

  V8_RETURN(Uint32::New(vgCreateImage(static_cast<VGImageFormat>(args[0]->Uint32Value()),
                                      (VGint) args[1]->Int32Value(),
                                      (VGint) args[2]->Int32Value(),
                                      (VGuint) args[3]->Uint32Value())));
}

V8_METHOD(openvg::DestroyImage) {
  HandleScope scope;

  CheckArgs1(destroyImage, VGImage, Number);

  vgDestroyImage((VGImage) (VGPaint) args[0]->Uint32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::ClearImage) {
  HandleScope scope;

  CheckArgs5(clearImage,
             VGImage, Number, x, Int32, y, Int32, width, Int32, height, Int32);

  vgClearImage((VGImage) args[0]->Uint32Value(),
               (VGint) args[1]->Int32Value(),
               (VGint) args[2]->Int32Value(),
               (VGint) args[3]->Int32Value(),
               (VGint) args[4]->Int32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::ImageSubData) {
  HandleScope scope;

  CheckArgs8(imageSubData,
             VGImage, Number, data, Object, dataStride, Int32,
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

  vgImageSubData((VGImage) args[0]->Uint32Value(),
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
             VGImage, Number, data, Object, dataStride, Int32,
             dataFormat, Uint32,
             x, Int32, y, Int32, width, Int32, height, Int32);

  TypedArrayWrapper<void> data(args[1]);

  vgGetImageSubData((VGImage) args[0]->Uint32Value(),
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
             VGImage, Number, x, Int32, y, Int32, width, Int32, height, Int32);

  V8_RETURN(Uint32::New(vgChildImage((VGImage) args[0]->Uint32Value(),
                                     (VGint) args[1]->Int32Value(),
                                     (VGint) args[2]->Int32Value(),
                                     (VGint) args[3]->Int32Value(),
                                     (VGint) args[4]->Int32Value())));
}

V8_METHOD(openvg::GetParent) {
  HandleScope scope;

  CheckArgs1(getParent, VGImage, Number);

  V8_RETURN(Uint32::New(vgGetParent((VGImage) args[0]->Uint32Value())));
}

V8_METHOD(openvg::CopyImage) {
  HandleScope scope;

  CheckArgs9(copyImage,
             dstImage, Number, dx, Int32, dy, Int32,
             srcImage, Number, sx, Int32, sy, Int32,
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

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::DrawImage) {
  HandleScope scope;

  CheckArgs1(drawImage, VGImage, Number);

  vgDrawImage((VGImage) args[0]->Uint32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::SetPixels) {
  HandleScope scope;

  CheckArgs7(setPixels,
             dx, Int32, dy, Int32,
             srcImage, Number, dx, Int32, dy, Int32,
             width, Int32, height, Int32);

  vgSetPixels((VGint) args[0]->Int32Value(),
              (VGint) args[1]->Int32Value(),
              (VGImage) args[2]->Uint32Value(),
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
             VGImage, Number,
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

  V8_RETURN(Uint32::New(vgCreateFont((VGint) args[0]->Int32Value())));
}

V8_METHOD(openvg::DestroyFont) {
  HandleScope scope;

  CheckArgs1(destroyFont, VGFont, Number);

  vgDestroyFont((VGFont) args[0]->Uint32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::SetGlyphToPath) {
  HandleScope scope;

  CheckArgs6(setGlyphToPath, VGFont, Number, glyphIndex, Number,
             VGPath, Number, isHinted, Boolean,
             glyphOrigin, Object, escapement, Object);

  TypedArrayWrapper<VGfloat> glyphOrigin(args[4]);
  TypedArrayWrapper<VGfloat> escapement(args[5]);

  vgSetGlyphToPath((VGFont) args[0]->Uint32Value(),
                   (VGuint) args[1]->Uint32Value(),
                   (VGPath) args[2]->Uint32Value(),
                   (VGboolean) args[3]->BooleanValue(),
                   glyphOrigin.pointer(),
                   escapement.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::SetGlyphToImage) {
  HandleScope scope;

  CheckArgs5(setGlyphToImage, VGFont, Number, glyphIndex, Number,
             VGImage, Number,
             glyphOrigin, Object, escapement, Object);

  TypedArrayWrapper<VGfloat> glyphOrigin(args[3]);
  TypedArrayWrapper<VGfloat> escapement(args[4]);

  vgSetGlyphToImage((VGFont) args[0]->Uint32Value(),
                    (VGuint) args[1]->Uint32Value(),
                    (VGImage) args[2]->Uint32Value(),
                    glyphOrigin.pointer(),
                    escapement.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::ClearGlyph) {
  HandleScope scope;

  CheckArgs2(clearGlyph, VGFont, Number, glyphIndex, Uint32);

  vgClearGlyph((VGFont) args[0]->Uint32Value(),
               (VGuint) args[1]->Uint32Value());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::DrawGlyph) {
  HandleScope scope;

  CheckArgs4(drawGlyph, VGFont, Number, glyphIndex, Uint32,
             paintModes, Uint32, allowAutoHinting, Boolean);

  vgDrawGlyph((VGFont) args[0]->Uint32Value(),
              (VGuint) args[1]->Uint32Value(),
              (VGbitfield) args[2]->Uint32Value(),
              (VGboolean) args[3]->BooleanValue());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::DrawGlyphs) {
  HandleScope scope;

  CheckArgs7(drawGlyphs, VGFont, Number, glyphCount, Int32,
             glyphIndices, Object, adjustments_x, Object, adjustments_y, Object,
             paintModes, Uint32, allowAutoHinting, Boolean);

  TypedArrayWrapper<VGuint> glyphIndices(args[2]);
  TypedArrayWrapper<VGfloat> adjustments_x(args[3]);
  TypedArrayWrapper<VGfloat> adjustments_y(args[4]);

  vgDrawGlyphs((VGFont) args[0]->Uint32Value(),
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
             dstVGImage, Number, srcVGImage, Number, matrix, Object);

  TypedArrayWrapper<VGfloat> matrix(args[2]);

  vgColorMatrix((VGImage) args[0]->Uint32Value(),
                (VGImage) args[1]->Uint32Value(),
                matrix.pointer());

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::Convolve) {
  HandleScope scope;

  CheckArgs10(convolve, dstVGImage, Number, srcVGImage, Number,
              kernelWidth, Int32, kernelHeight, Int32,
              shiftX, Int32, shiftY, Int32,
              kernel, Object, scale, Number, bias, Number,
              tilingMode, Uint32);

  TypedArrayWrapper<VGshort> kernel(args[6]);

  vgConvolve((VGImage) args[0]->Uint32Value(),
             (VGImage) args[1]->Uint32Value(),
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

  CheckArgs11(separableConvolve, dstVGImage, Number, srcVGImage, Number,
              kernelWidth, Int32, kernelHeight, Int32,
              shiftX, Int32, shiftY, Int32,
              kernelX, Object, kernelY, Object,
              scale, Number, bias, Number,
              tilingMode, Uint32);

  TypedArrayWrapper<VGshort> kernelX(args[6]);
  TypedArrayWrapper<VGshort> kernelY(args[7]);

  vgSeparableConvolve((VGImage) args[0]->Uint32Value(),
                      (VGImage) args[1]->Uint32Value(),
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

  CheckArgs5(gaussianBlur, dstVGImage, Number, srcVGImage, Number,
             stdDeviationX, Number, stdDeviationY, Number,
             tilingMode, Uint32);

  vgGaussianBlur((VGImage) args[0]->Uint32Value(),
                 (VGImage) args[1]->Uint32Value(),
                 (VGfloat) args[2]->NumberValue(),
                 (VGfloat) args[3]->NumberValue(),
                 static_cast<VGTilingMode>(args[4]->Uint32Value()));

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::Lookup) {
  HandleScope scope;

  CheckArgs9(lookup, VGImage, Number, dstVGImage, Number, srcVGImage, Number,
             redLUT, Object, greenLUT, Object, blueLUT, Object,
             alphaLUT, Object,
             outputLinear, Boolean, outputPremultiplied, Boolean);

  TypedArrayWrapper<VGubyte> redLUT(args[2]);
  TypedArrayWrapper<VGubyte> greenLUT(args[3]);
  TypedArrayWrapper<VGubyte> blueLUT(args[4]);
  TypedArrayWrapper<VGubyte> alphaLUT(args[5]);

  vgLookup((VGImage) args[0]->Uint32Value(),
           (VGImage) args[1]->Uint32Value(),
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

  CheckArgs6(lookupSingle, dstVGImage, Number, srcVGImage, Number,
             lookupTable, Object, sourceChannel, Uint32,
             outputLinear, Boolean, outputPremultiplied, Boolean);

  TypedArrayWrapper<VGuint> lookupTable(args[2]);

  vgLookupSingle((VGImage) args[0]->Uint32Value(),
                 (VGImage) args[1]->Uint32Value(),
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

  CheckArgs5(line,
             VGPath, Number, x0, Number, y0, Number, x1, Number, y1, Number);

  V8_RETURN(Uint32::New(vguLine((VGPath) args[0]->Uint32Value(),
                                (VGfloat) args[1]->NumberValue(),
                                (VGfloat) args[2]->NumberValue(),
                                (VGfloat) args[3]->NumberValue(),
                                (VGfloat) args[4]->NumberValue())));
}

V8_METHOD(openvg::vgu::Polygon) {
  HandleScope scope;

  CheckArgs4(polygon,
             VGPath, Number, Float32Array, Object, count, Int32,
             closed, Boolean);

  TypedArrayWrapper<VGfloat> points(args[1]);

  V8_RETURN(Uint32::New(vguPolygon((VGPath) args[0]->Uint32Value(),
                                   points.pointer(),
                                   (VGint) args[2]->Int32Value(),
                                   (VGboolean) args[3]->BooleanValue())));
}

V8_METHOD(openvg::vgu::Rect) {
  HandleScope scope;

  CheckArgs5(rect, VGPath, Number, x, Number, y, Number,
             width, Number, height, Number);

  V8_RETURN(Uint32::New(vguRect((VGPath) args[0]->Uint32Value(),
                                (VGfloat) args[1]->NumberValue(),
                                (VGfloat) args[2]->NumberValue(),
                                (VGfloat) args[3]->NumberValue(),
                                (VGfloat) args[4]->NumberValue())));
}

V8_METHOD(openvg::vgu::RoundRect) {
  HandleScope scope;

  CheckArgs7(rect, VGPath,
             Number, x, Number, y, Number, width, Number, height,
             Number, arcWidth, Number, arcHeight, Number);

  V8_RETURN(Uint32::New(vguRoundRect((VGPath) args[0]->Uint32Value(),
                                     (VGfloat) args[1]->NumberValue(),
                                     (VGfloat) args[2]->NumberValue(),
                                     (VGfloat) args[3]->NumberValue(),
                                     (VGfloat) args[4]->NumberValue(),
                                     (VGfloat) args[5]->NumberValue(),
                                     (VGfloat) args[6]->NumberValue())));
}

V8_METHOD(openvg::vgu::Ellipse) {
  HandleScope scope;

  CheckArgs5(ellipse, VGPath, Number, x, Number, y, Number,
             width, Number, height, Number);

  V8_RETURN(Uint32::New(vguEllipse((VGPath) args[0]->Uint32Value(),
                                   (VGfloat) args[1]->NumberValue(),
                                   (VGfloat) args[2]->NumberValue(),
                                   (VGfloat) args[3]->NumberValue(),
                                   (VGfloat) args[4]->NumberValue())));
}

V8_METHOD(openvg::vgu::Arc) {
  HandleScope scope;

  CheckArgs8(arc,
             VGPath, Number, x, Number, y, Number,
             width, Number, height, Number,
             startAngle, Number, angleExtent, Number, VGUArcType, Uint32);

  V8_RETURN(Uint32::New(vguArc((VGPath) args[0]->Uint32Value(),
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
  V8_RETURN(Uint32::New(vgCreateEGLImageTargetKHR(image)));
#else
  V8_RETURN(Undefined());
#endif
}

V8_METHOD(openvg::ext::IterativeAverageBlurKHR) {
  HandleScope scope;

  CheckArgs6(iterativeAverageBlurKHR,
             dstVGImage, Number, srcVGImage, Number,
             dimX, Number, dimY, Number, iterative, Number,
             tilingMode, Object);

#ifdef VG_VGEXT_PROTOTYPES
  vgIterativeAverageBlurKHR((VGImage) args[0]->Uint32Value(),
                            (VGImage) args[1]->Uint32Value(),
                            (VGfloat) args[2]->NumberValue(),
                            (VGfloat) args[3]->NumberValue(),
                            (VGImage) args[4]->Uint32Value(),
                            static_cast<VGTilingMode>((VGImage) args[5]->Uint32Value()));
#endif
  V8_RETURN(Undefined());
}

V8_METHOD(openvg::ext::ParametricFilterKHR) {
  HandleScope scope;

  CheckArgs9(iterativeAverageBlurKHR,
             dstVGImage, Number, srcVGImage, Number, blurVGImage, Number,
             strength, Number, offsetX, Number, offsetY, Number,
             filterFlags, Number, highlightPaint, Number, shadowPaint, Number);

#ifdef VG_VGEXT_PROTOTYPES
  vgParametricFilterKHR((VGImage) args[0]->Uint32Value(),
                        (VGImage) args[1]->Uint32Value(),
                        (VGImage) args[2]->Uint32Value(),
                        (VGfloat) args[3]->NumberValue(),
                        (VGfloat) args[4]->NumberValue(),
                        (VGfloat) args[5]->NumberValue(),
                        (VGbitfield) args[6]->Uint32Value(),
                        (VGPaint) args[7]->Uint32Value(),
                        (VGPaint) args[8]->Uint32Value());
#endif

  V8_RETURN(Undefined());
}

V8_METHOD(openvg::ext::DropShadowKHR) {
  HandleScope scope;

  CheckArgs11(dropShadowKHR,
              dstVGImage, Number, srcVGImage, Number,
              dimX, Number, dimY, Number, iterative, Number,
              strength, Number, distance, Number, angle, Number,
              filterFlags, Number, allowedQuality, Number,
              shadowColorRGBA, Number);

#ifdef VG_VGEXT_PROTOTYPES
  V8_RETURN(Uint32::New(vguDropShadowKHR((VGImage) args[0]->Uint32Value(),
                                         (VGImage) args[1]->Uint32Value(),
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
             dstVGImage, Number, srcVGImage, Number,
             dimX, Number, dimY, Number, iterative, Number,
             strength, Number,
             filterFlags, Number, allowedQuality, Number,
             glowColorRGBA, Number);

#ifdef VG_VGEXT_PROTOTYPES
  V8_RETURN(Uint32::New(vguGlowKHR((VGImage) args[0]->Uint32Value(),
                                   (VGImage) args[1]->Uint32Value(),
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
              dstVGImage, Number, srcVGImage, Number,
              dimX, Number, dimY, Number, iterative, Number,
              strength, Number, distance, Number, angle, Number,
              filterFlags, Number, allowedQuality, Number,
              highlightColorRGBA, Number, shadowColorRGBA, Number);

#ifdef VG_VGEXT_PROTOTYPES
  V8_RETURN(Uint32::New(vguBevelKHR((VGImage) args[0]->Uint32Value(),
                                    (VGImage) args[1]->Uint32Value(),
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
              dstVGImage, Number, srcVGImage, Number,
              dimX, Number, dimY, Number, iterative, Number,
              strength, Number, distance, Number, angle, Number,
              filterFlags, Number, allowedQuality, Number,
              stopsCount, Number, Float32Array, Object);

#ifdef VG_VGEXT_PROTOTYPES
  TypedArrayWrapper<VGfloat> glowColorRampStops(args[11]);

  V8_RETURN(Uint32::New(vguGradientGlowKHR((VGImage) args[0]->Uint32Value(),
                                           (VGImage) args[1]->Uint32Value(),
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
              dstVGImage, Number, srcVGImage, Number,
              dimX, Number, dimY, Number, iterative, Number,
              strength, Number, distance, Number, angle, Number,
              filterFlags, Number, allowedQuality, Number,
              stopsCount, Number, Float32Array, Object);

#ifdef VG_VGEXT_PROTOTYPES
  TypedArrayWrapper<VGfloat> bevelColorRampStops(args[11]);

  V8_RETURN(Uint32::New(vguGradientBevelKHR((VGImage) args[0]->Uint32Value(),
                                            (VGImage) args[1]->Uint32Value(),
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
