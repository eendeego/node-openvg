#include <stdio.h>

#include "VG/openvg.h"
#include "VG/vgu.h"
#include "VG/vgext.h"

#include <v8.h>
//#include <node.h>
#include <node_buffer.h>
#include "nan.h"

#include "openvg.h"
#include "egl.h"
#include "argchecks.h"

const bool kInitOpenGLES = false;

void init(Handle<Object> target) {
  Nan::SetMethod(target, "startUp"          , openvg::StartUp);
  Nan::SetMethod(target, "shutdown"         , openvg::Shutdown);

  Nan::SetMethod(target, "getError"         , openvg::GetError);

  Nan::SetMethod(target, "flush"            , openvg::Flush);
  Nan::SetMethod(target, "finish"           , openvg::Finish);

  /* Getters and Setters */
  Nan::SetMethod(target, "setF"             , openvg::SetF);
  Nan::SetMethod(target, "setI"             , openvg::SetI);
  Nan::SetMethod(target, "setFV"            , openvg::SetFV);
  Nan::SetMethod(target, "setIV"            , openvg::SetIV);
  Nan::SetMethod(target, "setFVOL"          , openvg::SetFVOL); // Offset + Length
  Nan::SetMethod(target, "setIVOL"          , openvg::SetIVOL); // Offset + Length

  Nan::SetMethod(target, "getF"             , openvg::GetF);
  Nan::SetMethod(target, "getI"             , openvg::GetI);
  Nan::SetMethod(target, "getVectorSize"    , openvg::GetVectorSize);
  Nan::SetMethod(target, "getFV"            , openvg::GetFV);
  Nan::SetMethod(target, "getIV"            , openvg::GetIV);
  Nan::SetMethod(target, "getFVOL"          , openvg::GetFVOL); // Offset + Length
  Nan::SetMethod(target, "getIVOL"          , openvg::GetIVOL); // Offset + Length

  Nan::SetMethod(target, "setParameterF"    , openvg::SetParameterF);
  Nan::SetMethod(target, "setParameterI"    , openvg::SetParameterI);
  Nan::SetMethod(target, "setParameterFV"   , openvg::SetParameterFV);
  Nan::SetMethod(target, "setParameterIV"   , openvg::SetParameterIV);
  Nan::SetMethod(target, "setParameterFVOL" , openvg::SetParameterFVOL);
  Nan::SetMethod(target, "setParameterIVOL" , openvg::SetParameterIVOL);

  Nan::SetMethod(target, "getParameterF"    , openvg::GetParameterF);
  Nan::SetMethod(target, "getParameterI"    , openvg::GetParameterI);
  Nan::SetMethod(target, "getParameterVectorSize", openvg::GetParameterVectorSize);
  Nan::SetMethod(target, "getParameterFV"   , openvg::GetParameterFV);
  Nan::SetMethod(target, "getParameterIV"   , openvg::GetParameterIV);
  Nan::SetMethod(target, "getParameterFVOL" , openvg::GetParameterFVOL); // Offset + Length
  Nan::SetMethod(target, "getParameterIVOL" , openvg::GetParameterIVOL); // Offset + Length

  /* Matrix Manipulation */
  Nan::SetMethod(target, "loadIdentity"     , openvg::LoadIdentity);
  Nan::SetMethod(target, "loadMatrix"       , openvg::LoadMatrix);
  Nan::SetMethod(target, "getMatrix"        , openvg::GetMatrix);
  Nan::SetMethod(target, "multMatrix"       , openvg::MultMatrix);
  Nan::SetMethod(target, "translate"        , openvg::Translate);
  Nan::SetMethod(target, "scale"            , openvg::Scale);
  Nan::SetMethod(target, "shear"            , openvg::Shear);
  Nan::SetMethod(target, "rotate"           , openvg::Rotate);

  /* Masking and Clearing */
  Nan::SetMethod(target, "mask"             , openvg::Mask);
  Nan::SetMethod(target, "renderToMask"     , openvg::RenderToMask);
  Nan::SetMethod(target, "createMaskLayer"  , openvg::CreateMaskLayer);
  Nan::SetMethod(target, "destroyMaskLayer" , openvg::DestroyMaskLayer);
  Nan::SetMethod(target, "fillMaskLayer"    , openvg::FillMaskLayer);
  Nan::SetMethod(target, "copyMask"         , openvg::CopyMask);
  Nan::SetMethod(target, "clear"            , openvg::Clear);

  /* Paths */
  Nan::SetMethod(target, "createPath"       , openvg::CreatePath);
  Nan::SetMethod(target, "clearPath"        , openvg::ClearPath);
  Nan::SetMethod(target, "destroyPath"      , openvg::DestroyPath);
  Nan::SetMethod(target, "removePathCapabilities",
                          openvg::RemovePathCapabilities);
  Nan::SetMethod(target, "getPathCapabilities",
                          openvg::GetPathCapabilities);
  Nan::SetMethod(target, "appendPath"       , openvg::AppendPath);
  Nan::SetMethod(target, "appendPathData"   , openvg::AppendPathData);
  Nan::SetMethod(target, "appendPathDataO"  , openvg::AppendPathDataO); // Offsets
  Nan::SetMethod(target, "modifyPathCoords" , openvg::ModifyPathCoords);
  Nan::SetMethod(target, "transformPath"    , openvg::TransformPath);
  Nan::SetMethod(target, "interpolatePath"  , openvg::InterpolatePath);
  Nan::SetMethod(target, "pathLength"       , openvg::PathLength);
  Nan::SetMethod(target, "pointAlongPath"   , openvg::PointAlongPath);
  Nan::SetMethod(target, "pathBounds"       , openvg::PathBounds);
  Nan::SetMethod(target, "pathTransformedBounds",
                          openvg::PathTransformedBounds);
  Nan::SetMethod(target, "drawPath"         , openvg::DrawPath);

  /* Paint */
  Nan::SetMethod(target, "createPaint"      , openvg::CreatePaint);
  Nan::SetMethod(target, "destroyPaint"     , openvg::DestroyPaint);
  Nan::SetMethod(target, "setPaint"         , openvg::SetPaint);
  Nan::SetMethod(target, "getPaint"         , openvg::GetPaint);
  Nan::SetMethod(target, "setColor"         , openvg::SetColor);
  Nan::SetMethod(target, "getColor"         , openvg::GetColor);
  Nan::SetMethod(target, "paintPattern"     , openvg::PaintPattern);

  /* Images */
  Nan::SetMethod(target, "createImage"      , openvg::CreateImage);
  Nan::SetMethod(target, "destroyImage"     , openvg::DestroyImage);
  Nan::SetMethod(target, "clearImage"       , openvg::ClearImage);
  Nan::SetMethod(target, "imageSubData"     , openvg::ImageSubData);
  Nan::SetMethod(target, "getImageSubData"  , openvg::GetImageSubData);
  Nan::SetMethod(target, "childImage"       , openvg::ChildImage);
  Nan::SetMethod(target, "getParent"        , openvg::GetParent);
  Nan::SetMethod(target, "copyImage"        , openvg::CopyImage);
  Nan::SetMethod(target, "drawImage"        , openvg::DrawImage);
  Nan::SetMethod(target, "setPixels"        , openvg::SetPixels);
  Nan::SetMethod(target, "writePixels"      , openvg::WritePixels);
  Nan::SetMethod(target, "getPixels"        , openvg::GetPixels);
  Nan::SetMethod(target, "readPixels"       , openvg::ReadPixels);
  Nan::SetMethod(target, "copyPixels"       , openvg::CopyPixels);

  /* Text */
  Nan::SetMethod(target, "createFont"       , openvg::CreateFont);
  Nan::SetMethod(target, "destroyFont"      , openvg::DestroyFont);
  Nan::SetMethod(target, "setGlyphToPath"   , openvg::SetGlyphToPath);
  Nan::SetMethod(target, "setGlyphToImage"  , openvg::SetGlyphToImage);
  Nan::SetMethod(target, "clearGlyph"       , openvg::ClearGlyph);
  Nan::SetMethod(target, "drawGlyph"        , openvg::DrawGlyph);
  Nan::SetMethod(target, "drawGlyphs"       , openvg::DrawGlyphs);

  /* Image Filters */
  Nan::SetMethod(target, "colorMatrix"      , openvg::ColorMatrix);
  Nan::SetMethod(target, "convolve"         , openvg::Convolve);
  Nan::SetMethod(target, "separableConvolve", openvg::SeparableConvolve);
  Nan::SetMethod(target, "gaussianBlur"     , openvg::GaussianBlur);
  Nan::SetMethod(target, "lookup"           , openvg::Lookup);
  Nan::SetMethod(target, "lookupSingle"     , openvg::LookupSingle);

  /* Hardware Queries */
  Nan::SetMethod(target, "hardwareQuery"    , openvg::HardwareQuery);

  /* Renderer and Extension Information */
  Nan::SetMethod(target, "getString"        , openvg::GetString);

  /* Utilities */
  Local<Object> VGU = Nan::New<Object>();
  Nan::Set(target, Nan::New<String>("vgu").ToLocalChecked(), VGU);

  Nan::SetMethod(VGU, "line"                   , openvg::vgu::Line);
  Nan::SetMethod(VGU, "polygon"                , openvg::vgu::Polygon);
  Nan::SetMethod(VGU, "rect"                   , openvg::vgu::Rect);
  Nan::SetMethod(VGU, "roundRect"              , openvg::vgu::RoundRect);
  Nan::SetMethod(VGU, "ellipse"                , openvg::vgu::Ellipse);
  Nan::SetMethod(VGU, "arc"                    , openvg::vgu::Arc);
  Nan::SetMethod(VGU, "computeWarpQuadToSquare",
                       openvg::vgu::ComputeWarpQuadToSquare);
  Nan::SetMethod(VGU, "computeWarpSquareToQuad",
                       openvg::vgu::ComputeWarpSquareToQuad);
  Nan::SetMethod(VGU, "computeWarpQuadToQuad",
                       openvg::vgu::ComputeWarpQuadToQuad);

  /* KHR extensions */
  Local<Object> ext = Nan::New<Object>();
  Nan::Set(target, Nan::New<String>("ext").ToLocalChecked(), ext);

  Nan::SetMethod(ext, "createEGLImageTargetKHR",
                       openvg::ext::CreateEGLImageTargetKHR);

  Nan::SetMethod(ext, "iterativeAverageBlurKHR",
                       openvg::ext::IterativeAverageBlurKHR);

  Nan::SetMethod(ext, "parametricFilterKHR", openvg::ext::ParametricFilterKHR);
  Nan::SetMethod(ext, "dropShadowKHR"      , openvg::ext::DropShadowKHR);
  Nan::SetMethod(ext, "glowKHR"            , openvg::ext::GlowKHR);
  Nan::SetMethod(ext, "bevelKHR"           , openvg::ext::BevelKHR);
  Nan::SetMethod(ext, "gradientGlowKHR"    , openvg::ext::GradientGlowKHR);
  Nan::SetMethod(ext, "gradientBevelKHR"   , openvg::ext::GradientBevelKHR);

  Nan::SetMethod(ext, "projectiveMatrixNDS",
                       openvg::ext::ProjectiveMatrixNDS);
  Nan::SetMethod(ext, "transformClipLineNDS",
                       openvg::ext::TransformClipLineNDS);

  /* EGL */
  Local<Object> egl = Nan::New<Object>();
  Nan::Set(target, Nan::New<String>("egl").ToLocalChecked(), egl);
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

NAN_METHOD(openvg::StartUp) {
  Nan::HandleScope scope;

  CheckArgs1(startUp, screen, Object);

  egl::Init();

  if (kInitOpenGLES) {
    egl::InitOpenGLES();
  }

  CHECK_VG_ERROR;

  Local<Object> screen = info[0].As<Object>();
  screen->Set(Nan::New<String>("width").ToLocalChecked(), Nan::New<Uint32>(egl::State.screen_width));
  screen->Set(Nan::New<String>("height").ToLocalChecked(), Nan::New<Uint32>(egl::State.screen_height));
  screen->Set(Nan::New<String>("display").ToLocalChecked(), Nan::New<External>(egl::State.display));
  screen->Set(Nan::New<String>("surface").ToLocalChecked(), Nan::New<External>(egl::State.surface));
  screen->Set(Nan::New<String>("context").ToLocalChecked(), Nan::New<External>(egl::State.context));
}

NAN_METHOD(openvg::Shutdown) {
  Nan::HandleScope scope;

  CheckArgs0(shutdown);

  egl::Finish();
}


NAN_METHOD(openvg::GetError) {
  Nan::HandleScope scope;

  CheckArgs0(getError);

  info.GetReturnValue().Set(Nan::New<Integer>(vgGetError()));
}


NAN_METHOD(openvg::Flush) {
  Nan::HandleScope scope;

  CheckArgs0(flush);

  vgFlush();

  
}

NAN_METHOD(openvg::Finish) {
  Nan::HandleScope scope;

  CheckArgs0(finish);

  vgFinish();

  
}


/* Getters and Setters */


NAN_METHOD(openvg::SetF) {
  Nan::HandleScope scope;

  CheckArgs2(setF, type, Int32, value, Number);

  vgSetf((VGParamType) info[0]->Int32Value(),
         (VGfloat) info[1]->NumberValue());

  
}

NAN_METHOD(openvg::SetI) {
  Nan::HandleScope scope;

  CheckArgs2(setI, type, Int32, value, Int32);

  vgSeti((VGParamType) info[0]->Int32Value(),
         (VGint) info[1]->Int32Value());

  
}

NAN_METHOD(openvg::SetFV) {
  Nan::HandleScope scope;

  CheckArgs2(setFV, type, Int32, Float32Array, Object);

  Nan::TypedArrayContents<VGfloat> data(info[1]);

  vgSetfv((VGParamType) info[0]->Int32Value(),
          data.length(),
          (*data));

  
}

NAN_METHOD(openvg::SetIV) {
  Nan::HandleScope scope;

  CheckArgs2(setIV, type, Int32, Int32Array, Object);

  Nan::TypedArrayContents<VGint> values(info[1]);

  vgSetiv((VGParamType) info[0]->Int32Value(),
          values.length(),
          *values);

  
}

NAN_METHOD(openvg::SetFVOL) {
  Nan::HandleScope scope;

  CheckArgs4(setFVOL, type, Int32, Float32Array, Object, offset, Int32, length, Int32);

  Nan::TypedArrayContents<VGfloat> values(info[1]);

  vgSetfv((VGParamType) info[0]->Int32Value(),
          (VGint) info[3]->Int32Value(),
          (*values) + info[2]->Int32Value());

  
}

NAN_METHOD(openvg::SetIVOL) {
  Nan::HandleScope scope;

  CheckArgs4(setIV, type, Int32, Int32Array, Object, offset, Int32, length, Int32);

  Nan::TypedArrayContents<VGint> values(info[1]);

  vgSetiv((VGParamType) info[0]->Int32Value(),
          (VGint) info[3]->Int32Value(),
          (*values) + info[2]->Int32Value());
}

NAN_METHOD(openvg::GetF) {
  Nan::HandleScope scope;

  CheckArgs1(getF, type, Int32);

  info.GetReturnValue().Set(Nan::New<Number>(vgGetf((VGParamType) info[0]->Int32Value())));
}

NAN_METHOD(openvg::GetI) {
  Nan::HandleScope scope;

  CheckArgs1(getI, type, Int32);

  info.GetReturnValue().Set(Nan::New<Integer>(vgGeti((VGParamType) info[0]->Int32Value())));
}

NAN_METHOD(openvg::GetVectorSize) {
  Nan::HandleScope scope;

  CheckArgs1(getVectorSize, type, Int32);

  info.GetReturnValue().Set(Nan::New<Integer>(vgGetVectorSize((VGParamType) info[0]->Int32Value())));
}

NAN_METHOD(openvg::GetFV) {
  Nan::HandleScope scope;

  CheckArgs2(getFV, type, Int32, Float32Array, Object);

  Nan::TypedArrayContents<VGfloat> values(info[1]);

  vgGetfv((VGParamType) info[0]->Int32Value(),
          values.length(),
          (*values));

  
}

NAN_METHOD(openvg::GetIV) {
  Nan::HandleScope scope;

  CheckArgs2(getIV, type, Int32, Float32Array, Object);

  Nan::TypedArrayContents<VGint> values(info[1]);

  vgGetiv((VGParamType) info[0]->Int32Value(),
          values.length(),
          (*values));

  
}

NAN_METHOD(openvg::GetFVOL) {
  Nan::HandleScope scope;

  CheckArgs4(getFV, type, Int32, Float32Array, Object, offset, Int32, length, Int32);

  Nan::TypedArrayContents<VGfloat> values(info[1]);

  vgGetfv((VGParamType) info[0]->Int32Value(),
          (VGint) info[3]->Int32Value(),
          (*values) + info[2]->Int32Value());

  
}

NAN_METHOD(openvg::GetIVOL) {
  Nan::HandleScope scope;

  CheckArgs4(getIV, type, Int32, Float32Array, Object, offset, Int32, length, Int32);

  Nan::TypedArrayContents<VGint> values(info[1]);

  vgGetiv((VGParamType) info[0]->Int32Value(),
          (VGint) info[3]->Int32Value(),
          (*values) + info[2]->Int32Value());

  
}


NAN_METHOD(openvg::SetParameterF) {
  Nan::HandleScope scope;

  CheckArgs3(setParameterF, VGHandle, Int32, VGParamType, Int32, value, Number);

  vgSetParameterf((VGHandle) info[0]->Int32Value(),
                  (VGParamType) info[1]->Int32Value(),
                  (VGfloat) info[2]->NumberValue());

  
}

NAN_METHOD(openvg::SetParameterI) {
  Nan::HandleScope scope;

  CheckArgs3(setParameterI, VGHandle, Int32, VGParamType, Int32, value, Int32);

  vgSetParameteri((VGHandle) info[0]->Int32Value(),
                  (VGParamType) info[1]->Int32Value(),
                  (VGint) info[2]->Int32Value());

  
}

NAN_METHOD(openvg::SetParameterFV) {
  Nan::HandleScope scope;

  CheckArgs3(setParameterFV,
             VGHandle, Int32, VGParamType, Int32, Float32Array, Object);

  Nan::TypedArrayContents<VGfloat> values(info[2]);

  vgSetParameterfv((VGHandle) info[0]->Int32Value(),
                   (VGParamType) info[1]->Int32Value(),
                   values.length(),
                   (*values));

  
}

NAN_METHOD(openvg::SetParameterIV) {
  Nan::HandleScope scope;

  CheckArgs3(setParameterIV,
             VGHandle, Int32, VGParamType, Int32, Int32Array, Object);

  Nan::TypedArrayContents<VGint> values(info[2]);

  vgSetParameteriv((VGHandle) info[0]->Int32Value(),
                   (VGParamType) info[1]->Int32Value(),
                   values.length(),
                   (*values));

  
}

NAN_METHOD(openvg::SetParameterFVOL) {
  Nan::HandleScope scope;

  CheckArgs5(setParameterFV,
             VGHandle, Int32, VGParamType, Int32, Float32Array, Object,
             offset, Int32, length, Int32);

  Nan::TypedArrayContents<VGfloat> values(info[2]);

  vgSetParameterfv((VGHandle) info[0]->Int32Value(),
                   (VGParamType) info[1]->Int32Value(),
                   (VGint) info[4]->Int32Value(),
                   (*values) + info[3]->Int32Value());

  
}

NAN_METHOD(openvg::SetParameterIVOL) {
  Nan::HandleScope scope;

  CheckArgs5(setParameterIV,
             VGHandle, Int32, VGParamType, Int32, Int32Array, Object,
             offset, Int32, length, Int32);

  Nan::TypedArrayContents<VGint> values(info[2]);

  vgSetParameteriv((VGHandle) info[0]->Int32Value(),
                   (VGParamType) info[1]->Int32Value(),
                   (VGint) info[4]->Int32Value(),
                   (*values) + info[3]->Int32Value());

  
}

NAN_METHOD(openvg::GetParameterF) {
  Nan::HandleScope scope;

  CheckArgs2(getParameterF, VGHandle, Int32, VGParamType, Int32);

  info.GetReturnValue().Set(Nan::New<Number>(vgGetParameterf((VGHandle) info[0]->Int32Value(),
                                                (VGParamType) info[1]->Int32Value())));
}

NAN_METHOD(openvg::GetParameterI) {
  Nan::HandleScope scope;

  CheckArgs2(getParameterI, VGHandle, Int32, VGParamType, Int32);

  info.GetReturnValue().Set(Nan::New<Integer>(vgGetParameteri((VGHandle) info[0]->Int32Value(),
                                                 (VGParamType) info[1]->Int32Value())));
}

NAN_METHOD(openvg::GetParameterVectorSize) {
  Nan::HandleScope scope;

  CheckArgs2(getParameterVectorSize, VGHandle, Int32, VGParamType, Int32);

  info.GetReturnValue().Set(Nan::New<Integer>(vgGetParameterVectorSize((VGHandle) info[0]->Int32Value(),
                                                          (VGParamType) info[1]->Int32Value())));
}

NAN_METHOD(openvg::GetParameterFV) {
  Nan::HandleScope scope;

  CheckArgs3(getParameterFV,
             VGHandle, Int32, VGParamType, Int32, Float32Array, Object);

  Nan::TypedArrayContents<VGfloat> values(info[2]);

  vgGetParameterfv((VGHandle) info[0]->Int32Value(),
                   (VGParamType) info[1]->Int32Value(),
                   values.length(),
                   (*values));

  
}

NAN_METHOD(openvg::GetParameterIV) {
  Nan::HandleScope scope;

  CheckArgs3(getParameterIV,
             VGHandle, Int32, VGParamType, Int32, Int32Array, Object);

  Nan::TypedArrayContents<VGint> values(info[2]);

  vgGetParameteriv((VGHandle) info[0]->Int32Value(),
                   (VGParamType) info[1]->Int32Value(),
                   values.length(),
                   (*values));

  
}

NAN_METHOD(openvg::GetParameterFVOL) {
  Nan::HandleScope scope;

  CheckArgs5(getParameterFV,
             VGHandle, Int32, VGParamType, Int32, Float32Array, Object,
             offset, Int32, length, Int32);

  Nan::TypedArrayContents<VGfloat> values(info[2]);

  vgGetParameterfv((VGHandle) info[0]->Int32Value(),
                   (VGParamType) info[1]->Int32Value(),
                   (VGint) info[4]->Int32Value(),
                   (*values) + info[3]->Int32Value());

  
}

NAN_METHOD(openvg::GetParameterIVOL) {
  Nan::HandleScope scope;

  CheckArgs5(getParameterIV,
             VGHandle, Int32, VGParamType, Int32, Int32Array, Object,
             offset, Int32, length, Int32);

  Nan::TypedArrayContents<VGint> values(info[2]);

  vgGetParameteriv((VGHandle) info[0]->Int32Value(),
                   (VGParamType) info[1]->Int32Value(),
                   (VGint) info[4]->Int32Value(),
                   (*values) + info[3]->Int32Value());

  
}


/* Matrix Manipulation */


NAN_METHOD(openvg::LoadIdentity) {
  Nan::HandleScope scope;

  CheckArgs0(loadIdentity);

  vgLoadIdentity();

  
}

NAN_METHOD(openvg::LoadMatrix) {
  Nan::HandleScope scope;

  CheckArgs1(loadIdentity, Float32Array, Object);

  Nan::TypedArrayContents<VGfloat> matrix(info[0]);

  vgLoadMatrix(*matrix);

  
}

NAN_METHOD(openvg::GetMatrix) {
  Nan::HandleScope scope;

  CheckArgs1(getMatrix, Float32Array, Object);

  Nan::TypedArrayContents<VGfloat> matrix(info[0]);

  vgGetMatrix(*matrix);

  
}

NAN_METHOD(openvg::MultMatrix) {
  Nan::HandleScope scope;

  CheckArgs1(multMatrix, Float32Array, Object);

  Nan::TypedArrayContents<VGfloat> matrix(info[0]);

  vgMultMatrix(*matrix);

  
}

NAN_METHOD(openvg::Translate) {
  Nan::HandleScope scope;

  CheckArgs2(translate, x, Number, y, Number);

  vgTranslate((VGfloat) info[0]->NumberValue(),
              (VGfloat) info[1]->NumberValue());

  
}

NAN_METHOD(openvg::Scale) {
  Nan::HandleScope scope;

  CheckArgs2(scale, x, Number, y, Number);

  vgScale((VGfloat) info[0]->NumberValue(),
          (VGfloat) info[1]->NumberValue());

  
}

NAN_METHOD(openvg::Shear) {
  Nan::HandleScope scope;

  CheckArgs2(shear, x, Number, y, Number);

  vgShear((VGfloat) info[0]->NumberValue(),
          (VGfloat) info[1]->NumberValue());

  
}

NAN_METHOD(openvg::Rotate) {
  Nan::HandleScope scope;

  CheckArgs1(shear, angle, Number);

  vgRotate((VGfloat) info[0]->NumberValue());

  
}


/* Masking and Clearing */


NAN_METHOD(openvg::Mask) {
  Nan::HandleScope scope;

  CheckArgs6(mask,
             VGHandle, Uint32, VGMaskOperation, Uint32,
             x, Int32, y, Int32, width, Int32, height, Int32);

  vgMask((VGHandle) info[0]->Uint32Value(),
         static_cast<VGMaskOperation>(info[1]->Uint32Value()),
         (VGint) info[2]->Int32Value(),
         (VGint) info[3]->Int32Value(),
         (VGint) info[4]->Int32Value(),
         (VGint) info[5]->Int32Value());

  
}

NAN_METHOD(openvg::RenderToMask) {
  Nan::HandleScope scope;

  CheckArgs3(renderToMask,
             VGPath, Uint32,
             VGbitfield, Uint32,
             VGMaskOperation, Uint32);

  vgRenderToMask((VGPath) info[0]->Uint32Value(),
                 (VGbitfield) info[1]->Uint32Value(),
                 (VGMaskOperation) info[2]->Uint32Value());

  
}

NAN_METHOD(openvg::CreateMaskLayer) {
  Nan::HandleScope scope;

  CheckArgs2(createMaskLayer, width, Int32, height, Int32);

  info.GetReturnValue().Set(Nan::New<Integer>(vgCreateMaskLayer((VGint) info[0]->Int32Value(),
                                                   (VGint) info[1]->Int32Value())));
}

NAN_METHOD(openvg::DestroyMaskLayer) {
  Nan::HandleScope scope;

  CheckArgs1(destroyMaskLayer, VGMaskLayer, Uint32);

  vgDestroyMaskLayer((VGMaskLayer) info[0]->Uint32Value());
}

NAN_METHOD(openvg::FillMaskLayer) {
  Nan::HandleScope scope;

  CheckArgs6(fillMaskLayer,
             VGMaskLayer, Uint32,
             x, Int32, y, Int32, width, Int32, height, Int32,
             value, Number);

  vgFillMaskLayer((VGMaskLayer) info[0]->Uint32Value(),
                  (VGint) info[1]->Int32Value(),
                  (VGint) info[2]->Int32Value(),
                  (VGint) info[3]->Int32Value(),
                  (VGint) info[4]->Int32Value(),
                  (VGfloat) info[5]->NumberValue());
}

NAN_METHOD(openvg::CopyMask) {
  Nan::HandleScope scope;

  CheckArgs7(fillMaskLayer,
             VGMaskLayer, Uint32,
             dx, Int32, dy, Int32, sx, Int32, sy, Int32,
             width, Int32, height, Int32);

  vgCopyMask((VGMaskLayer) info[0]->Uint32Value(),
             (VGint) info[1]->Int32Value(), (VGint) info[2]->Int32Value(),
             (VGint) info[3]->Int32Value(), (VGint) info[4]->Int32Value(),
             (VGint) info[5]->Int32Value(), (VGint) info[6]->Int32Value());
}

NAN_METHOD(openvg::Clear) {
  Nan::HandleScope scope;

  CheckArgs4(clear, x, Int32, y, Int32, width, Int32, height, Int32);

  vgClear((VGint) info[0]->Int32Value(), (VGint) info[1]->Int32Value(),
          (VGint) info[2]->Int32Value(), (VGint) info[3]->Int32Value());

  
}


/* Paths */


NAN_METHOD(openvg::CreatePath) {
  Nan::HandleScope scope;

  CheckArgs7(createPath,
             pathFormat, Int32, VGPathDatatype, Uint32,
             scale, Number, bias, Number, segmentCapacityHint, Int32,
             coordCapacityHint, Int32, capabilities, Uint32);

  info.GetReturnValue().Set(Nan::New<Uint32>(vgCreatePath((VGint) info[0]->Int32Value(),
                                             static_cast<VGPathDatatype>(info[1]->Uint32Value()),
                                             (VGfloat) info[2]->NumberValue(),
                                             (VGfloat) info[3]->NumberValue(),
                                             (VGint) info[4]->Int32Value(),
                                             (VGint) info[5]->Int32Value(),
                                             (VGbitfield) info[6]->Uint32Value())));
}

NAN_METHOD(openvg::ClearPath) {
  Nan::HandleScope scope;

  CheckArgs2(clearPath, VGPath, Number, capabilities, Uint32);

  vgClearPath((VGPath) info[0]->Uint32Value(),
              (VGbitfield) info[1]->Uint32Value());

  
}

NAN_METHOD(openvg::DestroyPath) {
  Nan::HandleScope scope;

  CheckArgs1(destroyPath, VGPath, Number);

  vgDestroyPath((VGPath) info[0]->Uint32Value());

  
}

NAN_METHOD(openvg::RemovePathCapabilities) {
  Nan::HandleScope scope;

  CheckArgs2(removePathCapabilities, VGPath, Number, capabilities, Uint32);

  vgRemovePathCapabilities((VGPath) info[0]->Uint32Value(),
                           (VGbitfield) info[1]->Uint32Value());

  
}

NAN_METHOD(openvg::GetPathCapabilities) {
  Nan::HandleScope scope;

  CheckArgs1(getPathCapabilities, VGPath, Number);

  info.GetReturnValue().Set(Nan::New<Uint32>(vgGetPathCapabilities((VGPath) info[0]->Int32Value())));
}

NAN_METHOD(openvg::AppendPath) {
  Nan::HandleScope scope;

  CheckArgs2(appendPath, dstPath, Number, srcPath, Number);

  vgAppendPath((VGPath) info[0]->Uint32Value(),
               (VGPath) info[1]->Uint32Value());

  
}

NAN_METHOD(openvg::AppendPathData) {
  Nan::HandleScope scope;

  CheckArgs4(appendPathData,
             dstPath, Number, numSegments, Int32, Uint8Array, Object,
             pathData, Object);

  Nan::TypedArrayContents<VGubyte> segments(info[2]);
  Nan::TypedArrayContents<void> data(info[3]);

  vgAppendPathData((VGPath) info[0]->Uint32Value(),
                   (VGint) info[1]->Int32Value(),
                   (*segments),
                   (*data));
}

NAN_METHOD(openvg::AppendPathDataO) {
  Nan::HandleScope scope;

  CheckArgs4(appendPathData,
             dstPath, Number, numSegments, Int32, Uint8Array, Object,
             pathData, Object);

  Nan::TypedArrayContents<VGubyte> segments(info[2]);
  Nan::TypedArrayContents<void> data(info[4]);

  vgAppendPathData((VGPath) info[0]->Uint32Value(),
                   (VGint) info[1]->Int32Value(),
                   (*segments) + info[3]->Uint32Value(),
                   (*data) + info[5]->Int32Value());
}

NAN_METHOD(openvg::ModifyPathCoords) {
  Nan::HandleScope scope;

  CheckArgs4(modifyPathCoords,
             VGPath, Number, startIndex, Int32, numSegments, Int32,
             pathData, Object);

  Nan::TypedArrayContents<void> data(info[3]);

  vgModifyPathCoords((VGPath) info[0]->Uint32Value(),
                     (VGint) info[1]->Int32Value(),
                     (VGint) info[2]->Int32Value(),
                     (*data));
}

NAN_METHOD(openvg::TransformPath) {
  Nan::HandleScope scope;

  CheckArgs2(transformPath, dstPath, Number, srcPath, Number);

  vgTransformPath((VGPath) info[0]->Uint32Value(),
                  (VGPath) info[1]->Uint32Value());
}

NAN_METHOD(openvg::InterpolatePath) {
  Nan::HandleScope scope;

  CheckArgs4(interpolatePath,
             dstPath, Number, startPath, Number, endPath, Number,
             amount, Number);

  info.GetReturnValue().Set(Nan::New<Boolean>(vgInterpolatePath((VGPath) info[0]->Uint32Value(),
                                                   (VGPath) info[1]->Uint32Value(),
                                                   (VGPath) info[2]->Uint32Value(),
                                                   (VGfloat) info[3]->NumberValue())));
}

NAN_METHOD(openvg::PathLength) {
  Nan::HandleScope scope;

  CheckArgs3(pathLength, path, Number,
             startSegment, Int32, numSegments, Int32);

  info.GetReturnValue().Set(Nan::New<Number>(vgPathLength((VGPath) info[0]->Uint32Value(),
                                             (VGint) info[1]->Int32Value(),
                                             (VGint) info[2]->Int32Value())));
}

NAN_METHOD(openvg::PointAlongPath) {
  Nan::HandleScope scope;

  CheckArgs5(pointAlongPath, path, Number,
             startSegment, Int32, numSegments, Int32,
             distance, Number, point, Object);

  VGfloat x, y, tx, ty;

  vgPointAlongPath((VGPath) info[0]->Uint32Value(),
                   (VGint) info[1]->Int32Value(),
                   (VGint) info[2]->Int32Value(),
                   (VGfloat) info[3]->NumberValue(),
                   &x, &y, &tx, &ty);

  Local<Object> point = info[4].As<Object>();
  point->Set(Nan::New<String>("x").ToLocalChecked(), Nan::New<Number>(x));
  point->Set(Nan::New<String>("y").ToLocalChecked(), Nan::New<Number>(y));
  point->Set(Nan::New<String>("tx").ToLocalChecked(), Nan::New<Number>(tx));
  point->Set(Nan::New<String>("ty").ToLocalChecked(), Nan::New<Number>(ty));
}

NAN_METHOD(openvg::PathBounds) {
  Nan::HandleScope scope;

  CheckArgs2(pathBounds, VGPath, Number, bounds, Object);

  VGfloat minX, minY, width, height;

  vgPathBounds((VGPath) info[0]->Uint32Value(),
               &minX, &minY, &width, &height);

  Local<Object> bounds = info[1].As<Object>();
  bounds->Set(Nan::New<String>("x").ToLocalChecked(), Nan::New<Number>(minX));
  bounds->Set(Nan::New<String>("y").ToLocalChecked(), Nan::New<Number>(minY));
  bounds->Set(Nan::New<String>("w").ToLocalChecked(), Nan::New<Number>(width));
  bounds->Set(Nan::New<String>("h").ToLocalChecked(), Nan::New<Number>(height));
}

NAN_METHOD(openvg::PathTransformedBounds) {
  Nan::HandleScope scope;

  CheckArgs2(pathTransformedBounds, VGPath, Number, bounds, Object);

  VGfloat minX, minY, width, height;

  vgPathTransformedBounds((VGPath) info[0]->Uint32Value(),
                          &minX, &minY, &width, &height);

  Local<Object> bounds = info[1].As<Object>();
  bounds->Set(Nan::New<String>("x").ToLocalChecked(), Nan::New<Number>(minX));
  bounds->Set(Nan::New<String>("y").ToLocalChecked(), Nan::New<Number>(minY));
  bounds->Set(Nan::New<String>("w").ToLocalChecked(), Nan::New<Number>(width));
  bounds->Set(Nan::New<String>("h").ToLocalChecked(), Nan::New<Number>(height));
}

NAN_METHOD(openvg::DrawPath) {
  Nan::HandleScope scope;

  CheckArgs2(drawPath, VGPath, Number, paintModes, Number);

  vgDrawPath((VGPath) info[0]->Uint32Value(),
             (VGbitfield) info[1]->Uint32Value());
}


/* Paint */


NAN_METHOD(openvg::CreatePaint) {
  Nan::HandleScope scope;

  CheckArgs0(createPaint);

  info.GetReturnValue().Set(Nan::New<Uint32>(vgCreatePaint()));
}

NAN_METHOD(openvg::DestroyPaint) {
  Nan::HandleScope scope;

  CheckArgs1(destroyPaint, VGPaint, Number);

  vgDestroyPaint((VGPaint) info[0]->Uint32Value());

  
}

NAN_METHOD(openvg::SetPaint) {
  Nan::HandleScope scope;

  CheckArgs2(setPaint, VGPaint, Number, paintModes, Number);

  vgSetPaint((VGPaint) info[0]->Uint32Value(),
             (VGbitfield) info[1]->Uint32Value());
}

NAN_METHOD(openvg::GetPaint) {
  Nan::HandleScope scope;

  CheckArgs1(getPaint, VGPaint, Uint32);

  info.GetReturnValue().Set(Nan::New<Uint32>(vgGetPaint(static_cast<VGPaintMode>(info[0]->Uint32Value()))));
}

NAN_METHOD(openvg::SetColor) {
  Nan::HandleScope scope;

  CheckArgs2(setColor, VGPaint, Uint32, rgba, Uint32);

  vgSetColor((VGPaint) info[0]->Uint32Value(),
             (VGuint) info[1]->Uint32Value());
}

NAN_METHOD(openvg::GetColor) {
  Nan::HandleScope scope;

  CheckArgs1(getColor, VGPaint, Uint32);

  info.GetReturnValue().Set(Nan::New<Uint32>(vgGetColor((VGPaint) info[0]->Uint32Value())));
}

NAN_METHOD(openvg::PaintPattern) {
  Nan::HandleScope scope;

  CheckArgs2(paintPattern, VGPaint, Uint32, VGImage, Uint32);

  vgPaintPattern((VGPaint) info[0]->Uint32Value(),
                 (VGImage) info[1]->Uint32Value());
}


/* Images */


NAN_METHOD(openvg::CreateImage) {
  Nan::HandleScope scope;

  CheckArgs4(createImage,
             VGImageFormat, Uint32, width, Int32, height, Int32,
             allowedQuality, Uint32);

  info.GetReturnValue().Set(Nan::New<Uint32>(vgCreateImage(static_cast<VGImageFormat>(info[0]->Uint32Value()),
                                              (VGint) info[1]->Int32Value(),
                                              (VGint) info[2]->Int32Value(),
                                              (VGuint) info[3]->Uint32Value())));
}

NAN_METHOD(openvg::DestroyImage) {
  Nan::HandleScope scope;

  CheckArgs1(destroyImage, VGImage, Number);

  vgDestroyImage((VGImage) (VGPaint) info[0]->Uint32Value());
}

NAN_METHOD(openvg::ClearImage) {
  Nan::HandleScope scope;

  CheckArgs5(clearImage,
             VGImage, Number, x, Int32, y, Int32, width, Int32, height, Int32);

  vgClearImage((VGImage) info[0]->Uint32Value(),
               (VGint) info[1]->Int32Value(),
               (VGint) info[2]->Int32Value(),
               (VGint) info[3]->Int32Value(),
               (VGint) info[4]->Int32Value());
}

NAN_METHOD(openvg::ImageSubData) {
  Nan::HandleScope scope;

  CheckArgs8(imageSubData,
             VGImage, Number, data, Object, dataStride, Int32,
             dataFormat, Uint32,
             x, Int32, y, Int32, width, Int32, height, Int32);

  vgImageSubData((VGImage) info[0]->Uint32Value(),
                 node::Buffer::Data(info[1]),
                 (VGint) info[2]->Int32Value(),
                 static_cast<VGImageFormat>(info[3]->Uint32Value()),
                 (VGint) info[4]->Int32Value(),
                 (VGint) info[5]->Int32Value(),
                 (VGint) info[6]->Int32Value(),
                 (VGint) info[7]->Int32Value());
}

NAN_METHOD(openvg::GetImageSubData) {
  Nan::HandleScope scope;

  CheckArgs8(getImageSubData,
             VGImage, Number, data, Object, dataStride, Int32,
             dataFormat, Uint32,
             x, Int32, y, Int32, width, Int32, height, Int32);

  Nan::TypedArrayContents<void> data(info[1]);

  vgGetImageSubData((VGImage) info[0]->Uint32Value(),
                    (*data),
                    (VGint) info[2]->Int32Value(),
                    static_cast<VGImageFormat>(info[3]->Uint32Value()),
                    (VGint) info[4]->Int32Value(),
                    (VGint) info[5]->Int32Value(),
                    (VGint) info[6]->Int32Value(),
                    (VGint) info[7]->Int32Value());
}

NAN_METHOD(openvg::ChildImage) {
  Nan::HandleScope scope;

  CheckArgs5(childImage,
             VGImage, Number, x, Int32, y, Int32, width, Int32, height, Int32);

  info.GetReturnValue().Set(Nan::New<Uint32>(vgChildImage((VGImage) info[0]->Uint32Value(),
                                     (VGint) info[1]->Int32Value(),
                                     (VGint) info[2]->Int32Value(),
                                     (VGint) info[3]->Int32Value(),
                                     (VGint) info[4]->Int32Value())));
}

NAN_METHOD(openvg::GetParent) {
  Nan::HandleScope scope;

  CheckArgs1(getParent, VGImage, Number);

  info.GetReturnValue().Set(Nan::New<Uint32>(vgGetParent((VGImage) info[0]->Uint32Value())));
}

NAN_METHOD(openvg::CopyImage) {
  Nan::HandleScope scope;

  CheckArgs9(copyImage,
             dstImage, Number, dx, Int32, dy, Int32,
             srcImage, Number, sx, Int32, sy, Int32,
             width, Int32, height, Int32, dither, Boolean);

  vgCopyImage((VGImage) info[0]->Uint32Value(),
              (VGint) info[1]->Int32Value(),
              (VGint) info[2]->Int32Value(),
              (VGImage) info[3]->Uint32Value(),
              (VGint) info[4]->Int32Value(),
              (VGint) info[5]->Int32Value(),
              (VGint) info[6]->Int32Value(),
              (VGint) info[7]->Int32Value(),
              (VGboolean) info[8]->BooleanValue());
}

NAN_METHOD(openvg::DrawImage) {
  Nan::HandleScope scope;

  CheckArgs1(drawImage, VGImage, Number);

  vgDrawImage((VGImage) info[0]->Uint32Value());
}

NAN_METHOD(openvg::SetPixels) {
  Nan::HandleScope scope;

  CheckArgs7(setPixels,
             dx, Int32, dy, Int32,
             srcImage, Number, dx, Int32, dy, Int32,
             width, Int32, height, Int32);

  vgSetPixels((VGint) info[0]->Int32Value(),
              (VGint) info[1]->Int32Value(),
              (VGImage) info[2]->Uint32Value(),
              (VGint) info[3]->Int32Value(),
              (VGint) info[4]->Int32Value(),
              (VGint) info[5]->Int32Value(),
              (VGint) info[6]->Int32Value());
}

NAN_METHOD(openvg::WritePixels) {
  Nan::HandleScope scope;

  CheckArgs7(writePixels, data, Object, dataStride, Int32,
             dataFormat, Uint32,
             dx, Int32, dy, Int32, width, Int32, height, Int32);

  Nan::TypedArrayContents<void> data(info[0]);

  vgWritePixels((*data),
                (VGint) info[1]->Int32Value(),
                static_cast<VGImageFormat>(info[2]->Uint32Value()),
                (VGint) info[3]->Int32Value(),
                (VGint) info[4]->Int32Value(),
                (VGint) info[5]->Int32Value(),
                (VGint) info[6]->Int32Value());
}

NAN_METHOD(openvg::GetPixels) {
  Nan::HandleScope scope;

  CheckArgs7(getPixels,
             VGImage, Number,
             dx, Int32, dy, Int32,
             sx, Int32, sy, Int32,
             width, Int32, height, Int32);

  vgGetPixels((VGImage) info[0]->Uint32Value(),
              (VGint) info[1]->Int32Value(),
              (VGint) info[2]->Int32Value(),
              (VGint) info[3]->Int32Value(),
              (VGint) info[4]->Int32Value(),
              (VGint) info[5]->Int32Value(),
              (VGint) info[6]->Int32Value());
}

NAN_METHOD(openvg::ReadPixels) {
  Nan::HandleScope scope;

  CheckArgs7(readPixels,
             data, Object, dataStride, Int32, dataFormat, Uint32,
             sx, Int32, sy, Int32, width, Int32, height, Int32);

  Nan::TypedArrayContents<void> data(info[0]);

  vgReadPixels((*data),
               (VGint) info[1]->Int32Value(),
               static_cast<VGImageFormat>(info[2]->Uint32Value()),
               (VGint) info[3]->Int32Value(),
               (VGint) info[4]->Int32Value(),
               (VGint) info[5]->Int32Value(),
               (VGint) info[6]->Int32Value());
}

NAN_METHOD(openvg::CopyPixels) {
  Nan::HandleScope scope;

  CheckArgs6(copyPixels,
             dx, Int32, dy, Int32, sx, Int32, sy, Int32,
             width, Int32, height, Int32);

  vgCopyPixels((VGint) info[0]->Int32Value(),
               (VGint) info[1]->Int32Value(),
               (VGint) info[2]->Int32Value(),
               (VGint) info[3]->Int32Value(),
               (VGint) info[4]->Int32Value(),
               (VGint) info[5]->Int32Value());
}


/* Text */


NAN_METHOD(openvg::CreateFont) {
  Nan::HandleScope scope;

  CheckArgs1(createFont, glyphCapacityHint, Int32);

  info.GetReturnValue().Set(Nan::New<Uint32>(vgCreateFont((VGint) info[0]->Int32Value())));
}

NAN_METHOD(openvg::DestroyFont) {
  Nan::HandleScope scope;

  CheckArgs1(destroyFont, VGFont, Number);

  vgDestroyFont((VGFont) info[0]->Uint32Value());
}

NAN_METHOD(openvg::SetGlyphToPath) {
  Nan::HandleScope scope;

  CheckArgs6(setGlyphToPath, VGFont, Number, glyphIndex, Number,
             VGPath, Number, isHinted, Boolean,
             glyphOrigin, Object, escapement, Object);

  Nan::TypedArrayContents<VGfloat> glyphOrigin(info[4]);
  Nan::TypedArrayContents<VGfloat> escapement(info[5]);

  vgSetGlyphToPath((VGFont) info[0]->Uint32Value(),
                   (VGuint) info[1]->Uint32Value(),
                   (VGPath) info[2]->Uint32Value(),
                   (VGboolean) info[3]->BooleanValue(),
                   (*glyphOrigin),
                   (*escapement));
}

NAN_METHOD(openvg::SetGlyphToImage) {
  Nan::HandleScope scope;

  CheckArgs5(setGlyphToImage, VGFont, Number, glyphIndex, Number,
             VGImage, Number,
             glyphOrigin, Object, escapement, Object);

  Nan::TypedArrayContents<VGfloat> glyphOrigin(info[3]);
  Nan::TypedArrayContents<VGfloat> escapement(info[4]);

  vgSetGlyphToImage((VGFont) info[0]->Uint32Value(),
                    (VGuint) info[1]->Uint32Value(),
                    (VGImage) info[2]->Uint32Value(),
                    (*glyphOrigin),
                    (*escapement));
}

NAN_METHOD(openvg::ClearGlyph) {
  Nan::HandleScope scope;

  CheckArgs2(clearGlyph, VGFont, Number, glyphIndex, Uint32);

  vgClearGlyph((VGFont) info[0]->Uint32Value(),
               (VGuint) info[1]->Uint32Value());
}

NAN_METHOD(openvg::DrawGlyph) {
  Nan::HandleScope scope;

  CheckArgs4(drawGlyph, VGFont, Number, glyphIndex, Uint32,
             paintModes, Uint32, allowAutoHinting, Boolean);

  vgDrawGlyph((VGFont) info[0]->Uint32Value(),
              (VGuint) info[1]->Uint32Value(),
              (VGbitfield) info[2]->Uint32Value(),
              (VGboolean) info[3]->BooleanValue());
}

NAN_METHOD(openvg::DrawGlyphs) {
  Nan::HandleScope scope;

  CheckArgs7(drawGlyphs, VGFont, Number, glyphCount, Int32,
             glyphIndices, Object, adjustments_x, Object, adjustments_y, Object,
             paintModes, Uint32, allowAutoHinting, Boolean);

  Nan::TypedArrayContents<VGuint> glyphIndices(info[2]);
  Nan::TypedArrayContents<VGfloat> adjustments_x(info[3]);
  Nan::TypedArrayContents<VGfloat> adjustments_y(info[4]);

  vgDrawGlyphs((VGFont) info[0]->Uint32Value(),
               (VGuint) info[1]->Uint32Value(),
               (*glyphIndices),
               (*adjustments_x),
               (*adjustments_y),
               (VGbitfield) info[5]->Uint32Value(),
               (VGboolean) info[6]->BooleanValue());
}


/* Image Filters */


NAN_METHOD(openvg::ColorMatrix) {
  Nan::HandleScope scope;

  CheckArgs3(colorMatrix,
             dstVGImage, Number, srcVGImage, Number, matrix, Object);

  Nan::TypedArrayContents<VGfloat> matrix(info[2]);

  vgColorMatrix((VGImage) info[0]->Uint32Value(),
                (VGImage) info[1]->Uint32Value(),
                (*matrix));

  
}

NAN_METHOD(openvg::Convolve) {
  Nan::HandleScope scope;

  CheckArgs10(convolve, dstVGImage, Number, srcVGImage, Number,
              kernelWidth, Int32, kernelHeight, Int32,
              shiftX, Int32, shiftY, Int32,
              kernel, Object, scale, Number, bias, Number,
              tilingMode, Uint32);

  Nan::TypedArrayContents<VGshort> kernel(info[6]);

  vgConvolve((VGImage) info[0]->Uint32Value(),
             (VGImage) info[1]->Uint32Value(),
             (VGint) info[2]->Int32Value(),
             (VGint) info[3]->Int32Value(),
             (VGint) info[4]->Int32Value(),
             (VGint) info[5]->Int32Value(),
             (*kernel),
             (VGfloat) info[7]->NumberValue(),
             (VGfloat) info[8]->NumberValue(),
             static_cast<VGTilingMode>(info[9]->Uint32Value()));
}

NAN_METHOD(openvg::SeparableConvolve) {
  Nan::HandleScope scope;

  CheckArgs11(separableConvolve, dstVGImage, Number, srcVGImage, Number,
              kernelWidth, Int32, kernelHeight, Int32,
              shiftX, Int32, shiftY, Int32,
              kernelX, Object, kernelY, Object,
              scale, Number, bias, Number,
              tilingMode, Uint32);

  Nan::TypedArrayContents<VGshort> kernelX(info[6]);
  Nan::TypedArrayContents<VGshort> kernelY(info[7]);

  vgSeparableConvolve((VGImage) info[0]->Uint32Value(),
                      (VGImage) info[1]->Uint32Value(),
                      (VGint) info[2]->Int32Value(),
                      (VGint) info[3]->Int32Value(),
                      (VGint) info[4]->Int32Value(),
                      (VGint) info[5]->Int32Value(),
                      (*kernelX),
                      (*kernelY),
                      (VGfloat) info[8]->NumberValue(),
                      (VGfloat) info[9]->NumberValue(),
                      static_cast<VGTilingMode>(info[10]->Uint32Value()));
}

NAN_METHOD(openvg::GaussianBlur) {
  Nan::HandleScope scope;

  CheckArgs5(gaussianBlur, dstVGImage, Number, srcVGImage, Number,
             stdDeviationX, Number, stdDeviationY, Number,
             tilingMode, Uint32);

  vgGaussianBlur((VGImage) info[0]->Uint32Value(),
                 (VGImage) info[1]->Uint32Value(),
                 (VGfloat) info[2]->NumberValue(),
                 (VGfloat) info[3]->NumberValue(),
                 static_cast<VGTilingMode>(info[4]->Uint32Value()));
}

NAN_METHOD(openvg::Lookup) {
  Nan::HandleScope scope;

  CheckArgs9(lookup, VGImage, Number, dstVGImage, Number, srcVGImage, Number,
             redLUT, Object, greenLUT, Object, blueLUT, Object,
             alphaLUT, Object,
             outputLinear, Boolean, outputPremultiplied, Boolean);

  Nan::TypedArrayContents<VGubyte> redLUT(info[2]);
  Nan::TypedArrayContents<VGubyte> greenLUT(info[3]);
  Nan::TypedArrayContents<VGubyte> blueLUT(info[4]);
  Nan::TypedArrayContents<VGubyte> alphaLUT(info[5]);

  vgLookup((VGImage) info[0]->Uint32Value(),
           (VGImage) info[1]->Uint32Value(),
           (*redLUT),
           (*greenLUT),
           (*blueLUT),
           (*alphaLUT),
           (VGboolean) info[6]->BooleanValue(),
           (VGboolean) info[7]->BooleanValue());
}

NAN_METHOD(openvg::LookupSingle) {
  Nan::HandleScope scope;

  CheckArgs6(lookupSingle, dstVGImage, Number, srcVGImage, Number,
             lookupTable, Object, sourceChannel, Uint32,
             outputLinear, Boolean, outputPremultiplied, Boolean);

  Nan::TypedArrayContents<VGuint> lookupTable(info[2]);

  vgLookupSingle((VGImage) info[0]->Uint32Value(),
                 (VGImage) info[1]->Uint32Value(),
                 (*lookupTable),
                 static_cast<VGImageChannel>(info[3]->Uint32Value()),
                 (VGboolean) info[4]->BooleanValue(),
                 (VGboolean) info[5]->BooleanValue());
}


/* Hardware Queries */
NAN_METHOD(openvg::HardwareQuery) {
  Nan::HandleScope scope;

  CheckArgs2(hardwareQuery, key, Uint32, setting, Int32);

  info.GetReturnValue().Set(Nan::New<Uint32>(vgHardwareQuery(static_cast<VGHardwareQueryType>(info[0]->Uint32Value()),
                                                (VGint) info[1]->Int32Value())));
}


/* Renderer and Extension Information */
VG_API_CALL const VGubyte * VG_API_ENTRY vgGetString(VGStringID name) VG_API_EXIT;

NAN_METHOD(openvg::GetString) {
  Nan::HandleScope scope;

  CheckArgs1(getString, key, Uint32);

  info.GetReturnValue().Set(Nan::New<String>((char*) vgGetString(static_cast<VGStringID>(info[0]->Uint32Value()))).ToLocalChecked());
}


/* Utilities */


NAN_METHOD(openvg::vgu::Line) {
  Nan::HandleScope scope;

  CheckArgs5(line,
             VGPath, Number, x0, Number, y0, Number, x1, Number, y1, Number);

  info.GetReturnValue().Set(Nan::New<Uint32>(vguLine((VGPath) info[0]->Uint32Value(),
                                        (VGfloat) info[1]->NumberValue(),
                                        (VGfloat) info[2]->NumberValue(),
                                        (VGfloat) info[3]->NumberValue(),
                                        (VGfloat) info[4]->NumberValue())));
}

NAN_METHOD(openvg::vgu::Polygon) {
  Nan::HandleScope scope;

  CheckArgs4(polygon,
             VGPath, Number, Float32Array, Object, count, Int32,
             closed, Boolean);

  Nan::TypedArrayContents<VGfloat> points(info[1]);

  info.GetReturnValue().Set(Nan::New<Uint32>(vguPolygon((VGPath) info[0]->Uint32Value(),
                                           (*points),
                                           (VGint) info[2]->Int32Value(),
                                           (VGboolean) info[3]->BooleanValue())));
}

NAN_METHOD(openvg::vgu::Rect) {
  Nan::HandleScope scope;

  CheckArgs5(rect, VGPath, Number, x, Number, y, Number,
             width, Number, height, Number);

  info.GetReturnValue().Set(Nan::New<Uint32>(vguRect((VGPath) info[0]->Uint32Value(),
                                        (VGfloat) info[1]->NumberValue(),
                                        (VGfloat) info[2]->NumberValue(),
                                        (VGfloat) info[3]->NumberValue(),
                                        (VGfloat) info[4]->NumberValue())));
}

NAN_METHOD(openvg::vgu::RoundRect) {
  Nan::HandleScope scope;

  CheckArgs7(rect, VGPath,
             Number, x, Number, y, Number, width, Number, height,
             Number, arcWidth, Number, arcHeight, Number);

  info.GetReturnValue().Set(Nan::New<Uint32>(vguRoundRect((VGPath) info[0]->Uint32Value(),
                                             (VGfloat) info[1]->NumberValue(),
                                             (VGfloat) info[2]->NumberValue(),
                                             (VGfloat) info[3]->NumberValue(),
                                             (VGfloat) info[4]->NumberValue(),
                                             (VGfloat) info[5]->NumberValue(),
                                             (VGfloat) info[6]->NumberValue())));
}

NAN_METHOD(openvg::vgu::Ellipse) {
  Nan::HandleScope scope;

  CheckArgs5(ellipse, VGPath, Number, x, Number, y, Number,
             width, Number, height, Number);

  info.GetReturnValue().Set(Nan::New<Uint32>(vguEllipse((VGPath) info[0]->Uint32Value(),
                                           (VGfloat) info[1]->NumberValue(),
                                           (VGfloat) info[2]->NumberValue(),
                                           (VGfloat) info[3]->NumberValue(),
                                           (VGfloat) info[4]->NumberValue())));
}

NAN_METHOD(openvg::vgu::Arc) {
  Nan::HandleScope scope;

  CheckArgs8(arc,
             VGPath, Number, x, Number, y, Number,
             width, Number, height, Number,
             startAngle, Number, angleExtent, Number, VGUArcType, Uint32);

  info.GetReturnValue().Set(Nan::New<Uint32>(vguArc((VGPath) info[0]->Uint32Value(),
                                       (VGfloat) info[1]->NumberValue(),
                                       (VGfloat) info[2]->NumberValue(),
                                       (VGfloat) info[3]->NumberValue(),
                                       (VGfloat) info[4]->NumberValue(),
                                       (VGfloat) info[5]->NumberValue(),
                                       (VGfloat) info[6]->NumberValue(),
                                       static_cast<VGUArcType>(info[7]->Uint32Value()))));
}

NAN_METHOD(openvg::vgu::ComputeWarpQuadToSquare) {
  Nan::HandleScope scope;

  CheckArgs9(computeWarpQuadToSquare,
             sx0, Number, sy0, Number, sx1, Number, sy1, Number,
             sx2, Number, sy2, Number, sx3, Number, sy3, Number,
             Float32Array, Object);

  Nan::TypedArrayContents<VGfloat> matrix(info[8]);

  info.GetReturnValue().Set(Nan::New<Uint32>(vguComputeWarpQuadToSquare((VGfloat) info[0]->NumberValue(),
                                                           (VGfloat) info[1]->NumberValue(),
                                                           (VGfloat) info[2]->NumberValue(),
                                                           (VGfloat) info[3]->NumberValue(),
                                                           (VGfloat) info[4]->NumberValue(),
                                                           (VGfloat) info[5]->NumberValue(),
                                                           (VGfloat) info[6]->NumberValue(),
                                                           (VGfloat) info[7]->NumberValue(),
                                                           (*matrix))));
}

NAN_METHOD(openvg::vgu::ComputeWarpSquareToQuad) {
  Nan::HandleScope scope;

  CheckArgs9(computeWarpSquareToQuad,
             sx0, Number, sy0, Number, sx1, Number, sy1, Number,
             sx2, Number, sy2, Number, sx3, Number, sy3, Number,
             Float32Array, Object);

  Nan::TypedArrayContents<VGfloat> matrix(info[8]);

  info.GetReturnValue().Set(Nan::New<Uint32>(vguComputeWarpSquareToQuad((VGfloat) info[0]->NumberValue(),
                                                           (VGfloat) info[1]->NumberValue(),
                                                           (VGfloat) info[2]->NumberValue(),
                                                           (VGfloat) info[3]->NumberValue(),
                                                           (VGfloat) info[4]->NumberValue(),
                                                           (VGfloat) info[5]->NumberValue(),
                                                           (VGfloat) info[6]->NumberValue(),
                                                           (VGfloat) info[7]->NumberValue(),
                                                           (*matrix))));
}

NAN_METHOD(openvg::vgu::ComputeWarpQuadToQuad) {
  Nan::HandleScope scope;

  // No arg check -> Would be a 17 arg macro

  Nan::TypedArrayContents<VGfloat> matrix(info[16]);

  info.GetReturnValue().Set(Nan::New<Uint32>(vguComputeWarpQuadToQuad((VGfloat) info[ 0]->NumberValue(),
                                                         (VGfloat) info[ 1]->NumberValue(),
                                                         (VGfloat) info[ 2]->NumberValue(),
                                                         (VGfloat) info[ 3]->NumberValue(),
                                                         (VGfloat) info[ 4]->NumberValue(),
                                                         (VGfloat) info[ 5]->NumberValue(),
                                                         (VGfloat) info[ 6]->NumberValue(),
                                                         (VGfloat) info[ 7]->NumberValue(),
                                                         (VGfloat) info[ 8]->NumberValue(),
                                                         (VGfloat) info[ 9]->NumberValue(),
                                                         (VGfloat) info[10]->NumberValue(),
                                                         (VGfloat) info[11]->NumberValue(),
                                                         (VGfloat) info[12]->NumberValue(),
                                                         (VGfloat) info[13]->NumberValue(),
                                                         (VGfloat) info[14]->NumberValue(),
                                                         (VGfloat) info[15]->NumberValue(),
                                                         (*matrix))));
}


/* KHR extensions */


NAN_METHOD(openvg::ext::CreateEGLImageTargetKHR) {
  Nan::HandleScope scope;

  CheckArgs1(createEGLImageTargetKHR, VGeglImageKHR, Object);

#ifdef VG_VGEXT_PROTOTYPES
// TODO FIX THIS
//  VGeglImageKHR *image = Nan::ObjectWrap::Unwrap<VGeglImageKHR>(info[0]->ToObject());
//  info.GetReturnValue().Set(Nan::New<Uint32>(vgCreateEGLImageTargetKHR(image)));
    printf("openvg.cc:%d: CreateEGLImageTargetKHR needs fixing\n", __LINE__);
#else
  
#endif
}

NAN_METHOD(openvg::ext::IterativeAverageBlurKHR) {
  Nan::HandleScope scope;

  CheckArgs6(iterativeAverageBlurKHR,
             dstVGImage, Number, srcVGImage, Number,
             dimX, Number, dimY, Number, iterative, Number,
             tilingMode, Object);

#ifdef VG_VGEXT_PROTOTYPES
  vgIterativeAverageBlurKHR((VGImage) info[0]->Uint32Value(),
                            (VGImage) info[1]->Uint32Value(),
                            (VGfloat) info[2]->NumberValue(),
                            (VGfloat) info[3]->NumberValue(),
                            (VGImage) info[4]->Uint32Value(),
                            static_cast<VGTilingMode>((VGImage) info[5]->Uint32Value()));
#endif
}

NAN_METHOD(openvg::ext::ParametricFilterKHR) {
  Nan::HandleScope scope;

  CheckArgs9(iterativeAverageBlurKHR,
             dstVGImage, Number, srcVGImage, Number, blurVGImage, Number,
             strength, Number, offsetX, Number, offsetY, Number,
             filterFlags, Number, highlightPaint, Number, shadowPaint, Number);

#ifdef VG_VGEXT_PROTOTYPES
  vgParametricFilterKHR((VGImage) info[0]->Uint32Value(),
                        (VGImage) info[1]->Uint32Value(),
                        (VGImage) info[2]->Uint32Value(),
                        (VGfloat) info[3]->NumberValue(),
                        (VGfloat) info[4]->NumberValue(),
                        (VGfloat) info[5]->NumberValue(),
                        (VGbitfield) info[6]->Uint32Value(),
                        (VGPaint) info[7]->Uint32Value(),
                        (VGPaint) info[8]->Uint32Value());
#endif
}

NAN_METHOD(openvg::ext::DropShadowKHR) {
  Nan::HandleScope scope;

  CheckArgs11(dropShadowKHR,
              dstVGImage, Number, srcVGImage, Number,
              dimX, Number, dimY, Number, iterative, Number,
              strength, Number, distance, Number, angle, Number,
              filterFlags, Number, allowedQuality, Number,
              shadowColorRGBA, Number);

#ifdef VG_VGEXT_PROTOTYPES
  info.GetReturnValue().Set(Nan::New<Uint32>(vguDropShadowKHR((VGImage) info[0]->Uint32Value(),
                                                 (VGImage) info[1]->Uint32Value(),
                                                 (VGfloat) info[2]->NumberValue(),
                                                 (VGfloat) info[3]->NumberValue(),
                                                 (VGuint) info[4]->Uint32Value(),
                                                 (VGfloat) info[5]->NumberValue(),
                                                 (VGfloat) info[6]->NumberValue(),
                                                 (VGfloat) info[7]->NumberValue(),
                                                 (VGbitfield) info[8]->Uint32Value(),
                                                 (VGbitfield) info[9]->Uint32Value(),
                                                 (VGuint) info[10]->Uint32Value())));
#else
  
#endif
}

NAN_METHOD(openvg::ext::GlowKHR) {
  Nan::HandleScope scope;

  CheckArgs9(glowKHR,
             dstVGImage, Number, srcVGImage, Number,
             dimX, Number, dimY, Number, iterative, Number,
             strength, Number,
             filterFlags, Number, allowedQuality, Number,
             glowColorRGBA, Number);

#ifdef VG_VGEXT_PROTOTYPES
  info.GetReturnValue().Set(Nan::New<Uint32>(vguGlowKHR((VGImage) info[0]->Uint32Value(),
                                           (VGImage) info[1]->Uint32Value(),
                                           (VGfloat) info[2]->NumberValue(),
                                           (VGfloat) info[3]->NumberValue(),
                                           (VGuint) info[4]->Uint32Value(),
                                           (VGfloat) info[5]->NumberValue(),
                                           (VGbitfield) info[6]->Uint32Value(),
                                           (VGbitfield) info[7]->Uint32Value(),
                                           (VGuint) info[8]->Uint32Value())));
#else
  
#endif
}

NAN_METHOD(openvg::ext::BevelKHR) {
  Nan::HandleScope scope;

  CheckArgs12(bevelKHR,
              dstVGImage, Number, srcVGImage, Number,
              dimX, Number, dimY, Number, iterative, Number,
              strength, Number, distance, Number, angle, Number,
              filterFlags, Number, allowedQuality, Number,
              highlightColorRGBA, Number, shadowColorRGBA, Number);

#ifdef VG_VGEXT_PROTOTYPES
  info.GetReturnValue().Set(Nan::New<Uint32>(vguBevelKHR((VGImage) info[0]->Uint32Value(),
                                            (VGImage) info[1]->Uint32Value(),
                                            (VGfloat) info[2]->NumberValue(),
                                            (VGfloat) info[3]->NumberValue(),
                                            (VGuint) info[4]->Uint32Value(),
                                            (VGfloat) info[5]->NumberValue(),
                                            (VGfloat) info[6]->NumberValue(),
                                            (VGfloat) info[7]->NumberValue(),
                                            (VGbitfield) info[8]->Uint32Value(),
                                            (VGbitfield) info[9]->Uint32Value(),
                                            (VGuint) info[10]->Uint32Value(),
                                            (VGuint) info[11]->Uint32Value())));
#else
  
#endif
}

NAN_METHOD(openvg::ext::GradientGlowKHR) {
  Nan::HandleScope scope;

  CheckArgs12(gradientGlowKHR,
              dstVGImage, Number, srcVGImage, Number,
              dimX, Number, dimY, Number, iterative, Number,
              strength, Number, distance, Number, angle, Number,
              filterFlags, Number, allowedQuality, Number,
              stopsCount, Number, Float32Array, Object);

#ifdef VG_VGEXT_PROTOTYPES
  Nan::TypedArrayContents<VGfloat> glowColorRampStops(info[11]);

  info.GetReturnValue().Set(Nan::New<Uint32>(vguGradientGlowKHR((VGImage) info[0]->Uint32Value(),
                                                   (VGImage) info[1]->Uint32Value(),
                                                   (VGfloat) info[2]->NumberValue(),
                                                   (VGfloat) info[3]->NumberValue(),
                                                   (VGuint) info[4]->Uint32Value(),
                                                   (VGfloat) info[5]->NumberValue(),
                                                   (VGfloat) info[6]->NumberValue(),
                                                   (VGfloat) info[7]->NumberValue(),
                                                   (VGbitfield) info[8]->Uint32Value(),
                                                   (VGbitfield) info[9]->Uint32Value(),
                                                   (VGuint) info[10]->Uint32Value(),
                                                   (*glowColorRampStops))));
#else
  
#endif
}

NAN_METHOD(openvg::ext::GradientBevelKHR) {
  Nan::HandleScope scope;

  CheckArgs12(gradientBevelKHR,
              dstVGImage, Number, srcVGImage, Number,
              dimX, Number, dimY, Number, iterative, Number,
              strength, Number, distance, Number, angle, Number,
              filterFlags, Number, allowedQuality, Number,
              stopsCount, Number, Float32Array, Object);

#ifdef VG_VGEXT_PROTOTYPES
  Nan::TypedArrayContents<VGfloat> bevelColorRampStops(info[11]);

  info.GetReturnValue().Set(Nan::New<Uint32>(vguGradientBevelKHR((VGImage) info[0]->Uint32Value(),
                                                    (VGImage) info[1]->Uint32Value(),
                                                    (VGfloat) info[2]->NumberValue(),
                                                    (VGfloat) info[3]->NumberValue(),
                                                    (VGuint) info[4]->Uint32Value(),
                                                    (VGfloat) info[5]->NumberValue(),
                                                    (VGfloat) info[6]->NumberValue(),
                                                    (VGfloat) info[7]->NumberValue(),
                                                    (VGbitfield) info[8]->Uint32Value(),
                                                    (VGbitfield) info[9]->Uint32Value(),
                                                    (VGuint) info[10]->Uint32Value(),
                                                    (*bevelColorRampStops))));
#else
  
#endif
}

NAN_METHOD(openvg::ext::ProjectiveMatrixNDS) {
  Nan::HandleScope scope;

  CheckArgs1(projectiveMatrixNDS, enable, Boolean);

#ifdef VG_VGEXT_PROTOTYPES
  vgProjectiveMatrixNDS((VGboolean) info[0]->BooleanValue());
#endif

  
}

NAN_METHOD(openvg::ext::TransformClipLineNDS) {
  Nan::HandleScope scope;

  CheckArgs8(gradientBevelKHR,
             Ain, Number, Bin, Number, Cin, Number,
             Float32Array, Object, inverse, Boolean,
             Float32Array, Object, Float32Array, Object, Float32Array, Object);

#ifdef VG_VGEXT_PROTOTYPES
  Nan::TypedArrayContents<VGfloat> matrix(info[3]);
  Nan::TypedArrayContents<VGfloat> Aout(info[5]);
  Nan::TypedArrayContents<VGfloat> Bout(info[6]);
  Nan::TypedArrayContents<VGfloat> Cout(info[7]);

  info.GetReturnValue().Set(Nan::New<Uint32>(vguTransformClipLineNDS((VGfloat) info[0]->NumberValue(),
                                                        (VGfloat) info[1]->NumberValue(),
                                                        (VGfloat) info[2]->NumberValue(),
                                                        (*matrix),
                                                        (VGboolean) info[4]->BooleanValue(),
                                                        (*Aout),
                                                        (*Bout),
                                                        (*Cout))));
#else
  
#endif
}
