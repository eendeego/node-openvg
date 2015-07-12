#include <stdio.h>

#include "VG/openvg.h"
#include "VG/vgu.h"
#include "VG/vgext.h"

#include <v8.h>
#include <node.h>
#include <node_buffer.h>
#include "nan.h"

#include "openvg.h"
#include "egl.h"
#include "argchecks.h"

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
  Local<Object> VGU = NanNew<Object>();
  target->Set(NanNew<String>("vgu"), VGU);

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
  Local<Object> ext = NanNew<Object>();
  target->Set(NanNew<String>("ext"), ext);

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
  Local<Object> egl = NanNew<Object>();
  target->Set(NanNew<String>("egl"), egl);
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

#if NODE_MODULE_VERSION <= NODE_0_10_MODULE_VERSION
template<class C> class TypedArrayWrapper {
 private:
  Local<Object> array;
  Handle<Object> buffer;
  int byteOffset;
 public:
  NAN_INLINE TypedArrayWrapper(const Local<Value>& arg) :
    array(arg->ToObject()),
    buffer(array->Get(NanNew<String>("buffer"))->ToObject()),
    byteOffset(array->Get(NanNew<String>("byteOffset"))->Int32Value()) {
  }

  NAN_INLINE C* pointer(int offset = 0) {
    return (C*) &((char*) buffer->GetIndexedPropertiesExternalArrayData())[byteOffset + offset];
  }

  NAN_INLINE int length() {
    return array->Get(NanNew<String>("length"))->Uint32Value();
  }
};
#else
template<class C> class TypedArrayWrapper {
 private:
  Local<TypedArray> array;
 public:
  NAN_INLINE TypedArrayWrapper(const Local<Value>& arg) :
    array(Handle<TypedArray>::Cast(arg->ToObject())) {
  }

  NAN_INLINE C* pointer(int offset = 0) {
    return (C*) &((char*) array->BaseAddress())[offset];
  }

  NAN_INLINE int length() {
    return array->Length();
  }
};
#endif

NAN_METHOD(openvg::StartUp) {
  NanScope();

  CheckArgs1(startUp, screen, Object);

  egl::Init();

  if (kInitOpenGLES) {
    egl::InitOpenGLES();
  }

  CHECK_VG_ERROR;

  Local<Object> screen = args[0].As<Object>();
  screen->Set(NanNew<String>("width" ), NanNew<Uint32>(egl::State.screen_width));
  screen->Set(NanNew<String>("height"), NanNew<Uint32>(egl::State.screen_height));
  screen->Set(NanNew<String>("display"), NanNew<External>(egl::State.display));
  screen->Set(NanNew<String>("surface"), NanNew<External>(egl::State.surface));
  screen->Set(NanNew<String>("context"), NanNew<External>(egl::State.context));

  NanReturnUndefined();
}

NAN_METHOD(openvg::Shutdown) {
  NanScope();

  CheckArgs0(shutdown);

  egl::Finish();

  NanReturnUndefined();
}


NAN_METHOD(openvg::GetError) {
  NanScope();

  CheckArgs0(getError);

  NanReturnValue(NanNew<Integer>(vgGetError()));
}


NAN_METHOD(openvg::Flush) {
  NanScope();

  CheckArgs0(flush);

  vgFlush();

  NanReturnUndefined();
}

NAN_METHOD(openvg::Finish) {
  NanScope();

  CheckArgs0(finish);

  vgFinish();

  NanReturnUndefined();
}


/* Getters and Setters */


NAN_METHOD(openvg::SetF) {
  NanScope();

  CheckArgs2(setF, type, Int32, value, Number);

  vgSetf((VGParamType) args[0]->Int32Value(),
         (VGfloat) args[1]->NumberValue());

  NanReturnUndefined();
}

NAN_METHOD(openvg::SetI) {
  NanScope();

  CheckArgs2(setI, type, Int32, value, Int32);

  vgSeti((VGParamType) args[0]->Int32Value(),
         (VGint) args[1]->Int32Value());

  NanReturnUndefined();
}

NAN_METHOD(openvg::SetFV) {
  NanScope();

  CheckArgs2(setFV, type, Int32, Float32Array, Object);

  TypedArrayWrapper<VGfloat> values(args[1]);

  vgSetfv((VGParamType) args[0]->Int32Value(),
          values.length(),
          values.pointer());

  NanReturnUndefined();
}

NAN_METHOD(openvg::SetIV) {
  NanScope();

  CheckArgs2(setIV, type, Int32, Int32Array, Object);

  TypedArrayWrapper<VGint> values(args[1]);

  vgSetiv((VGParamType) args[0]->Int32Value(),
          values.length(),
          values.pointer());

  NanReturnUndefined();
}

NAN_METHOD(openvg::SetFVOL) {
  NanScope();

  CheckArgs4(setFVOL, type, Int32, Float32Array, Object, offset, Int32, length, Int32);

  TypedArrayWrapper<VGfloat> values(args[1]);

  vgSetfv((VGParamType) args[0]->Int32Value(),
          (VGint) args[3]->Int32Value(),
          values.pointer(args[2]->Int32Value()));

  NanReturnUndefined();
}

NAN_METHOD(openvg::SetIVOL) {
  NanScope();

  CheckArgs4(setIV, type, Int32, Int32Array, Object, offset, Int32, length, Int32);

  TypedArrayWrapper<VGint> values(args[1]);

  vgSetiv((VGParamType) args[0]->Int32Value(),
          (VGint) args[3]->Int32Value(),
          values.pointer(args[2]->Int32Value()));

  NanReturnUndefined();
}

NAN_METHOD(openvg::GetF) {
  NanScope();

  CheckArgs1(getF, type, Int32);

  NanReturnValue(NanNew<Number>(vgGetf((VGParamType) args[0]->Int32Value())));
}

NAN_METHOD(openvg::GetI) {
  NanScope();

  CheckArgs1(getI, type, Int32);

  NanReturnValue(NanNew<Integer>(vgGeti((VGParamType) args[0]->Int32Value())));
}

NAN_METHOD(openvg::GetVectorSize) {
  NanScope();

  CheckArgs1(getVectorSize, type, Int32);

  NanReturnValue(NanNew<Integer>(vgGetVectorSize((VGParamType) args[0]->Int32Value())));
}

NAN_METHOD(openvg::GetFV) {
  NanScope();

  CheckArgs2(getFV, type, Int32, Float32Array, Object);

  TypedArrayWrapper<VGfloat> values(args[1]);

  vgGetfv((VGParamType) args[0]->Int32Value(),
          values.length(),
          values.pointer());

  NanReturnUndefined();
}

NAN_METHOD(openvg::GetIV) {
  NanScope();

  CheckArgs2(getIV, type, Int32, Float32Array, Object);

  TypedArrayWrapper<VGint> values(args[1]);

  vgGetiv((VGParamType) args[0]->Int32Value(),
          values.length(),
          values.pointer());

  NanReturnUndefined();
}

NAN_METHOD(openvg::GetFVOL) {
  NanScope();

  CheckArgs4(getFV, type, Int32, Float32Array, Object, offset, Int32, length, Int32);

  TypedArrayWrapper<VGfloat> values(args[1]);

  vgGetfv((VGParamType) args[0]->Int32Value(),
          (VGint) args[3]->Int32Value(),
          values.pointer(args[2]->Int32Value()));

  NanReturnUndefined();
}

NAN_METHOD(openvg::GetIVOL) {
  NanScope();

  CheckArgs4(getIV, type, Int32, Float32Array, Object, offset, Int32, length, Int32);

  TypedArrayWrapper<VGint> values(args[1]);

  vgGetiv((VGParamType) args[0]->Int32Value(),
          (VGint) args[3]->Int32Value(),
          values.pointer(args[2]->Int32Value()));

  NanReturnUndefined();
}


NAN_METHOD(openvg::SetParameterF) {
  NanScope();

  CheckArgs3(setParameterF, VGHandle, Int32, VGParamType, Int32, value, Number);

  vgSetParameterf((VGHandle) args[0]->Int32Value(),
                  (VGParamType) args[1]->Int32Value(),
                  (VGfloat) args[2]->NumberValue());

  NanReturnUndefined();
}

NAN_METHOD(openvg::SetParameterI) {
  NanScope();

  CheckArgs3(setParameterI, VGHandle, Int32, VGParamType, Int32, value, Int32);

  vgSetParameteri((VGHandle) args[0]->Int32Value(),
                  (VGParamType) args[1]->Int32Value(),
                  (VGint) args[2]->Int32Value());

  NanReturnUndefined();
}

NAN_METHOD(openvg::SetParameterFV) {
  NanScope();

  CheckArgs3(setParameterFV,
             VGHandle, Int32, VGParamType, Int32, Float32Array, Object);

  TypedArrayWrapper<VGfloat> values(args[2]);

  vgSetParameterfv((VGHandle) args[0]->Int32Value(),
                   (VGParamType) args[1]->Int32Value(),
                   values.length(),
                   values.pointer());

  NanReturnUndefined();
}

NAN_METHOD(openvg::SetParameterIV) {
  NanScope();

  CheckArgs3(setParameterIV,
             VGHandle, Int32, VGParamType, Int32, Int32Array, Object);

  TypedArrayWrapper<VGint> values(args[2]);

  vgSetParameteriv((VGHandle) args[0]->Int32Value(),
                   (VGParamType) args[1]->Int32Value(),
                   values.length(),
                   values.pointer());

  NanReturnUndefined();
}

NAN_METHOD(openvg::SetParameterFVOL) {
  NanScope();

  CheckArgs5(setParameterFV,
             VGHandle, Int32, VGParamType, Int32, Float32Array, Object,
             offset, Int32, length, Int32);

  TypedArrayWrapper<VGfloat> values(args[2]);

  vgSetParameterfv((VGHandle) args[0]->Int32Value(),
                   (VGParamType) args[1]->Int32Value(),
                   (VGint) args[4]->Int32Value(),
                   values.pointer(args[3]->Int32Value()));

  NanReturnUndefined();
}

NAN_METHOD(openvg::SetParameterIVOL) {
  NanScope();

  CheckArgs5(setParameterIV,
             VGHandle, Int32, VGParamType, Int32, Int32Array, Object,
             offset, Int32, length, Int32);

  TypedArrayWrapper<VGint> values(args[2]);

  vgSetParameteriv((VGHandle) args[0]->Int32Value(),
                   (VGParamType) args[1]->Int32Value(),
                   (VGint) args[4]->Int32Value(),
                   values.pointer(args[3]->Int32Value()));

  NanReturnUndefined();
}

NAN_METHOD(openvg::GetParameterF) {
  NanScope();

  CheckArgs2(getParameterF, VGHandle, Int32, VGParamType, Int32);

  NanReturnValue(NanNew<Number>(vgGetParameterf((VGHandle) args[0]->Int32Value(),
                                                (VGParamType) args[1]->Int32Value())));
}

NAN_METHOD(openvg::GetParameterI) {
  NanScope();

  CheckArgs2(getParameterI, VGHandle, Int32, VGParamType, Int32);

  NanReturnValue(NanNew<Integer>(vgGetParameteri((VGHandle) args[0]->Int32Value(),
                                                 (VGParamType) args[1]->Int32Value())));
}

NAN_METHOD(openvg::GetParameterVectorSize) {
  NanScope();

  CheckArgs2(getParameterVectorSize, VGHandle, Int32, VGParamType, Int32);

  NanReturnValue(NanNew<Integer>(vgGetParameterVectorSize((VGHandle) args[0]->Int32Value(),
                                                          (VGParamType) args[1]->Int32Value())));
}

NAN_METHOD(openvg::GetParameterFV) {
  NanScope();

  CheckArgs3(getParameterFV,
             VGHandle, Int32, VGParamType, Int32, Float32Array, Object);

  TypedArrayWrapper<VGfloat> values(args[2]);

  vgGetParameterfv((VGHandle) args[0]->Int32Value(),
                   (VGParamType) args[1]->Int32Value(),
                   values.length(),
                   values.pointer());

  NanReturnUndefined();
}

NAN_METHOD(openvg::GetParameterIV) {
  NanScope();

  CheckArgs3(getParameterIV,
             VGHandle, Int32, VGParamType, Int32, Int32Array, Object);

  TypedArrayWrapper<VGint> values(args[2]);

  vgGetParameteriv((VGHandle) args[0]->Int32Value(),
                   (VGParamType) args[1]->Int32Value(),
                   values.length(),
                   values.pointer());

  NanReturnUndefined();
}

NAN_METHOD(openvg::GetParameterFVOL) {
  NanScope();

  CheckArgs5(getParameterFV,
             VGHandle, Int32, VGParamType, Int32, Float32Array, Object,
             offset, Int32, length, Int32);

  TypedArrayWrapper<VGfloat> values(args[2]);

  vgGetParameterfv((VGHandle) args[0]->Int32Value(),
                   (VGParamType) args[1]->Int32Value(),
                   (VGint) args[4]->Int32Value(),
                   values.pointer(args[3]->Int32Value()));

  NanReturnUndefined();
}

NAN_METHOD(openvg::GetParameterIVOL) {
  NanScope();

  CheckArgs5(getParameterIV,
             VGHandle, Int32, VGParamType, Int32, Int32Array, Object,
             offset, Int32, length, Int32);

  TypedArrayWrapper<VGint> values(args[2]);

  vgGetParameteriv((VGHandle) args[0]->Int32Value(),
                   (VGParamType) args[1]->Int32Value(),
                   (VGint) args[4]->Int32Value(),
                   values.pointer(args[3]->Int32Value()));

  NanReturnUndefined();
}


/* Matrix Manipulation */


NAN_METHOD(openvg::LoadIdentity) {
  NanScope();

  CheckArgs0(loadIdentity);

  vgLoadIdentity();

  NanReturnUndefined();
}

NAN_METHOD(openvg::LoadMatrix) {
  NanScope();

  CheckArgs1(loadIdentity, Float32Array, Object);

  TypedArrayWrapper<VGfloat> matrix(args[0]);

  vgLoadMatrix(matrix.pointer());

  NanReturnUndefined();
}

NAN_METHOD(openvg::GetMatrix) {
  NanScope();

  CheckArgs1(getMatrix, Float32Array, Object);

  TypedArrayWrapper<VGfloat> matrix(args[0]);

  vgGetMatrix(matrix.pointer());

  NanReturnUndefined();
}

NAN_METHOD(openvg::MultMatrix) {
  NanScope();

  CheckArgs1(multMatrix, Float32Array, Object);

  TypedArrayWrapper<VGfloat> matrix(args[0]);

  vgMultMatrix(matrix.pointer());

  NanReturnUndefined();
}

NAN_METHOD(openvg::Translate) {
  NanScope();

  CheckArgs2(translate, x, Number, y, Number);

  vgTranslate((VGfloat) args[0]->NumberValue(),
              (VGfloat) args[1]->NumberValue());

  NanReturnUndefined();
}

NAN_METHOD(openvg::Scale) {
  NanScope();

  CheckArgs2(scale, x, Number, y, Number);

  vgScale((VGfloat) args[0]->NumberValue(),
          (VGfloat) args[1]->NumberValue());

  NanReturnUndefined();
}

NAN_METHOD(openvg::Shear) {
  NanScope();

  CheckArgs2(shear, x, Number, y, Number);

  vgShear((VGfloat) args[0]->NumberValue(),
          (VGfloat) args[1]->NumberValue());

  NanReturnUndefined();
}

NAN_METHOD(openvg::Rotate) {
  NanScope();

  CheckArgs1(shear, angle, Number);

  vgRotate((VGfloat) args[0]->NumberValue());

  NanReturnUndefined();
}


/* Masking and Clearing */


NAN_METHOD(openvg::Mask) {
  NanScope();

  CheckArgs6(mask,
             VGHandle, Uint32, VGMaskOperation, Uint32,
             x, Int32, y, Int32, width, Int32, height, Int32);

  vgMask((VGHandle) args[0]->Uint32Value(),
         static_cast<VGMaskOperation>(args[1]->Uint32Value()),
         (VGint) args[2]->Int32Value(),
         (VGint) args[3]->Int32Value(),
         (VGint) args[4]->Int32Value(),
         (VGint) args[5]->Int32Value());

  NanReturnUndefined();
}

NAN_METHOD(openvg::RenderToMask) {
  NanScope();

  CheckArgs3(renderToMask,
             VGPath, Uint32,
             VGbitfield, Uint32,
             VGMaskOperation, Uint32);

  vgRenderToMask((VGPath) args[0]->Uint32Value(),
                 (VGbitfield) args[1]->Uint32Value(),
                 (VGMaskOperation) args[2]->Uint32Value());

  NanReturnUndefined();
}

NAN_METHOD(openvg::CreateMaskLayer) {
  NanScope();

  CheckArgs2(createMaskLayer, width, Int32, height, Int32);

  NanReturnValue(NanNew<Integer>(vgCreateMaskLayer((VGint) args[0]->Int32Value(),
                                                   (VGint) args[1]->Int32Value())));
}

NAN_METHOD(openvg::DestroyMaskLayer) {
  NanScope();

  CheckArgs1(destroyMaskLayer, VGMaskLayer, Uint32);

  vgDestroyMaskLayer((VGMaskLayer) args[0]->Uint32Value());

  NanReturnUndefined();
}

NAN_METHOD(openvg::FillMaskLayer) {
  NanScope();

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

  NanReturnUndefined();
}

NAN_METHOD(openvg::CopyMask) {
  NanScope();

  CheckArgs7(fillMaskLayer,
             VGMaskLayer, Uint32,
             dx, Int32, dy, Int32, sx, Int32, sy, Int32,
             width, Int32, height, Int32);

  vgCopyMask((VGMaskLayer) args[0]->Uint32Value(),
             (VGint) args[1]->Int32Value(), (VGint) args[2]->Int32Value(),
             (VGint) args[3]->Int32Value(), (VGint) args[4]->Int32Value(),
             (VGint) args[5]->Int32Value(), (VGint) args[6]->Int32Value());

  NanReturnUndefined();
}

NAN_METHOD(openvg::Clear) {
  NanScope();

  CheckArgs4(clear, x, Int32, y, Int32, width, Int32, height, Int32);

  vgClear((VGint) args[0]->Int32Value(), (VGint) args[1]->Int32Value(),
          (VGint) args[2]->Int32Value(), (VGint) args[3]->Int32Value());

  NanReturnUndefined();
}


/* Paths */


NAN_METHOD(openvg::CreatePath) {
  NanScope();

  CheckArgs7(createPath,
             pathFormat, Int32, VGPathDatatype, Uint32,
             scale, Number, bias, Number, segmentCapacityHint, Int32,
             coordCapacityHint, Int32, capabilities, Uint32);

  NanReturnValue(NanNew<Uint32>(vgCreatePath((VGint) args[0]->Int32Value(),
                                             static_cast<VGPathDatatype>(args[1]->Uint32Value()),
                                             (VGfloat) args[2]->NumberValue(),
                                             (VGfloat) args[3]->NumberValue(),
                                             (VGint) args[4]->Int32Value(),
                                             (VGint) args[5]->Int32Value(),
                                             (VGbitfield) args[6]->Uint32Value())));
}

NAN_METHOD(openvg::ClearPath) {
  NanScope();

  CheckArgs2(clearPath, VGPath, Number, capabilities, Uint32);

  vgClearPath((VGPath) args[0]->Uint32Value(),
              (VGbitfield) args[1]->Uint32Value());

  NanReturnUndefined();
}

NAN_METHOD(openvg::DestroyPath) {
  NanScope();

  CheckArgs1(destroyPath, VGPath, Number);

  vgDestroyPath((VGPath) args[0]->Uint32Value());

  NanReturnUndefined();
}

NAN_METHOD(openvg::RemovePathCapabilities) {
  NanScope();

  CheckArgs2(removePathCapabilities, VGPath, Number, capabilities, Uint32);

  vgRemovePathCapabilities((VGPath) args[0]->Uint32Value(),
                           (VGbitfield) args[1]->Uint32Value());

  NanReturnUndefined();
}

NAN_METHOD(openvg::GetPathCapabilities) {
  NanScope();

  CheckArgs1(getPathCapabilities, VGPath, Number);

  NanReturnValue(NanNew<Uint32>(vgGetPathCapabilities((VGPath) args[0]->Int32Value())));
}

NAN_METHOD(openvg::AppendPath) {
  NanScope();

  CheckArgs2(appendPath, dstPath, Number, srcPath, Number);

  vgAppendPath((VGPath) args[0]->Uint32Value(),
               (VGPath) args[1]->Uint32Value());

  NanReturnUndefined();
}

NAN_METHOD(openvg::AppendPathData) {
  NanScope();

  CheckArgs4(appendPathData,
             dstPath, Number, numSegments, Int32, Uint8Array, Object,
             pathData, Object);

  TypedArrayWrapper<VGubyte> segments(args[2]);
  TypedArrayWrapper<void> data(args[3]);

  vgAppendPathData((VGPath) args[0]->Uint32Value(),
                   (VGint) args[1]->Int32Value(),
                   segments.pointer(),
                   data.pointer());

  NanReturnUndefined();
}

NAN_METHOD(openvg::AppendPathDataO) {
  NanScope();

  CheckArgs4(appendPathData,
             dstPath, Number, numSegments, Int32, Uint8Array, Object,
             pathData, Object);

  TypedArrayWrapper<VGubyte> segments(args[2]);
  TypedArrayWrapper<void> data(args[4]);

  vgAppendPathData((VGPath) args[0]->Uint32Value(),
                   (VGint) args[1]->Int32Value(),
                   segments.pointer(args[3]->Uint32Value()),
                   data.pointer(args[5]->Int32Value()));

  NanReturnUndefined();
}

NAN_METHOD(openvg::ModifyPathCoords) {
  NanScope();

  CheckArgs4(modifyPathCoords,
             VGPath, Number, startIndex, Int32, numSegments, Int32,
             pathData, Object);

  TypedArrayWrapper<void> data(args[3]);

  vgModifyPathCoords((VGPath) args[0]->Uint32Value(),
                     (VGint) args[1]->Int32Value(),
                     (VGint) args[2]->Int32Value(),
                     data.pointer());

  NanReturnUndefined();
}

NAN_METHOD(openvg::TransformPath) {
  NanScope();

  CheckArgs2(transformPath, dstPath, Number, srcPath, Number);

  vgTransformPath((VGPath) args[0]->Uint32Value(),
                  (VGPath) args[1]->Uint32Value());

  NanReturnUndefined();
}

NAN_METHOD(openvg::InterpolatePath) {
  NanScope();

  CheckArgs4(interpolatePath,
             dstPath, Number, startPath, Number, endPath, Number,
             amount, Number);

  NanReturnValue(NanNew<Boolean>(vgInterpolatePath((VGPath) args[0]->Uint32Value(),
                                                   (VGPath) args[1]->Uint32Value(),
                                                   (VGPath) args[2]->Uint32Value(),
                                                   (VGfloat) args[3]->NumberValue())));
}

NAN_METHOD(openvg::PathLength) {
  NanScope();

  CheckArgs3(pathLength, path, Number,
             startSegment, Int32, numSegments, Int32);

  NanReturnValue(NanNew<Number>(vgPathLength((VGPath) args[0]->Uint32Value(),
                                             (VGint) args[1]->Int32Value(),
                                             (VGint) args[2]->Int32Value())));
}

NAN_METHOD(openvg::PointAlongPath) {
  NanScope();

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
  point->Set(NanNew<String>("x"), NanNew<Number>(x));
  point->Set(NanNew<String>("y"), NanNew<Number>(y));
  point->Set(NanNew<String>("tx"), NanNew<Number>(tx));
  point->Set(NanNew<String>("ty"), NanNew<Number>(ty));

  NanReturnUndefined();
}

NAN_METHOD(openvg::PathBounds) {
  NanScope();

  CheckArgs2(pathBounds, VGPath, Number, bounds, Object);

  VGfloat minX, minY, width, height;

  vgPathBounds((VGPath) args[0]->Uint32Value(),
               &minX, &minY, &width, &height);

  Local<Object> bounds = args[1].As<Object>();
  bounds->Set(NanNew<String>("x"), NanNew<Number>(minX));
  bounds->Set(NanNew<String>("y"), NanNew<Number>(minY));
  bounds->Set(NanNew<String>("w"), NanNew<Number>(width));
  bounds->Set(NanNew<String>("h"), NanNew<Number>(height));

  NanReturnUndefined();
}

NAN_METHOD(openvg::PathTransformedBounds) {
  NanScope();

  CheckArgs2(pathTransformedBounds, VGPath, Number, bounds, Object);

  VGfloat minX, minY, width, height;

  vgPathTransformedBounds((VGPath) args[0]->Uint32Value(),
                          &minX, &minY, &width, &height);

  Local<Object> bounds = args[1].As<Object>();
  bounds->Set(NanNew<String>("x"), NanNew<Number>(minX));
  bounds->Set(NanNew<String>("y"), NanNew<Number>(minY));
  bounds->Set(NanNew<String>("w"), NanNew<Number>(width));
  bounds->Set(NanNew<String>("h"), NanNew<Number>(height));

  NanReturnUndefined();
}

NAN_METHOD(openvg::DrawPath) {
  NanScope();

  CheckArgs2(drawPath, VGPath, Number, paintModes, Number);

  vgDrawPath((VGPath) args[0]->Uint32Value(),
             (VGbitfield) args[1]->Uint32Value());

  NanReturnUndefined();
}


/* Paint */


NAN_METHOD(openvg::CreatePaint) {
  NanScope();

  CheckArgs0(createPaint);

  NanReturnValue(NanNew<Uint32>(vgCreatePaint()));
}

NAN_METHOD(openvg::DestroyPaint) {
  NanScope();

  CheckArgs1(destroyPaint, VGPaint, Number);

  vgDestroyPaint((VGPaint) args[0]->Uint32Value());

  NanReturnUndefined();
}

NAN_METHOD(openvg::SetPaint) {
  NanScope();

  CheckArgs2(setPaint, VGPaint, Number, paintModes, Number);

  vgSetPaint((VGPaint) args[0]->Uint32Value(),
             (VGbitfield) args[1]->Uint32Value());

  NanReturnUndefined();
}

NAN_METHOD(openvg::GetPaint) {
  NanScope();

  CheckArgs1(getPaint, VGPaint, Uint32);

  NanReturnValue(NanNew<Uint32>(vgGetPaint(static_cast<VGPaintMode>(args[0]->Uint32Value()))));
}

NAN_METHOD(openvg::SetColor) {
  NanScope();

  CheckArgs2(setColor, VGPaint, Uint32, rgba, Uint32);

  vgSetColor((VGPaint) args[0]->Uint32Value(),
             (VGuint) args[1]->Uint32Value());

  NanReturnUndefined();
}

NAN_METHOD(openvg::GetColor) {
  NanScope();

  CheckArgs1(getColor, VGPaint, Uint32);

  NanReturnValue(NanNew<Uint32>(vgGetColor((VGPaint) args[0]->Uint32Value())));
}

NAN_METHOD(openvg::PaintPattern) {
  NanScope();

  CheckArgs2(paintPattern, VGPaint, Uint32, VGImage, Uint32);

  vgPaintPattern((VGPaint) args[0]->Uint32Value(),
                 (VGImage) args[1]->Uint32Value());

  NanReturnUndefined();
}


/* Images */


NAN_METHOD(openvg::CreateImage) {
  NanScope();

  CheckArgs4(createImage,
             VGImageFormat, Uint32, width, Int32, height, Int32,
             allowedQuality, Uint32);

  NanReturnValue(NanNew<Uint32>(vgCreateImage(static_cast<VGImageFormat>(args[0]->Uint32Value()),
                                              (VGint) args[1]->Int32Value(),
                                              (VGint) args[2]->Int32Value(),
                                              (VGuint) args[3]->Uint32Value())));
}

NAN_METHOD(openvg::DestroyImage) {
  NanScope();

  CheckArgs1(destroyImage, VGImage, Number);

  vgDestroyImage((VGImage) (VGPaint) args[0]->Uint32Value());

  NanReturnUndefined();
}

NAN_METHOD(openvg::ClearImage) {
  NanScope();

  CheckArgs5(clearImage,
             VGImage, Number, x, Int32, y, Int32, width, Int32, height, Int32);

  vgClearImage((VGImage) args[0]->Uint32Value(),
               (VGint) args[1]->Int32Value(),
               (VGint) args[2]->Int32Value(),
               (VGint) args[3]->Int32Value(),
               (VGint) args[4]->Int32Value());

  NanReturnUndefined();
}

NAN_METHOD(openvg::ImageSubData) {
  NanScope();

  CheckArgs8(imageSubData,
             VGImage, Number, data, Object, dataStride, Int32,
             dataFormat, Uint32,
             x, Int32, y, Int32, width, Int32, height, Int32);

  Local<Object> data = args[1]->ToObject();
  void *dataPointer;

  Local<Value> nativeBuffer = data->Get(NanNew<String>("buffer"));
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

  NanReturnUndefined();
}

NAN_METHOD(openvg::GetImageSubData) {
  NanScope();

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

  NanReturnUndefined();
}

NAN_METHOD(openvg::ChildImage) {
  NanScope();

  CheckArgs5(childImage,
             VGImage, Number, x, Int32, y, Int32, width, Int32, height, Int32);

  NanReturnValue(NanNew<Uint32>(vgChildImage((VGImage) args[0]->Uint32Value(),
                                     (VGint) args[1]->Int32Value(),
                                     (VGint) args[2]->Int32Value(),
                                     (VGint) args[3]->Int32Value(),
                                     (VGint) args[4]->Int32Value())));
}

NAN_METHOD(openvg::GetParent) {
  NanScope();

  CheckArgs1(getParent, VGImage, Number);

  NanReturnValue(NanNew<Uint32>(vgGetParent((VGImage) args[0]->Uint32Value())));
}

NAN_METHOD(openvg::CopyImage) {
  NanScope();

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

  NanReturnUndefined();
}

NAN_METHOD(openvg::DrawImage) {
  NanScope();

  CheckArgs1(drawImage, VGImage, Number);

  vgDrawImage((VGImage) args[0]->Uint32Value());

  NanReturnUndefined();
}

NAN_METHOD(openvg::SetPixels) {
  NanScope();

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

  NanReturnUndefined();
}

NAN_METHOD(openvg::WritePixels) {
  NanScope();

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

  NanReturnUndefined();
}

NAN_METHOD(openvg::GetPixels) {
  NanScope();

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

  NanReturnUndefined();
}

NAN_METHOD(openvg::ReadPixels) {
  NanScope();

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

  NanReturnUndefined();
}

NAN_METHOD(openvg::CopyPixels) {
  NanScope();

  CheckArgs6(copyPixels,
             dx, Int32, dy, Int32, sx, Int32, sy, Int32,
             width, Int32, height, Int32);

  vgCopyPixels((VGint) args[0]->Int32Value(),
               (VGint) args[1]->Int32Value(),
               (VGint) args[2]->Int32Value(),
               (VGint) args[3]->Int32Value(),
               (VGint) args[4]->Int32Value(),
               (VGint) args[5]->Int32Value());

  NanReturnUndefined();
}


/* Text */


NAN_METHOD(openvg::CreateFont) {
  NanScope();

  CheckArgs1(createFont, glyphCapacityHint, Int32);

  NanReturnValue(NanNew<Uint32>(vgCreateFont((VGint) args[0]->Int32Value())));
}

NAN_METHOD(openvg::DestroyFont) {
  NanScope();

  CheckArgs1(destroyFont, VGFont, Number);

  vgDestroyFont((VGFont) args[0]->Uint32Value());

  NanReturnUndefined();
}

NAN_METHOD(openvg::SetGlyphToPath) {
  NanScope();

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

  NanReturnUndefined();
}

NAN_METHOD(openvg::SetGlyphToImage) {
  NanScope();

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

  NanReturnUndefined();
}

NAN_METHOD(openvg::ClearGlyph) {
  NanScope();

  CheckArgs2(clearGlyph, VGFont, Number, glyphIndex, Uint32);

  vgClearGlyph((VGFont) args[0]->Uint32Value(),
               (VGuint) args[1]->Uint32Value());

  NanReturnUndefined();
}

NAN_METHOD(openvg::DrawGlyph) {
  NanScope();

  CheckArgs4(drawGlyph, VGFont, Number, glyphIndex, Uint32,
             paintModes, Uint32, allowAutoHinting, Boolean);

  vgDrawGlyph((VGFont) args[0]->Uint32Value(),
              (VGuint) args[1]->Uint32Value(),
              (VGbitfield) args[2]->Uint32Value(),
              (VGboolean) args[3]->BooleanValue());

  NanReturnUndefined();
}

NAN_METHOD(openvg::DrawGlyphs) {
  NanScope();

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

  NanReturnUndefined();
}


/* Image Filters */


NAN_METHOD(openvg::ColorMatrix) {
  NanScope();

  CheckArgs3(colorMatrix,
             dstVGImage, Number, srcVGImage, Number, matrix, Object);

  TypedArrayWrapper<VGfloat> matrix(args[2]);

  vgColorMatrix((VGImage) args[0]->Uint32Value(),
                (VGImage) args[1]->Uint32Value(),
                matrix.pointer());

  NanReturnUndefined();
}

NAN_METHOD(openvg::Convolve) {
  NanScope();

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

  NanReturnUndefined();
}

NAN_METHOD(openvg::SeparableConvolve) {
  NanScope();

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

  NanReturnUndefined();
}

NAN_METHOD(openvg::GaussianBlur) {
  NanScope();

  CheckArgs5(gaussianBlur, dstVGImage, Number, srcVGImage, Number,
             stdDeviationX, Number, stdDeviationY, Number,
             tilingMode, Uint32);

  vgGaussianBlur((VGImage) args[0]->Uint32Value(),
                 (VGImage) args[1]->Uint32Value(),
                 (VGfloat) args[2]->NumberValue(),
                 (VGfloat) args[3]->NumberValue(),
                 static_cast<VGTilingMode>(args[4]->Uint32Value()));

  NanReturnUndefined();
}

NAN_METHOD(openvg::Lookup) {
  NanScope();

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

  NanReturnUndefined();
}

NAN_METHOD(openvg::LookupSingle) {
  NanScope();

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

  NanReturnUndefined();
}


/* Hardware Queries */
NAN_METHOD(openvg::HardwareQuery) {
  NanScope();

  CheckArgs2(hardwareQuery, key, Uint32, setting, Int32);

  NanReturnValue(NanNew<Uint32>(vgHardwareQuery(static_cast<VGHardwareQueryType>(args[0]->Uint32Value()),
                                                (VGint) args[1]->Int32Value())));
}


/* Renderer and Extension Information */
VG_API_CALL const VGubyte * VG_API_ENTRY vgGetString(VGStringID name) VG_API_EXIT;

NAN_METHOD(openvg::GetString) {
  NanScope();

  CheckArgs1(getString, key, Uint32);

  NanReturnValue(NanNew<String>((char*) vgGetString(static_cast<VGStringID>(args[0]->Uint32Value()))));
}


/* Utilities */


NAN_METHOD(openvg::vgu::Line) {
  NanScope();

  CheckArgs5(line,
             VGPath, Number, x0, Number, y0, Number, x1, Number, y1, Number);

  NanReturnValue(NanNew<Uint32>(vguLine((VGPath) args[0]->Uint32Value(),
                                        (VGfloat) args[1]->NumberValue(),
                                        (VGfloat) args[2]->NumberValue(),
                                        (VGfloat) args[3]->NumberValue(),
                                        (VGfloat) args[4]->NumberValue())));
}

NAN_METHOD(openvg::vgu::Polygon) {
  NanScope();

  CheckArgs4(polygon,
             VGPath, Number, Float32Array, Object, count, Int32,
             closed, Boolean);

  TypedArrayWrapper<VGfloat> points(args[1]);

  NanReturnValue(NanNew<Uint32>(vguPolygon((VGPath) args[0]->Uint32Value(),
                                           points.pointer(),
                                           (VGint) args[2]->Int32Value(),
                                           (VGboolean) args[3]->BooleanValue())));
}

NAN_METHOD(openvg::vgu::Rect) {
  NanScope();

  CheckArgs5(rect, VGPath, Number, x, Number, y, Number,
             width, Number, height, Number);

  NanReturnValue(NanNew<Uint32>(vguRect((VGPath) args[0]->Uint32Value(),
                                        (VGfloat) args[1]->NumberValue(),
                                        (VGfloat) args[2]->NumberValue(),
                                        (VGfloat) args[3]->NumberValue(),
                                        (VGfloat) args[4]->NumberValue())));
}

NAN_METHOD(openvg::vgu::RoundRect) {
  NanScope();

  CheckArgs7(rect, VGPath,
             Number, x, Number, y, Number, width, Number, height,
             Number, arcWidth, Number, arcHeight, Number);

  NanReturnValue(NanNew<Uint32>(vguRoundRect((VGPath) args[0]->Uint32Value(),
                                             (VGfloat) args[1]->NumberValue(),
                                             (VGfloat) args[2]->NumberValue(),
                                             (VGfloat) args[3]->NumberValue(),
                                             (VGfloat) args[4]->NumberValue(),
                                             (VGfloat) args[5]->NumberValue(),
                                             (VGfloat) args[6]->NumberValue())));
}

NAN_METHOD(openvg::vgu::Ellipse) {
  NanScope();

  CheckArgs5(ellipse, VGPath, Number, x, Number, y, Number,
             width, Number, height, Number);

  NanReturnValue(NanNew<Uint32>(vguEllipse((VGPath) args[0]->Uint32Value(),
                                           (VGfloat) args[1]->NumberValue(),
                                           (VGfloat) args[2]->NumberValue(),
                                           (VGfloat) args[3]->NumberValue(),
                                           (VGfloat) args[4]->NumberValue())));
}

NAN_METHOD(openvg::vgu::Arc) {
  NanScope();

  CheckArgs8(arc,
             VGPath, Number, x, Number, y, Number,
             width, Number, height, Number,
             startAngle, Number, angleExtent, Number, VGUArcType, Uint32);

  NanReturnValue(NanNew<Uint32>(vguArc((VGPath) args[0]->Uint32Value(),
                                       (VGfloat) args[1]->NumberValue(),
                                       (VGfloat) args[2]->NumberValue(),
                                       (VGfloat) args[3]->NumberValue(),
                                       (VGfloat) args[4]->NumberValue(),
                                       (VGfloat) args[5]->NumberValue(),
                                       (VGfloat) args[6]->NumberValue(),
                                       static_cast<VGUArcType>(args[7]->Uint32Value()))));
}

NAN_METHOD(openvg::vgu::ComputeWarpQuadToSquare) {
  NanScope();

  CheckArgs9(computeWarpQuadToSquare,
             sx0, Number, sy0, Number, sx1, Number, sy1, Number,
             sx2, Number, sy2, Number, sx3, Number, sy3, Number,
             Float32Array, Object);

  TypedArrayWrapper<VGfloat> matrix(args[8]);

  NanReturnValue(NanNew<Uint32>(vguComputeWarpQuadToSquare((VGfloat) args[0]->NumberValue(),
                                                           (VGfloat) args[1]->NumberValue(),
                                                           (VGfloat) args[2]->NumberValue(),
                                                           (VGfloat) args[3]->NumberValue(),
                                                           (VGfloat) args[4]->NumberValue(),
                                                           (VGfloat) args[5]->NumberValue(),
                                                           (VGfloat) args[6]->NumberValue(),
                                                           (VGfloat) args[7]->NumberValue(),
                                                           matrix.pointer())));
}

NAN_METHOD(openvg::vgu::ComputeWarpSquareToQuad) {
  NanScope();

  CheckArgs9(computeWarpSquareToQuad,
             sx0, Number, sy0, Number, sx1, Number, sy1, Number,
             sx2, Number, sy2, Number, sx3, Number, sy3, Number,
             Float32Array, Object);

  TypedArrayWrapper<VGfloat> matrix(args[8]);

  NanReturnValue(NanNew<Uint32>(vguComputeWarpSquareToQuad((VGfloat) args[0]->NumberValue(),
                                                           (VGfloat) args[1]->NumberValue(),
                                                           (VGfloat) args[2]->NumberValue(),
                                                           (VGfloat) args[3]->NumberValue(),
                                                           (VGfloat) args[4]->NumberValue(),
                                                           (VGfloat) args[5]->NumberValue(),
                                                           (VGfloat) args[6]->NumberValue(),
                                                           (VGfloat) args[7]->NumberValue(),
                                                           matrix.pointer())));
}

NAN_METHOD(openvg::vgu::ComputeWarpQuadToQuad) {
  NanScope();

  // No arg check -> Would be a 17 arg macro

  TypedArrayWrapper<VGfloat> matrix(args[16]);

  NanReturnValue(NanNew<Uint32>(vguComputeWarpQuadToQuad((VGfloat) args[ 0]->NumberValue(),
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


NAN_METHOD(openvg::ext::CreateEGLImageTargetKHR) {
  NanScope();

  CheckArgs1(createEGLImageTargetKHR, VGeglImageKHR, Object);

#ifdef VG_VGEXT_PROTOTYPES
  VGeglImageKHR image = node::ObjectWrap::Unwrap<VGeglImageKHR>(args[0]->ToObject());
  NanReturnValue(NanNew<Uint32>(vgCreateEGLImageTargetKHR(image)));
#else
  NanReturnUndefined();
#endif
}

NAN_METHOD(openvg::ext::IterativeAverageBlurKHR) {
  NanScope();

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
  NanReturnUndefined();
}

NAN_METHOD(openvg::ext::ParametricFilterKHR) {
  NanScope();

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

  NanReturnUndefined();
}

NAN_METHOD(openvg::ext::DropShadowKHR) {
  NanScope();

  CheckArgs11(dropShadowKHR,
              dstVGImage, Number, srcVGImage, Number,
              dimX, Number, dimY, Number, iterative, Number,
              strength, Number, distance, Number, angle, Number,
              filterFlags, Number, allowedQuality, Number,
              shadowColorRGBA, Number);

#ifdef VG_VGEXT_PROTOTYPES
  NanReturnValue(NanNew<Uint32>(vguDropShadowKHR((VGImage) args[0]->Uint32Value(),
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
  NanReturnUndefined();
#endif
}

NAN_METHOD(openvg::ext::GlowKHR) {
  NanScope();

  CheckArgs9(glowKHR,
             dstVGImage, Number, srcVGImage, Number,
             dimX, Number, dimY, Number, iterative, Number,
             strength, Number,
             filterFlags, Number, allowedQuality, Number,
             glowColorRGBA, Number);

#ifdef VG_VGEXT_PROTOTYPES
  NanReturnValue(NanNew<Uint32>(vguGlowKHR((VGImage) args[0]->Uint32Value(),
                                           (VGImage) args[1]->Uint32Value(),
                                           (VGfloat) args[2]->NumberValue(),
                                           (VGfloat) args[3]->NumberValue(),
                                           (VGuint) args[4]->Uint32Value(),
                                           (VGfloat) args[5]->NumberValue(),
                                           (VGbitfield) args[6]->Uint32Value(),
                                           (VGbitfield) args[7]->Uint32Value(),
                                           (VGuint) args[8]->Uint32Value())));
#else
  NanReturnUndefined();
#endif
}

NAN_METHOD(openvg::ext::BevelKHR) {
  NanScope();

  CheckArgs12(bevelKHR,
              dstVGImage, Number, srcVGImage, Number,
              dimX, Number, dimY, Number, iterative, Number,
              strength, Number, distance, Number, angle, Number,
              filterFlags, Number, allowedQuality, Number,
              highlightColorRGBA, Number, shadowColorRGBA, Number);

#ifdef VG_VGEXT_PROTOTYPES
  NanReturnValue(NanNew<Uint32>(vguBevelKHR((VGImage) args[0]->Uint32Value(),
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
  NanReturnUndefined();
#endif
}

NAN_METHOD(openvg::ext::GradientGlowKHR) {
  NanScope();

  CheckArgs12(gradientGlowKHR,
              dstVGImage, Number, srcVGImage, Number,
              dimX, Number, dimY, Number, iterative, Number,
              strength, Number, distance, Number, angle, Number,
              filterFlags, Number, allowedQuality, Number,
              stopsCount, Number, Float32Array, Object);

#ifdef VG_VGEXT_PROTOTYPES
  TypedArrayWrapper<VGfloat> glowColorRampStops(args[11]);

  NanReturnValue(NanNew<Uint32>(vguGradientGlowKHR((VGImage) args[0]->Uint32Value(),
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
  NanReturnUndefined();
#endif
}

NAN_METHOD(openvg::ext::GradientBevelKHR) {
  NanScope();

  CheckArgs12(gradientBevelKHR,
              dstVGImage, Number, srcVGImage, Number,
              dimX, Number, dimY, Number, iterative, Number,
              strength, Number, distance, Number, angle, Number,
              filterFlags, Number, allowedQuality, Number,
              stopsCount, Number, Float32Array, Object);

#ifdef VG_VGEXT_PROTOTYPES
  TypedArrayWrapper<VGfloat> bevelColorRampStops(args[11]);

  NanReturnValue(NanNew<Uint32>(vguGradientBevelKHR((VGImage) args[0]->Uint32Value(),
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
  NanReturnUndefined();
#endif
}

NAN_METHOD(openvg::ext::ProjectiveMatrixNDS) {
  NanScope();

  CheckArgs1(projectiveMatrixNDS, enable, Boolean);

#ifdef VG_VGEXT_PROTOTYPES
  vgProjectiveMatrixNDS((VGboolean) args[0]->BooleanValue());
#endif

  NanReturnUndefined();
}

NAN_METHOD(openvg::ext::TransformClipLineNDS) {
  NanScope();

  CheckArgs8(gradientBevelKHR,
             Ain, Number, Bin, Number, Cin, Number,
             Float32Array, Object, inverse, Boolean,
             Float32Array, Object, Float32Array, Object, Float32Array, Object);

#ifdef VG_VGEXT_PROTOTYPES
  TypedArrayWrapper<VGfloat> matrix(args[3]);
  TypedArrayWrapper<VGfloat> Aout(args[5]);
  TypedArrayWrapper<VGfloat> Bout(args[6]);
  TypedArrayWrapper<VGfloat> Cout(args[7]);

  NanReturnValue(NanNew<Uint32>(vguTransformClipLineNDS((VGfloat) args[0]->NumberValue(),
                                                        (VGfloat) args[1]->NumberValue(),
                                                        (VGfloat) args[2]->NumberValue(),
                                                        matrix.pointer(),
                                                        (VGboolean) args[4]->BooleanValue(),
                                                        Aout.pointer(),
                                                        Bout.pointer(),
                                                        Cout.pointer())));
#else
  NanReturnUndefined();
#endif
}
