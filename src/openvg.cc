#include <stdio.h>

#include "VG/openvg.h"
#include "VG/vgu.h"
#include "VG/vgext.h"

#include <v8.h>
#include <node.h>
#include <node_buffer.h>
#include <node_api.h>

// TODO REMOVE
#define ARG_CHECKS

#include "openvg.h"
#include "egl.h"
#include "node-common.h"
#include "argchecks.h"

const bool kInitOpenGLES = false;

using namespace node;
using namespace v8;

// Functions suffixed as "OL" allow c-style pointer arithmetic
// "OL" - Offset + Length

void Init(napi_env env, napi_value exports, napi_value module, void* priv) {
  napi_property_descriptor descriptors[] = {
    DECLARE_NAPI_PROPERTY("nativeInit", openvg::NativeInit),
    DECLARE_NAPI_PROPERTY("nativeDeinit", openvg::NativeDeinit),

    DECLARE_NAPI_PROPERTY("getError", openvg::GetError),

    DECLARE_NAPI_PROPERTY("flush", openvg::Flush),
    DECLARE_NAPI_PROPERTY("finish", openvg::Finish),

    /* Getters and Setters */
    DECLARE_NAPI_PROPERTY("setF", openvg::SetF),
    DECLARE_NAPI_PROPERTY("setI", openvg::SetI),
    DECLARE_NAPI_PROPERTY("setFV", openvg::SetFV),
    DECLARE_NAPI_PROPERTY("setIV", openvg::SetIV),
    DECLARE_NAPI_PROPERTY("setFVOL", openvg::SetFVOL),
    DECLARE_NAPI_PROPERTY("setIVOL", openvg::SetIVOL),

    DECLARE_NAPI_PROPERTY("getF", openvg::GetF),
    DECLARE_NAPI_PROPERTY("getI", openvg::GetI),
    DECLARE_NAPI_PROPERTY("getVectorSize", openvg::GetVectorSize),
    DECLARE_NAPI_PROPERTY("getFV", openvg::GetFV),
    DECLARE_NAPI_PROPERTY("getIV", openvg::GetIV),
    DECLARE_NAPI_PROPERTY("getFVOL", openvg::GetFVOL),
    DECLARE_NAPI_PROPERTY("getIVOL", openvg::GetIVOL),

    DECLARE_NAPI_PROPERTY("setParameterF", openvg::SetParameterF),
    DECLARE_NAPI_PROPERTY("setParameterI", openvg::SetParameterI),
    DECLARE_NAPI_PROPERTY("setParameterFV", openvg::SetParameterFV),
    DECLARE_NAPI_PROPERTY("setParameterIV", openvg::SetParameterIV),
    DECLARE_NAPI_PROPERTY("setParameterFVOL", openvg::SetParameterFVOL),
    DECLARE_NAPI_PROPERTY("setParameterIVOL", openvg::SetParameterIVOL),

    DECLARE_NAPI_PROPERTY("getParameterF", openvg::GetParameterF),
    DECLARE_NAPI_PROPERTY("getParameterI", openvg::GetParameterI),
    DECLARE_NAPI_PROPERTY("getParameterVectorSize", openvg::GetParameterVectorSize),
    DECLARE_NAPI_PROPERTY("getParameterFV", openvg::GetParameterFV),
    DECLARE_NAPI_PROPERTY("getParameterIV", openvg::GetParameterIV),
    DECLARE_NAPI_PROPERTY("getParameterFVOL", openvg::GetParameterFVOL),
    DECLARE_NAPI_PROPERTY("getParameterIVOL", openvg::GetParameterIVOL),

    /* Matrix Manipulation */
    DECLARE_NAPI_PROPERTY("loadIdentity", openvg::LoadIdentity),
    DECLARE_NAPI_PROPERTY("loadMatrix", openvg::LoadMatrix),
    DECLARE_NAPI_PROPERTY("getMatrix", openvg::GetMatrix),
    DECLARE_NAPI_PROPERTY("multMatrix", openvg::MultMatrix),
    DECLARE_NAPI_PROPERTY("translate", openvg::Translate),
    DECLARE_NAPI_PROPERTY("scale", openvg::Scale),
    DECLARE_NAPI_PROPERTY("shear", openvg::Shear),
    DECLARE_NAPI_PROPERTY("rotate", openvg::Rotate),

    /* Masking and Clearing */
    DECLARE_NAPI_PROPERTY("mask", openvg::Mask),
    DECLARE_NAPI_PROPERTY("renderToMask", openvg::RenderToMask),
    DECLARE_NAPI_PROPERTY("createMaskLayer", openvg::CreateMaskLayer),
    DECLARE_NAPI_PROPERTY("destroyMaskLayer", openvg::DestroyMaskLayer),
    DECLARE_NAPI_PROPERTY("fillMaskLayer", openvg::FillMaskLayer),
    DECLARE_NAPI_PROPERTY("copyMask", openvg::CopyMask),
    DECLARE_NAPI_PROPERTY("clear", openvg::Clear),

    /* Paths */
    DECLARE_NAPI_PROPERTY("createPath", openvg::CreatePath),
    DECLARE_NAPI_PROPERTY("clearPath", openvg::ClearPath),
    DECLARE_NAPI_PROPERTY("destroyPath", openvg::DestroyPath),
    DECLARE_NAPI_PROPERTY("removePathCapabilities",
                            openvg::RemovePathCapabilities),
    DECLARE_NAPI_PROPERTY("getPathCapabilities",
                            openvg::GetPathCapabilities),
    DECLARE_NAPI_PROPERTY("appendPath", openvg::AppendPath),
    DECLARE_NAPI_PROPERTY("appendPathData", openvg::AppendPathData),
    DECLARE_NAPI_PROPERTY("appendPathDataO", openvg::AppendPathDataO), // Offsets
    DECLARE_NAPI_PROPERTY("modifyPathCoords", openvg::ModifyPathCoords),
    DECLARE_NAPI_PROPERTY("transformPath", openvg::TransformPath),
    DECLARE_NAPI_PROPERTY("interpolatePath", openvg::InterpolatePath),
    DECLARE_NAPI_PROPERTY("pathLength", openvg::PathLength),
    DECLARE_NAPI_PROPERTY("pointAlongPath", openvg::PointAlongPath),
    DECLARE_NAPI_PROPERTY("pathBounds", openvg::PathBounds),
    DECLARE_NAPI_PROPERTY("pathTransformedBounds",
                            openvg::PathTransformedBounds),
    DECLARE_NAPI_PROPERTY("drawPath", openvg::DrawPath),

    /* Paint */
    DECLARE_NAPI_PROPERTY("createPaint", openvg::CreatePaint),
    DECLARE_NAPI_PROPERTY("destroyPaint", openvg::DestroyPaint),
    DECLARE_NAPI_PROPERTY("setPaint", openvg::SetPaint),
    DECLARE_NAPI_PROPERTY("getPaint", openvg::GetPaint),
    DECLARE_NAPI_PROPERTY("setColor", openvg::SetColor),
    DECLARE_NAPI_PROPERTY("getColor", openvg::GetColor),
    DECLARE_NAPI_PROPERTY("paintPattern", openvg::PaintPattern),

    /* Images */
    DECLARE_NAPI_PROPERTY("createImage", openvg::CreateImage),
    DECLARE_NAPI_PROPERTY("destroyImage", openvg::DestroyImage),
    DECLARE_NAPI_PROPERTY("clearImage", openvg::ClearImage),
    DECLARE_NAPI_PROPERTY("imageSubData", openvg::ImageSubData),
    DECLARE_NAPI_PROPERTY("getImageSubData", openvg::GetImageSubData),
    DECLARE_NAPI_PROPERTY("childImage", openvg::ChildImage),
    DECLARE_NAPI_PROPERTY("getParent", openvg::GetParent),
    DECLARE_NAPI_PROPERTY("copyImage", openvg::CopyImage),
    DECLARE_NAPI_PROPERTY("drawImage", openvg::DrawImage),
    DECLARE_NAPI_PROPERTY("setPixels", openvg::SetPixels),
    DECLARE_NAPI_PROPERTY("writePixels", openvg::WritePixels),
    DECLARE_NAPI_PROPERTY("getPixels", openvg::GetPixels),
    DECLARE_NAPI_PROPERTY("readPixels", openvg::ReadPixels),
    DECLARE_NAPI_PROPERTY("copyPixels", openvg::CopyPixels),

    /* Text */
    DECLARE_NAPI_PROPERTY("createFont", openvg::CreateFont),
    DECLARE_NAPI_PROPERTY("destroyFont", openvg::DestroyFont),
    DECLARE_NAPI_PROPERTY("setGlyphToPath", openvg::SetGlyphToPath),
    DECLARE_NAPI_PROPERTY("setGlyphToImage", openvg::SetGlyphToImage),
    DECLARE_NAPI_PROPERTY("clearGlyph", openvg::ClearGlyph),
    DECLARE_NAPI_PROPERTY("drawGlyph", openvg::DrawGlyph),
    DECLARE_NAPI_PROPERTY("drawGlyphs", openvg::DrawGlyphs),

    /* Image Filters */
    DECLARE_NAPI_PROPERTY("colorMatrix", openvg::ColorMatrix),
    DECLARE_NAPI_PROPERTY("convolve", openvg::Convolve),
    DECLARE_NAPI_PROPERTY("separableConvolve", openvg::SeparableConvolve),
    DECLARE_NAPI_PROPERTY("gaussianBlur", openvg::GaussianBlur),
    DECLARE_NAPI_PROPERTY("lookup", openvg::Lookup),
    DECLARE_NAPI_PROPERTY("lookupSingle", openvg::LookupSingle),

    /* Hardware Queries */
    DECLARE_NAPI_PROPERTY("hardwareQuery", openvg::HardwareQuery),

    /* Renderer and Extension Information */
    DECLARE_NAPI_PROPERTY("getString", openvg::GetString),
  };

  /* Utilities */
  napi_value vgu;
  NAPI_CALL_RETURN_VOID(env, napi_create_object(env, &vgu));
  NAPI_CALL_RETURN_VOID(env, napi_set_named_property(env, exports, "vgu", vgu));

  napi_property_descriptor vgu_descriptors[] = {
    DECLARE_NAPI_PROPERTY("line", openvg::vgu::Line),
    DECLARE_NAPI_PROPERTY("polygon", openvg::vgu::Polygon),
    DECLARE_NAPI_PROPERTY("rect", openvg::vgu::Rect),
    DECLARE_NAPI_PROPERTY("roundRect", openvg::vgu::RoundRect),
    DECLARE_NAPI_PROPERTY("ellipse", openvg::vgu::Ellipse),
    DECLARE_NAPI_PROPERTY("arc", openvg::vgu::Arc),
    DECLARE_NAPI_PROPERTY("computeWarpQuadToSquare",
                          openvg::vgu::ComputeWarpQuadToSquare),
    DECLARE_NAPI_PROPERTY("computeWarpSquareToQuad",
                          openvg::vgu::ComputeWarpSquareToQuad),
    DECLARE_NAPI_PROPERTY("computeWarpQuadToQuad",
                          openvg::vgu::ComputeWarpQuadToQuad),
  };

  NAPI_CALL_RETURN_VOID(
    env,
    napi_define_properties(
      env,
      vgu,
      sizeof(vgu_descriptors) / sizeof(*vgu_descriptors),
      vgu_descriptors
    )
  );

  /* KHR extensions */
  napi_value ext;
  NAPI_CALL_RETURN_VOID(env, napi_create_object(env, &ext));
  NAPI_CALL_RETURN_VOID(env, napi_set_named_property(env, exports, "ext", ext));

  napi_property_descriptor ext_descriptors[] = {
    DECLARE_NAPI_PROPERTY("createEGLImageTargetKHR",
                          openvg::ext::CreateEGLImageTargetKHR),

    DECLARE_NAPI_PROPERTY("iterativeAverageBlurKHR",
                          openvg::ext::IterativeAverageBlurKHR),

    DECLARE_NAPI_PROPERTY("parametricFilterKHR",
                          openvg::ext::ParametricFilterKHR),
    DECLARE_NAPI_PROPERTY("dropShadowKHR", openvg::ext::DropShadowKHR),
    DECLARE_NAPI_PROPERTY("glowKHR", openvg::ext::GlowKHR),
    DECLARE_NAPI_PROPERTY("bevelKHR", openvg::ext::BevelKHR),
    DECLARE_NAPI_PROPERTY("gradientGlowKHR", openvg::ext::GradientGlowKHR),
    DECLARE_NAPI_PROPERTY("gradientBevelKHR", openvg::ext::GradientBevelKHR),

    DECLARE_NAPI_PROPERTY("projectiveMatrixNDS",
                         openvg::ext::ProjectiveMatrixNDS),
    DECLARE_NAPI_PROPERTY("transformClipLineNDS",
                         openvg::ext::TransformClipLineNDS),
  };

  NAPI_CALL_RETURN_VOID(
    env,
    napi_define_properties(
      env,
      ext,
      sizeof(ext_descriptors) / sizeof(*ext_descriptors),
      ext_descriptors
    )
  );

  /* EGL */
  napi_value eglProp;
  NAPI_CALL_RETURN_VOID(env, napi_create_object(env, &eglProp));
  NAPI_CALL_RETURN_VOID(
    env,
    napi_set_named_property(env, exports, "egl", eglProp)
  );

  egl::InitBindings(env, eglProp);

  NAPI_CALL_RETURN_VOID(
    env,
    napi_define_properties(
      env,
      exports,
      sizeof(descriptors) / sizeof(*descriptors),
      descriptors
    )
  );
}
NAPI_MODULE(openvg, Init)

#define CHECK_VG_ERROR {\
    VGErrorCode errorCode = vgGetError();\
    if(errorCode != VG_NO_ERROR) {\
      char buffer[100];\
      snprintf(&buffer[0], sizeof(buffer), "vgGetError: 0x%04x", errorCode);\
      __assert_fail (buffer, __FILE__, __LINE__, __PRETTY_FUNCTION__);\
    }\
  }

#define INLINE inline __attribute__((always_inline))

template<class C> class TypedArrayWrapper {
 private:
  void* data;
  size_t byte_offset;
  size_t array_length;
 public:
  INLINE TypedArrayWrapper(
    napi_env env,
    const napi_value array
  ): data(NULL), byte_offset(0), array_length(0) {
    bool is_typedarray;
    NAPI_CALL_RETURN_VOID(env, napi_is_typedarray(env, array, &is_typedarray));
    NAPI_ASSERT_RETURN_VOID(env, is_typedarray, "Argument is not a typed array.");

    napi_typedarray_type native_type;
    napi_value array_buffer;
    NAPI_CALL_RETURN_VOID(
      env,
      napi_get_typedarray_info(
        env,
        array,
        &native_type,
        &array_length,
        &data,
        &array_buffer,
        &byte_offset
      )
    );
  }

  INLINE C* pointer(int offset = 0) {
    return (C*) ((uint8_t*)(data) + offset);
  }

  INLINE size_t length() {
    return array_length;
  }
};

napi_value openvg::NativeInit(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  CheckArgs1(env, info, nativeInit, argc, args, object);

  egl::Init();

  if (kInitOpenGLES) {
    egl::InitOpenGLES();
  }

  CHECK_VG_ERROR;

  napi_value screen = args[0];
  napi_value screen_width, screen_height, display, surface, context;

  NAPI_CALL(env, napi_create_number(env, egl::State.screen_width, &screen_width));
  NAPI_CALL(env, napi_create_number(env, egl::State.screen_height, &screen_height));
  NAPI_CALL(env, napi_create_external(env, egl::State.display, NULL, NULL, &display));
  NAPI_CALL(env, napi_create_external(env, egl::State.surface, NULL, NULL, &surface));
  NAPI_CALL(env, napi_create_external(env, egl::State.context, NULL, NULL, &context));

  NAPI_CALL(env, napi_set_named_property(env, screen, "width", screen_width));
  NAPI_CALL(env, napi_set_named_property(env, screen, "height" , screen_height));
  NAPI_CALL(env, napi_set_named_property(env, screen, "display", display));
  NAPI_CALL(env, napi_set_named_property(env, screen, "surface", surface));
  NAPI_CALL(env, napi_set_named_property(env, screen, "context", context));

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::NativeDeinit(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 0);
  CheckArgs0(env, info, nativeDeinit, argc, args);

  egl::Finish();

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}


napi_value openvg::GetError(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 0);
  CheckArgs0(env, info, getError, argc, args);

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, vgGetError(), &result));
  return result;
}


napi_value openvg::Flush(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 0);
  CheckArgs0(env, info, flush, argc, args);

  vgFlush();

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::Finish(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 0);
  CheckArgs0(env, info, finish, argc, args);

  vgFinish();

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}


/* Getters and Setters */


napi_value openvg::SetF(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, setF, argc, args, number, number);

  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &type));

  double value;
  NAPI_CALL(env, napi_get_value_double(env, args[1], &value));

  vgSetf((VGParamType) type, (VGfloat) value);

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::SetI(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, setI, argc, args, number, number);

  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &type));

  int32_t value;
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &value));

  vgSeti((VGParamType) type, (VGint) value);

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::SetFV(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, setFV, argc, args, number, object);

  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &type));

  TypedArrayWrapper<VGfloat> values(env, args[1]);

  vgSetfv((VGParamType) type, values.length(), values.pointer());

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::SetIV(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, setIV, argc, args, number, object);

  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &type));

  TypedArrayWrapper<VGint> values(env, args[1]);

  vgSetiv((VGParamType) type, values.length(), values.pointer());

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::SetFVOL(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 4);
  CheckArgs4(env, info, setFVOL, argc, args, number, object, number, number);

  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &type));

  TypedArrayWrapper<VGfloat> values(env, args[1]);

  uint32_t offset;
  NAPI_CALL(env, napi_get_value_uint32(env, args[2], &offset));
  uint32_t length;
  NAPI_CALL(env, napi_get_value_uint32(env, args[3], &length));

  vgSetfv((VGParamType) type, (VGint) length, values.pointer(offset));

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::SetIVOL(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 4);
  CheckArgs4(env, info, setIVOL, argc, args, number, object, number, number);

  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &type));

  TypedArrayWrapper<VGint> values(env, args[1]);

  uint32_t offset;
  NAPI_CALL(env, napi_get_value_uint32(env, args[2], &offset));
  uint32_t length;
  NAPI_CALL(env, napi_get_value_uint32(env, args[3], &length));

  vgSetiv((VGParamType) type, (VGint) length, values.pointer(offset));

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::GetF(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, getF, argc, args, number);

  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &type));

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, vgGetf((VGParamType) type), &result));
  return result;
}

napi_value openvg::GetI(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, getI, argc, args, number);

  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &type));

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, vgGeti((VGParamType) type), &result));
  return result;
}

napi_value openvg::GetVectorSize(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, getVectorSize, argc, args, number);

  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &type));

  napi_value result;
  NAPI_CALL(
    env,
    napi_create_number(env, vgGetVectorSize((VGParamType) type), &result)
  );
  return result;
}

napi_value openvg::GetFV(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, getFV, argc, args, number, object);

  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &type));

  TypedArrayWrapper<VGfloat> values(env, args[1]);

  vgGetfv((VGParamType) type, values.length(), values.pointer());

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::GetIV(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, getIV, argc, args, number, object);

  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &type));

  TypedArrayWrapper<VGint> values(env, args[1]);

  vgGetiv((VGParamType) type, values.length(), values.pointer());

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::GetFVOL(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 4);
  CheckArgs4(env, info, getFV, argc, args, number, object, number, number);

  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &type));

  TypedArrayWrapper<VGfloat> values(env, args[1]);

  uint32_t offset;
  NAPI_CALL(env, napi_get_value_uint32(env, args[2], &offset));
  uint32_t length;
  NAPI_CALL(env, napi_get_value_uint32(env, args[3], &length));

  vgGetfv((VGParamType) type, (VGint) length, values.pointer(offset));

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::GetIVOL(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 4);
  CheckArgs4(env, info, getIV, argc, args, number, object, number, number);

  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &type));

  uint32_t offset;
  NAPI_CALL(env, napi_get_value_uint32(env, args[2], &offset));
  uint32_t length;
  NAPI_CALL(env, napi_get_value_uint32(env, args[3], &length));

  TypedArrayWrapper<VGint> values(env, args[1]);

  vgGetiv((VGParamType) type, (VGint) length, values.pointer(offset));

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}


napi_value openvg::SetParameterF(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 3);
  CheckArgs3(env, info, setParameterF, argc, args, number, number, number);

  uint32_t handle;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &handle));
  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &type));
  double value;
  NAPI_CALL(env, napi_get_value_double(env, args[2], &value));

  vgSetParameterf((VGHandle) handle, (VGParamType) type, (VGfloat) value);

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::SetParameterI(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 3);
  CheckArgs3(env, info, setParameterI, argc, args, number, number, number);

  uint32_t handle;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &handle));
  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &type));
  int32_t value;
  NAPI_CALL(env, napi_get_value_int32(env, args[2], &value));

  vgSetParameteri((VGHandle) handle, (VGParamType) type, (VGint) value);

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::SetParameterFV(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 3);
  CheckArgs3(env, info, setParameterFV, argc, args, number, number, object);

  uint32_t handle;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &handle));
  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &type));

  TypedArrayWrapper<VGfloat> values(env, args[2]);

  vgSetParameterfv(
    (VGHandle) handle,
    (VGParamType) type,
    values.length(),
    values.pointer()
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::SetParameterIV(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 3);
  CheckArgs3(env, info, setParameterIV, argc, args, number, number, object);

  uint32_t handle;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &handle));
  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &type));

  TypedArrayWrapper<VGint> values(env, args[2]);

  vgSetParameteriv(
    (VGHandle) handle,
    (VGParamType) type,
    values.length(),
    values.pointer()
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::SetParameterFVOL(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 5);
  CheckArgs5(env, info, setParameterFV, argc, args, number, number, object, number, number);

  uint32_t handle;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &handle));
  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &type));

  TypedArrayWrapper<VGfloat> values(env, args[2]);

  uint32_t offset;
  NAPI_CALL(env, napi_get_value_uint32(env, args[3], &offset));
  uint32_t length;
  NAPI_CALL(env, napi_get_value_uint32(env, args[4], &length));

  vgSetParameterfv(
    (VGHandle) handle,
    (VGParamType) type,
    length,
    values.pointer(offset)
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::SetParameterIVOL(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 5);
  CheckArgs5(env, info, setParameterIV, argc, args, number, number, object, number, number);

  uint32_t handle;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &handle));
  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &type));

  TypedArrayWrapper<VGint> values(env, args[2]);

  uint32_t offset;
  NAPI_CALL(env, napi_get_value_uint32(env, args[3], &offset));
  uint32_t length;
  NAPI_CALL(env, napi_get_value_uint32(env, args[4], &length));

  vgSetParameteriv(
    (VGHandle) handle,
    (VGParamType) type,
    length,
    values.pointer(offset)
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::GetParameterF(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, getParameterF, argc, args, number, number);

  uint32_t handle;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &handle));
  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &type));

  napi_value result;
  NAPI_CALL(
    env,
    napi_create_number(
      env,
      vgGetParameterf((VGHandle) handle, (VGParamType) type),
      &result
    )
  );
  return result;
}

napi_value openvg::GetParameterI(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, getParameterI, argc, args, number, number);

  uint32_t handle;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &handle));
  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &type));

  napi_value result;
  NAPI_CALL(
    env,
    napi_create_number(
      env,
      vgGetParameteri((VGHandle) handle, (VGParamType) type),
      &result
    )
  );
  return result;
}

napi_value openvg::GetParameterVectorSize(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, getParameterVectorSize, argc, args, number, number);

  uint32_t handle;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &handle));
  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &type));

  napi_value result;
  NAPI_CALL(
    env,
    napi_create_number(
      env,
      vgGetParameterVectorSize((VGHandle) handle, (VGParamType) type),
      &result
    )
  );
  return result;
}

napi_value openvg::GetParameterFV(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 3);
  CheckArgs3(env, info, getParameterFV, argc, args, number, number, object);

  uint32_t handle;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &handle));
  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &type));

  TypedArrayWrapper<VGfloat> values(env, args[2]);

  vgGetParameterfv(
    (VGHandle) handle,
    (VGParamType) type,
    values.length(),
    values.pointer()
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::GetParameterIV(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 3);
  CheckArgs3(env, info, getParameterIV, argc, args, number, number, object);

  uint32_t handle;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &handle));
  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &type));

  TypedArrayWrapper<VGint> values(env, args[2]);

  vgGetParameteriv(
    (VGHandle) handle,
    (VGParamType) type,
    values.length(),
    values.pointer()
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::GetParameterFVOL(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 5);
  CheckArgs5(env, info, getParameterFV, argc, args, number, number, object, number, number);

  uint32_t handle;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &handle));
  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &type));

  TypedArrayWrapper<VGfloat> values(env, args[2]);

  uint32_t offset;
  NAPI_CALL(env, napi_get_value_uint32(env, args[3], &offset));
  uint32_t length;
  NAPI_CALL(env, napi_get_value_uint32(env, args[4], &length));

  vgGetParameterfv(
    (VGHandle) handle,
    (VGParamType) type,
    length,
    values.pointer(offset)
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::GetParameterIVOL(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 5);
  CheckArgs5(env, info, getParameterIV, argc, args, number, number, object, number, number);

  uint32_t handle;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &handle));
  uint32_t type;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &type));

  TypedArrayWrapper<VGint> values(env, args[2]);

  uint32_t offset;
  NAPI_CALL(env, napi_get_value_uint32(env, args[3], &offset));
  uint32_t length;
  NAPI_CALL(env, napi_get_value_uint32(env, args[4], &length));

  vgGetParameteriv(
    (VGHandle) handle,
    (VGParamType) type,
    length,
    values.pointer(offset)
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}


/* Matrix Manipulation */


napi_value openvg::LoadIdentity(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 0);
  CheckArgs0(env, info, loadIdentity, argc, args);

  vgLoadIdentity();

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::LoadMatrix(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, loadMatrix, argc, args, object);

  TypedArrayWrapper<VGfloat> matrix(env, args[0]);

  vgLoadMatrix(matrix.pointer());

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::GetMatrix(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, getMatrix, argc, args, object);

  TypedArrayWrapper<VGfloat> matrix(env, args[0]);

  vgGetMatrix(matrix.pointer());

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::MultMatrix(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, multMatrix, argc, args, object);

  TypedArrayWrapper<VGfloat> matrix(env, args[0]);

  vgMultMatrix(matrix.pointer());

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::Translate(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, translate, argc, args, number, number);

  double x;
  NAPI_CALL(env, napi_get_value_double(env, args[0], &x));
  double y;
  NAPI_CALL(env, napi_get_value_double(env, args[1], &y));

  vgTranslate((VGfloat) x, (VGfloat) y);

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::Scale(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, scale, argc, args, number, number);

  double x;
  NAPI_CALL(env, napi_get_value_double(env, args[0], &x));
  double y;
  NAPI_CALL(env, napi_get_value_double(env, args[1], &y));

  vgScale((VGfloat) x, (VGfloat) y);

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::Shear(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, shear, argc, args, number, number);

  double x;
  NAPI_CALL(env, napi_get_value_double(env, args[0], &x));
  double y;
  NAPI_CALL(env, napi_get_value_double(env, args[1], &y));

  vgShear((VGfloat) x, (VGfloat) y);

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::Rotate(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, rotate, argc, args, number);

  double angle;
  NAPI_CALL(env, napi_get_value_double(env, args[0], &angle));

  vgRotate((VGfloat) angle);

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}


/* Masking and Clearing */


napi_value openvg::Mask(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 6);
  CheckArgs6(env, info, mask, argc, args, number, number, number, number, number, number);

  uint32_t handle;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &handle));
  uint32_t mask_op;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &mask_op));
  int32_t x;
  NAPI_CALL(env, napi_get_value_int32(env, args[2], &x));
  int32_t y;
  NAPI_CALL(env, napi_get_value_int32(env, args[3], &y));
  int32_t width;
  NAPI_CALL(env, napi_get_value_int32(env, args[4], &width));
  int32_t height;
  NAPI_CALL(env, napi_get_value_int32(env, args[5], &height));

  vgMask(
    (VGHandle) handle,
    static_cast<VGMaskOperation>(mask_op),
    x,
    y,
    width,
    height
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::RenderToMask(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 3);
  CheckArgs3(env, info, renderToMask, argc, args, number, number, number);

  uint32_t path;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &path));
  uint32_t bitfield;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &bitfield));
  uint32_t mask_op;
  NAPI_CALL(env, napi_get_value_uint32(env, args[2], &mask_op));

  vgRenderToMask(
    (VGPath) path,
    (VGbitfield) bitfield,
    (VGMaskOperation) mask_op
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::CreateMaskLayer(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, createMaskLayer, argc, args, number, number);

  int32_t width;
  NAPI_CALL(env, napi_get_value_int32(env, args[0], &width));
  int32_t height;
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &height));

  napi_value result;
  NAPI_CALL(
    env,
    napi_create_number(
      env,
      vgCreateMaskLayer((VGint) width, (VGint) height),
      &result
    )
  );
  return result;
}

napi_value openvg::DestroyMaskLayer(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, destroyMaskLayer, argc, args, number);

  uint32_t handle;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &handle));

  vgDestroyMaskLayer((VGMaskLayer) handle);

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::FillMaskLayer(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 6);
  CheckArgs6(env, info, fillMaskLayer, argc, args, number, number, number, number, number, number);

  uint32_t handle;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &handle));
  int32_t x;
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &x));
  int32_t y;
  NAPI_CALL(env, napi_get_value_int32(env, args[2], &y));
  int32_t width;
  NAPI_CALL(env, napi_get_value_int32(env, args[3], &width));
  int32_t height;
  NAPI_CALL(env, napi_get_value_int32(env, args[4], &height));
  double value;
  NAPI_CALL(env, napi_get_value_double(env, args[5], &value));

  vgFillMaskLayer(
    (VGMaskLayer) handle,
    (VGint) x,
    (VGint) y,
    (VGint) width,
    (VGint) height,
    (VGfloat) value
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::CopyMask(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 7);
  CheckArgs7(env, info, copyMask, argc, args, number, number, number, number, number, number, number);

  uint32_t handle;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &handle));
  int32_t dx;
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &dx));
  int32_t dy;
  NAPI_CALL(env, napi_get_value_int32(env, args[2], &dy));
  int32_t sx;
  NAPI_CALL(env, napi_get_value_int32(env, args[3], &sx));
  int32_t sy;
  NAPI_CALL(env, napi_get_value_int32(env, args[4], &sy));
  int32_t width;
  NAPI_CALL(env, napi_get_value_int32(env, args[5], &width));
  int32_t height;
  NAPI_CALL(env, napi_get_value_int32(env, args[6], &height));

  vgCopyMask(
    (VGMaskLayer) handle,
    (VGint) dx, (VGint) dy,
    (VGint) sx, (VGint) sy,
    (VGint) width, (VGint) height
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::Clear(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 4);
  CheckArgs4(env, info, clear, argc, args, number, number, number, number);

  int32_t x;
  NAPI_CALL(env, napi_get_value_int32(env, args[0], &x));
  int32_t y;
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &y));
  int32_t width;
  NAPI_CALL(env, napi_get_value_int32(env, args[2], &width));
  int32_t height;
  NAPI_CALL(env, napi_get_value_int32(env, args[3], &height));

  vgClear((VGint) x, (VGint) y, (VGint) width, (VGint) height);

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}


/* Paths */


napi_value openvg::CreatePath(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 7);
  CheckArgs7(env, info, createPath, argc, args, number, number, number, number, number, number, number);

  int32_t pathFormat;
  NAPI_CALL(env, napi_get_value_int32(env, args[0], &pathFormat));
  uint32_t pathDataType;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &pathDataType));
  double scale;
  NAPI_CALL(env, napi_get_value_double(env, args[2], &scale));
  double bias;
  NAPI_CALL(env, napi_get_value_double(env, args[3], &bias));
  int32_t segmentCapacityHint;
  NAPI_CALL(env, napi_get_value_int32(env, args[4], &segmentCapacityHint));
  int32_t coordCapacityHint;
  NAPI_CALL(env, napi_get_value_int32(env, args[5], &coordCapacityHint));
  uint32_t capabilities;
  NAPI_CALL(env, napi_get_value_uint32(env, args[6], &capabilities));

  VGPath path = vgCreatePath(
    (VGint) pathFormat,
    static_cast<VGPathDatatype>(pathDataType),
    (VGfloat) scale,
    (VGfloat) bias,
    (VGint) segmentCapacityHint,
    (VGint) coordCapacityHint,
    (VGbitfield) capabilities
  );

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, path, &result));
  return result;
}

napi_value openvg::ClearPath(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, clearPath, argc, args, number, number);

  uint32_t path;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &path));
  uint32_t capabilities;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &capabilities));

  vgClearPath((VGPath) path, (VGbitfield) capabilities);

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::DestroyPath(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, destroyPath, argc, args, number);

  uint32_t path;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &path));

  vgDestroyPath((VGPath) path);

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::RemovePathCapabilities(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, removePathCapabilities, argc, args, number, number);

  uint32_t path;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &path));
  uint32_t capabilities;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &capabilities));

  vgRemovePathCapabilities((VGPath) path, (VGbitfield) capabilities);

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::GetPathCapabilities(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, getPathCapabilities, argc, args, number);

  uint32_t path;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &path));

  napi_value result;
  NAPI_CALL(
    env,
    napi_create_number(env, vgGetPathCapabilities((VGPath) path), &result)
  );
  return result;
}

napi_value openvg::AppendPath(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, appendPath, argc, args, number, number);

  uint32_t dstPath;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &dstPath));
  uint32_t srcPath;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &srcPath));

  vgAppendPath((VGPath) dstPath, (VGPath) srcPath);

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::AppendPathData(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 4);
  CheckArgs4(env, info, appendPathData, argc, args, number, number, object, object);

  uint32_t dstPath;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &dstPath));
  int32_t numSegments;
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &numSegments));
  TypedArrayWrapper<VGubyte> segments(env, args[2]);
  TypedArrayWrapper<void> data(env, args[3]);

  vgAppendPathData(
    (VGPath) dstPath,
    (VGint) numSegments,
    segments.pointer(),
    data.pointer()
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::AppendPathDataO(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 6);
  CheckArgs6(env, info, appendPathData, argc, args, number, number, object, number, object, number);

  uint32_t dstPath;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &dstPath));
  int32_t numSegments;
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &numSegments));

  TypedArrayWrapper<VGubyte> segments(env, args[2]);

  uint32_t segmentOffset;
  NAPI_CALL(env, napi_get_value_uint32(env, args[3], &segmentOffset));

  TypedArrayWrapper<void> data(env, args[4]);

  uint32_t dataOffset;
  NAPI_CALL(env, napi_get_value_uint32(env, args[5], &dataOffset));

  vgAppendPathData(
    (VGPath) dstPath,
    (VGint) numSegments,
    segments.pointer(segmentOffset),
    data.pointer(dataOffset)
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::ModifyPathCoords(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 4);
  CheckArgs4(env, info, modifyPathCoords, argc, args, number, number, number, object);

  uint32_t path;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &path));
  int32_t startIndex;
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &startIndex));
  int32_t numSegments;
  NAPI_CALL(env, napi_get_value_int32(env, args[2], &numSegments));

  TypedArrayWrapper<void> data(env, args[3]);

  vgModifyPathCoords(
    (VGPath) path,
    (VGint) startIndex,
    (VGint) numSegments,
    data.pointer()
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::TransformPath(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, transformPath, argc, args, number, number);

  uint32_t dstPath;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &dstPath));
  uint32_t srcPath;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &srcPath));

  vgTransformPath((VGPath) dstPath, (VGPath) srcPath);

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::InterpolatePath(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 4);
  CheckArgs4(env, info, interpolatePath, argc, args, number, number, number, number);

  uint32_t dstPath;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &dstPath));
  uint32_t startPath;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &startPath));
  uint32_t endPath;
  NAPI_CALL(env, napi_get_value_uint32(env, args[2], &endPath));
  double amount;
  NAPI_CALL(env, napi_get_value_double(env, args[3], &amount));

  VGboolean res = vgInterpolatePath(
    (VGPath) dstPath,
    (VGPath) startPath,
    (VGPath) endPath,
    (VGfloat) amount
  );

  napi_value result;
  NAPI_CALL(env, napi_get_boolean(env, res, &result));
  return result;
}

napi_value openvg::PathLength(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 3);
  CheckArgs3(env, info, pathLength, argc, args, number, number, number);

  uint32_t path;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &path));
  int32_t startSegment;
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &startSegment));
  int32_t numSegments;
  NAPI_CALL(env, napi_get_value_int32(env, args[2], &numSegments));

  VGfloat length = vgPathLength(
    (VGPath) path,
    (VGint) startSegment,
    (VGint) numSegments
  );

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, length, &result));
  return result;
}

napi_value openvg::PointAlongPath(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 4);
  CheckArgs4(env, info, pointAlongPath, argc, args, number, number, number, number);

  uint32_t path;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &path));
  int32_t startSegment;
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &startSegment));
  int32_t numSegments;
  NAPI_CALL(env, napi_get_value_int32(env, args[2], &numSegments));
  double distance;
  NAPI_CALL(env, napi_get_value_double(env, args[3], &distance));

  VGfloat x, y, tx, ty;

  vgPointAlongPath(
    (VGPath) path,
    (VGint) startSegment,
    (VGint) numSegments,
    (VGfloat) distance,
    &x, &y, &tx, &ty
  );

  napi_value point;
  NAPI_CALL(env, napi_create_object(env, &point));

  napi_value rX, rY, rTX, rTY;
  NAPI_CALL(env, napi_create_number(env, x, &rX));
  NAPI_CALL(env, napi_set_named_property(env, point, "x", rX));

  NAPI_CALL(env, napi_create_number(env, y, &rY));
  NAPI_CALL(env, napi_set_named_property(env, point, "y", rY));

  NAPI_CALL(env, napi_create_number(env, tx, &rTX));
  NAPI_CALL(env, napi_set_named_property(env, point, "tx", rTX));

  NAPI_CALL(env, napi_create_number(env, ty, &rTY));
  NAPI_CALL(env, napi_set_named_property(env, point, "ty", rTY));

  return point;
}

napi_value openvg::PathBounds(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, pathBounds, argc, args, number);

  uint32_t path;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &path));

  VGfloat minX, minY, width, height;
  vgPathBounds((VGPath) path, &minX, &minY, &width, &height);

  napi_value bounds;
  NAPI_CALL(env, napi_create_object(env, &bounds));

  napi_value rX, rY, rW, rH;
  NAPI_CALL(env, napi_create_number(env, minX, &rX));
  NAPI_CALL(env, napi_set_named_property(env, bounds, "x", rX));

  NAPI_CALL(env, napi_create_number(env, minY, &rY));
  NAPI_CALL(env, napi_set_named_property(env, bounds, "y", rY));

  NAPI_CALL(env, napi_create_number(env, width, &rW));
  NAPI_CALL(env, napi_set_named_property(env, bounds, "w", rW));

  NAPI_CALL(env, napi_create_number(env, height, &rH));
  NAPI_CALL(env, napi_set_named_property(env, bounds, "h", rH));

  return bounds;
}

napi_value openvg::PathTransformedBounds(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, pathTransformedBounds, argc, args, number, object);

  uint32_t path;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &path));

  VGfloat minX, minY, width, height;

  vgPathTransformedBounds((VGPath) path, &minX, &minY, &width, &height);

  napi_value bounds;
  NAPI_CALL(env, napi_create_object(env, &bounds));

  napi_value rX, rY, rW, rH;
  NAPI_CALL(env, napi_create_number(env, minX, &rX));
  NAPI_CALL(env, napi_set_named_property(env, bounds, "x", rX));

  NAPI_CALL(env, napi_create_number(env, minY, &rY));
  NAPI_CALL(env, napi_set_named_property(env, bounds, "y", rY));

  NAPI_CALL(env, napi_create_number(env, width, &rW));
  NAPI_CALL(env, napi_set_named_property(env, bounds, "w", rW));

  NAPI_CALL(env, napi_create_number(env, height, &rH));
  NAPI_CALL(env, napi_set_named_property(env, bounds, "h", rH));

  return bounds;
}

napi_value openvg::DrawPath(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, drawPath, argc, args, number, number);

  uint32_t path;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &path));
  uint32_t paintModes;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &paintModes));

  vgDrawPath((VGPath) path, (VGbitfield) paintModes);

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}


/* Paint */


napi_value openvg::CreatePaint(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 0);
  CheckArgs0(env, info, createPaint, argc, args);

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, vgCreatePaint(), &result));
  return result;
}

napi_value openvg::DestroyPaint(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, destroyPaint, argc, args, number);

  uint32_t paint;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &paint));

  vgDestroyPaint((VGPaint) paint);

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::SetPaint(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, setPaint, argc, args, number, number);

  uint32_t paint;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &paint));
  uint32_t paintModes;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &paintModes));

  vgSetPaint((VGPaint) paint, (VGbitfield) paintModes);

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::GetPaint(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, getPaint, argc, args, number);

  uint32_t paint;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &paint));

  uint32_t paintModes = vgGetPaint(static_cast<VGPaintMode>(paint));

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, paintModes, &result));
  return result;
}

napi_value openvg::SetColor(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, setColor, argc, args, number, number);

  uint32_t paint;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &paint));
  uint32_t rgba;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &rgba));

  vgSetColor((VGPaint) paint, (VGuint) rgba);

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::GetColor(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, getColor, argc, args, number);

  uint32_t paint;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &paint));
  uint32_t color = vgGetColor((VGPaint) paint);

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, color, &result));
  return result;
}

napi_value openvg::PaintPattern(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, paintPattern, argc, args, number, number);

  uint32_t paint;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &paint));
  uint32_t image;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &image));

  vgPaintPattern((VGPaint) paint, (VGImage) image);

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}


/* Images */


napi_value openvg::CreateImage(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 4);
  CheckArgs4(env, info, createImage, argc, args, number, number, number, number);

  uint32_t imageFormat;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &imageFormat));
  int32_t width;
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &width));
  int32_t height;
  NAPI_CALL(env, napi_get_value_int32(env, args[2], &height));
  uint32_t allowedQuality;
  NAPI_CALL(env, napi_get_value_uint32(env, args[3], &allowedQuality));

  VGImage image = vgCreateImage(
    static_cast<VGImageFormat>(imageFormat),
    (VGint) width,
    (VGint) height,
    (VGuint) allowedQuality
  );

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, image, &result));
  return result;
}

napi_value openvg::DestroyImage(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, destroyImage, argc, args, number);

  uint32_t image;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &image));

  vgDestroyImage((VGImage) image);

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::ClearImage(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 5);
  CheckArgs5(env, info, clearImage, argc, args, number, number, number, number, number);

  uint32_t image;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &image));
  int32_t x;
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &x));
  int32_t y;
  NAPI_CALL(env, napi_get_value_int32(env, args[2], &y));
  int32_t width;
  NAPI_CALL(env, napi_get_value_int32(env, args[3], &width));
  int32_t height;
  NAPI_CALL(env, napi_get_value_int32(env, args[4], &height));

  vgClearImage(
    (VGImage) image,
    (VGint) x,
    (VGint) y,
    (VGint) width,
    (VGint) height
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::ImageSubData(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 8);
  CheckArgs8(env, info, imageSubData, argc, args, number, object, number, number, number, number, number, number);

  uint32_t image;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &image));

  TypedArrayWrapper<void> data(env, args[1]);
  int32_t dataStride;
  NAPI_CALL(env, napi_get_value_int32(env, args[2], &dataStride));
  uint32_t dataFormat;
  NAPI_CALL(env, napi_get_value_uint32(env, args[3], &dataFormat));
  int32_t x;
  NAPI_CALL(env, napi_get_value_int32(env, args[4], &x));
  int32_t y;
  NAPI_CALL(env, napi_get_value_int32(env, args[5], &y));
  int32_t width;
  NAPI_CALL(env, napi_get_value_int32(env, args[6], &width));
  int32_t height;
  NAPI_CALL(env, napi_get_value_int32(env, args[7], &height));

  vgImageSubData(
    (VGImage) image,
    data.pointer(),
    (VGint) dataStride,
    static_cast<VGImageFormat>(dataFormat),
    (VGint) x,
    (VGint) y,
    (VGint) width,
    (VGint) height
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::GetImageSubData(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 8);
  CheckArgs8(env, info, getImageSubData, argc, args, number, object, number, number, number, number, number, number);

  uint32_t image;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &image));

  TypedArrayWrapper<void> data(env, args[1]);

  int32_t dataStride;
  NAPI_CALL(env, napi_get_value_int32(env, args[2], &dataStride));
  uint32_t dataFormat;
  NAPI_CALL(env, napi_get_value_uint32(env, args[3], &dataFormat));
  int32_t x;
  NAPI_CALL(env, napi_get_value_int32(env, args[4], &x));
  int32_t y;
  NAPI_CALL(env, napi_get_value_int32(env, args[5], &y));
  int32_t width;
  NAPI_CALL(env, napi_get_value_int32(env, args[6], &width));
  int32_t height;
  NAPI_CALL(env, napi_get_value_int32(env, args[7], &height));

  vgGetImageSubData(
    (VGImage) image,
    data.pointer(),
    (VGint) dataStride,
    static_cast<VGImageFormat>(dataFormat),
    (VGint) x,
    (VGint) y,
    (VGint) width,
    (VGint) height
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::ChildImage(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 5);
  CheckArgs5(env, info, childImage, argc, args, number, number, number, number, number);

  uint32_t image;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &image));
  int32_t x;
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &x));
  int32_t y;
  NAPI_CALL(env, napi_get_value_int32(env, args[2], &y));
  int32_t width;
  NAPI_CALL(env, napi_get_value_int32(env, args[3], &width));
  int32_t height;
  NAPI_CALL(env, napi_get_value_int32(env, args[4], &height));

  uint32_t child = vgChildImage(
    (VGImage) image,
    (VGint) x,
    (VGint) y,
    (VGint) width,
    (VGint) height
  );


  napi_value result;
  NAPI_CALL(env, napi_create_number(env, child, &result));
  return result;
}

napi_value openvg::GetParent(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, getParent, argc, args, number);

  uint32_t image;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &image));

  uint32_t parent = vgGetParent(image);

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, parent, &result));
  return result;
}

napi_value openvg::CopyImage(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 9);
  CheckArgs9(env, info, copyImage, argc, args, number, number, number, number, number, number, number, number, boolean);

  uint32_t dstImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &dstImage));
  int32_t dX;
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &dX));
  int32_t dY;
  NAPI_CALL(env, napi_get_value_int32(env, args[2], &dY));
  uint32_t srcImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[3], &srcImage));
  int32_t sX;
  NAPI_CALL(env, napi_get_value_int32(env, args[4], &sX));
  int32_t sY;
  NAPI_CALL(env, napi_get_value_int32(env, args[5], &sY));
  int32_t width;
  NAPI_CALL(env, napi_get_value_int32(env, args[6], &width));
  int32_t height;
  NAPI_CALL(env, napi_get_value_int32(env, args[7], &height));
  bool dither;
  NAPI_CALL(env, napi_get_value_bool(env, args[8], &dither));

  vgCopyImage(
    (VGImage) dstImage,
    (VGint) dX,
    (VGint) dY,
    (VGImage) srcImage,
    (VGint) sX,
    (VGint) sY,
    (VGint) width,
    (VGint) height,
    (VGboolean) dither
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::DrawImage(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, drawImage, argc, args, number);

  uint32_t image;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &image));

  vgDrawImage((VGImage) image);

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::SetPixels(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 7);
  CheckArgs7(env, info, setPixels, argc, args, number, number, number, number, number, number, number);

  int32_t dX;
  NAPI_CALL(env, napi_get_value_int32(env, args[0], &dX));
  int32_t dY;
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &dY));
  uint32_t srcImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[2], &srcImage));
  int32_t sX;
  NAPI_CALL(env, napi_get_value_int32(env, args[3], &sX));
  int32_t sY;
  NAPI_CALL(env, napi_get_value_int32(env, args[4], &sY));
  int32_t width;
  NAPI_CALL(env, napi_get_value_int32(env, args[5], &width));
  int32_t height;
  NAPI_CALL(env, napi_get_value_int32(env, args[6], &height));

  vgSetPixels(
    (VGint) dX,
    (VGint) dY,
    (VGImage) srcImage,
    (VGint) sX,
    (VGint) sY,
    (VGint) width,
    (VGint) height
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::WritePixels(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 7);
  CheckArgs7(env, info, writePixels, argc, args, object, number, number, number, number, number, number);

  TypedArrayWrapper<void> data(env, args[0]);
  int32_t dataStride;
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &dataStride));
  uint32_t dataFormat;
  NAPI_CALL(env, napi_get_value_uint32(env, args[2], &dataFormat));
  int32_t x;
  NAPI_CALL(env, napi_get_value_int32(env, args[3], &x));
  int32_t y;
  NAPI_CALL(env, napi_get_value_int32(env, args[4], &y));
  int32_t width;
  NAPI_CALL(env, napi_get_value_int32(env, args[5], &width));
  int32_t height;
  NAPI_CALL(env, napi_get_value_int32(env, args[6], &height));

  vgWritePixels(
    data.pointer(),
    dataStride,
    static_cast<VGImageFormat>(dataFormat),
    (VGint) x,
    (VGint) y,
    (VGint) width,
    (VGint) height
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::GetPixels(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 7);
  CheckArgs7(env, info, getPixels, argc, args, number, number, number, number, number, number, number);

  uint32_t dstImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &dstImage));
  int32_t dX;
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &dX));
  int32_t dY;
  NAPI_CALL(env, napi_get_value_int32(env, args[2], &dY));
  int32_t sX;
  NAPI_CALL(env, napi_get_value_int32(env, args[3], &sX));
  int32_t sY;
  NAPI_CALL(env, napi_get_value_int32(env, args[4], &sY));
  int32_t width;
  NAPI_CALL(env, napi_get_value_int32(env, args[5], &width));
  int32_t height;
  NAPI_CALL(env, napi_get_value_int32(env, args[6], &height));

  vgGetPixels(
    (VGImage) dstImage,
    (VGint) dX,
    (VGint) dY,
    (VGint) sX,
    (VGint) sY,
    (VGint) width,
    (VGint) height
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::ReadPixels(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 7);
  CheckArgs7(env, info, readPixels, argc, args, object, number, number, number, number, number, number);

  TypedArrayWrapper<void> data(env, args[0]);
  int32_t dataStride;
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &dataStride));
  uint32_t dataFormat;
  NAPI_CALL(env, napi_get_value_uint32(env, args[2], &dataFormat));
  int32_t sX;
  NAPI_CALL(env, napi_get_value_int32(env, args[3], &sX));
  int32_t sY;
  NAPI_CALL(env, napi_get_value_int32(env, args[4], &sY));
  int32_t width;
  NAPI_CALL(env, napi_get_value_int32(env, args[5], &width));
  int32_t height;
  NAPI_CALL(env, napi_get_value_int32(env, args[6], &height));

  vgReadPixels(
    data.pointer(),
    (VGint) dataStride,
    static_cast<VGImageFormat>(dataFormat),
    (VGint) sX,
    (VGint) sY,
    (VGint) width,
    (VGint) height
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::CopyPixels(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 6);
  CheckArgs6(env, info, copyPixels, argc, args, number, number, number, number, number, number);

  int32_t dX;
  NAPI_CALL(env, napi_get_value_int32(env, args[0], &dX));
  int32_t dY;
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &dY));
  int32_t sX;
  NAPI_CALL(env, napi_get_value_int32(env, args[2], &sX));
  int32_t sY;
  NAPI_CALL(env, napi_get_value_int32(env, args[3], &sY));
  int32_t width;
  NAPI_CALL(env, napi_get_value_int32(env, args[4], &width));
  int32_t height;
  NAPI_CALL(env, napi_get_value_int32(env, args[5], &height));

  vgCopyPixels(
    (VGint) dX,
    (VGint) dY,
    (VGint) sX,
    (VGint) sY,
    (VGint) width,
    (VGint) height
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}


/* Text */


napi_value openvg::CreateFont(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, createFont, argc, args, number);

  int32_t capacityHint;
  NAPI_CALL(env, napi_get_value_int32(env, args[0], &capacityHint));

  VGFont font = vgCreateFont((VGint) capacityHint);

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, font, &result));
  return result;
}

napi_value openvg::DestroyFont(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, destroyFont, argc, args, number);

  uint32_t font;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &font));

  vgDestroyFont((VGFont) font);

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::SetGlyphToPath(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 6);
  CheckArgs6(env, info, setGlyphToPath, argc, args, number, number, number, boolean, object, object);

  uint32_t font;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &font));
  uint32_t glyphIndex;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &glyphIndex));
  uint32_t path;
  NAPI_CALL(env, napi_get_value_uint32(env, args[2], &path));
  bool isHinted;
  NAPI_CALL(env, napi_get_value_bool(env, args[3], &isHinted));
  TypedArrayWrapper<VGfloat> glyphOrigin(env, args[4]);
  TypedArrayWrapper<VGfloat> escapement(env, args[5]);

  vgSetGlyphToPath(
    (VGFont) font,
    (VGuint) glyphIndex,
    (VGPath) path,
    (VGboolean) isHinted,
    glyphOrigin.pointer(),
    escapement.pointer()
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::SetGlyphToImage(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 5);
  CheckArgs5(env, info, setGlyphToImage, argc, args, number, number, number, object, object);

  uint32_t font;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &font));
  uint32_t glyphIndex;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &glyphIndex));
  uint32_t image;
  NAPI_CALL(env, napi_get_value_uint32(env, args[2], &image));
  TypedArrayWrapper<VGfloat> glyphOrigin(env, args[3]);
  TypedArrayWrapper<VGfloat> escapement(env, args[4]);

  vgSetGlyphToImage(
    (VGFont) font,
    (VGuint) glyphIndex,
    (VGImage) image,
    glyphOrigin.pointer(),
    escapement.pointer()
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::ClearGlyph(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, clearGlyph, argc, args, number, number);

  uint32_t font;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &font));
  uint32_t glyphIndex;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &glyphIndex));

  vgClearGlyph((VGFont) font, (VGuint) glyphIndex);

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::DrawGlyph(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 4);
  CheckArgs4(env, info, drawGlyph, argc, args, number, number, number, boolean);

  uint32_t font;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &font));
  uint32_t glyphIndex;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &glyphIndex));
  uint32_t paintModes;
  NAPI_CALL(env, napi_get_value_uint32(env, args[2], &paintModes));
  bool allowAutoHinting;
  NAPI_CALL(env, napi_get_value_bool(env, args[3], &allowAutoHinting));

  vgDrawGlyph(
    (VGFont) font,
    (VGuint) glyphIndex,
    (VGbitfield) paintModes,
    (VGboolean) allowAutoHinting
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::DrawGlyphs(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 7);
  CheckArgs7(env, info, drawGlyphs, argc, args, number, number, object, object, object, number, boolean);

  uint32_t font;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &font));
  int32_t glyphCount;
  NAPI_CALL(env, napi_get_value_int32(env, args[1], &glyphCount));
  TypedArrayWrapper<VGuint> glyphIndices(env, args[2]);
  TypedArrayWrapper<VGfloat> adjustments_x(env, args[3]);
  TypedArrayWrapper<VGfloat> adjustments_y(env, args[4]);
  uint32_t paintModes;
  NAPI_CALL(env, napi_get_value_uint32(env, args[5], &paintModes));
  bool allowAutoHinting;
  NAPI_CALL(env, napi_get_value_bool(env, args[6], &allowAutoHinting));

  vgDrawGlyphs(
    (VGFont) font,
    (VGuint) glyphCount,
    glyphIndices.pointer(),
    adjustments_x.pointer(),
    adjustments_y.pointer(),
    (VGbitfield) paintModes,
    (VGboolean) allowAutoHinting
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}


/* Image Filters */


napi_value openvg::ColorMatrix(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 3);
  CheckArgs3(env, info, colorMatrix, argc, args, number, number, object);

  uint32_t dstImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &dstImage));
  uint32_t srcImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &srcImage));
  TypedArrayWrapper<VGfloat> matrix(env, args[2]);

  vgColorMatrix((VGImage) dstImage, (VGImage) srcImage, matrix.pointer());

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::Convolve(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 10);
  CheckArgs10(env, info, convolve, argc, args, number, number, number, number, number, number, object, number, number, number);

  uint32_t dstImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &dstImage));
  uint32_t srcImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &srcImage));
  int32_t kernelWidth;
  NAPI_CALL(env, napi_get_value_int32(env, args[2], &kernelWidth));
  int32_t kernelHeight;
  NAPI_CALL(env, napi_get_value_int32(env, args[3], &kernelHeight));
  int32_t shiftX;
  NAPI_CALL(env, napi_get_value_int32(env, args[4], &shiftX));
  int32_t shiftY;
  NAPI_CALL(env, napi_get_value_int32(env, args[5], &shiftY));
  TypedArrayWrapper<VGshort> kernel(env, args[6]);
  double scale;
  NAPI_CALL(env, napi_get_value_double(env, args[7], &scale));
  double bias;
  NAPI_CALL(env, napi_get_value_double(env, args[8], &bias));
  uint32_t tilingMode;
  NAPI_CALL(env, napi_get_value_uint32(env, args[9], &tilingMode));

  vgConvolve(
    (VGImage) dstImage,
    (VGImage) srcImage,
    (VGint) kernelWidth,
    (VGint) kernelHeight,
    (VGint) shiftX,
    (VGint) shiftY,
    kernel.pointer(),
    (VGfloat) scale,
    (VGfloat) bias,
    static_cast<VGTilingMode>(tilingMode)
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::SeparableConvolve(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 11);
  CheckArgs11(env, info, separableConvolve, argc, args, number, number, number, number, number, number, object, object, number, number, number);

  uint32_t dstImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &dstImage));
  uint32_t srcImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &srcImage));
  int32_t kernelWidth;
  NAPI_CALL(env, napi_get_value_int32(env, args[2], &kernelWidth));
  int32_t kernelHeight;
  NAPI_CALL(env, napi_get_value_int32(env, args[3], &kernelHeight));
  int32_t shiftX;
  NAPI_CALL(env, napi_get_value_int32(env, args[4], &shiftX));
  int32_t shiftY;
  NAPI_CALL(env, napi_get_value_int32(env, args[5], &shiftY));
  TypedArrayWrapper<VGshort> kernelX(env, args[6]);
  TypedArrayWrapper<VGshort> kernelY(env, args[7]);
  double scale;
  NAPI_CALL(env, napi_get_value_double(env, args[8], &scale));
  double bias;
  NAPI_CALL(env, napi_get_value_double(env, args[9], &bias));
  uint32_t tilingMode;
  NAPI_CALL(env, napi_get_value_uint32(env, args[10], &tilingMode));

  vgSeparableConvolve(
    (VGImage) dstImage,
    (VGImage) srcImage,
    (VGint) kernelWidth,
    (VGint) kernelHeight,
    (VGint) shiftX,
    (VGint) shiftY,
    kernelX.pointer(),
    kernelY.pointer(),
    (VGfloat) scale,
    (VGfloat) bias,
    static_cast<VGTilingMode>(tilingMode)
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::GaussianBlur(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 5);
  CheckArgs5(env, info, gaussianBlur, argc, args, number, number, number, number, number);

  uint32_t dstImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &dstImage));
  uint32_t srcImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &srcImage));
  double stdDeviationX;
  NAPI_CALL(env, napi_get_value_double(env, args[2], &stdDeviationX));
  double stdDeviationY;
  NAPI_CALL(env, napi_get_value_double(env, args[3], &stdDeviationY));
  uint32_t tilingMode;
  NAPI_CALL(env, napi_get_value_uint32(env, args[4], &tilingMode));

  vgGaussianBlur(
    (VGImage) dstImage,
    (VGImage) srcImage,
    (VGfloat) stdDeviationX,
    (VGfloat) stdDeviationY,
    static_cast<VGTilingMode>(tilingMode)
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::Lookup(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 9);
  CheckArgs9(env, info, lookup, argc, args, number, number, number, object, object, object, object, boolean, boolean);

  uint32_t dstImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &dstImage));
  uint32_t srcImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &srcImage));
  TypedArrayWrapper<VGubyte> redLUT(env, args[2]);
  TypedArrayWrapper<VGubyte> greenLUT(env, args[3]);
  TypedArrayWrapper<VGubyte> blueLUT(env, args[4]);
  TypedArrayWrapper<VGubyte> alphaLUT(env, args[5]);
  bool outputLinear;
  NAPI_CALL(env, napi_get_value_bool(env, args[6], &outputLinear));
  bool outputPremultiplied;
  NAPI_CALL(env, napi_get_value_bool(env, args[7], &outputPremultiplied));

  vgLookup(
    (VGImage) dstImage,
    (VGImage) srcImage,
    redLUT.pointer(),
    greenLUT.pointer(),
    blueLUT.pointer(),
    alphaLUT.pointer(),
    (VGboolean) outputLinear,
    (VGboolean) outputPremultiplied
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::LookupSingle(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 6);
  CheckArgs6(env, info, lookupSingle, argc, args, number, number, object, number, boolean, boolean);

  uint32_t dstImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &dstImage));
  uint32_t srcImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &srcImage));
  TypedArrayWrapper<VGuint> lookupTable(env, args[2]);
  uint32_t sourceChannel;
  NAPI_CALL(env, napi_get_value_uint32(env, args[3], &sourceChannel));
  bool outputLinear;
  NAPI_CALL(env, napi_get_value_bool(env, args[4], &outputLinear));
  bool outputPremultiplied;
  NAPI_CALL(env, napi_get_value_bool(env, args[5], &outputPremultiplied));

  vgLookupSingle(
    (VGImage) dstImage,
    (VGImage) srcImage,
    lookupTable.pointer(),
    static_cast<VGImageChannel>(sourceChannel),
    (VGboolean) outputLinear,
    (VGboolean) outputPremultiplied
  );

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}


/* Hardware Queries */
napi_value openvg::HardwareQuery(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 2);
  CheckArgs2(env, info, hardwareQuery, argc, args, number, number);

  uint32_t key;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &key));
  int32_t setting;
  NAPI_CALL(env, napi_get_value_int32(env, args[0], &setting));

  uint32_t value = vgHardwareQuery(
    static_cast<VGHardwareQueryType>(key),
    (VGint) setting
  );

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, value, &result));
  return result;
}


/* Renderer and Extension Information */
VG_API_CALL const VGubyte * VG_API_ENTRY vgGetString(VGStringID name) VG_API_EXIT;

napi_value openvg::GetString(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, getString, argc, args, number);

  uint32_t name;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &name));

  const VGubyte* string = vgGetString(static_cast<VGStringID>(name));

  napi_value result;
  NAPI_CALL(env, napi_create_string_utf8(env, (char*) string, -1, &result));
  return result;
}


/* Utilities */


napi_value openvg::vgu::Line(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 5);
  CheckArgs5(env, info, line, argc, args, number, number, number, number, number);

  uint32_t path;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &path));
  double x0;
  NAPI_CALL(env, napi_get_value_double(env, args[1], &x0));
  double y0;
  NAPI_CALL(env, napi_get_value_double(env, args[2], &y0));
  double x1;
  NAPI_CALL(env, napi_get_value_double(env, args[3], &x1));
  double y1;
  NAPI_CALL(env, napi_get_value_double(env, args[4], &y1));

  VGUErrorCode errorCode = vguLine(
    (VGPath) path,
    (VGfloat) x0,
    (VGfloat) y0,
    (VGfloat) x1,
    (VGfloat) y1
  );

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, errorCode, &result));
  return result;
}

napi_value openvg::vgu::Polygon(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 4);
  CheckArgs4(env, info, polygon, argc, args, number, object, number, boolean);

  uint32_t path;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &path));
  TypedArrayWrapper<VGfloat> points(env, args[1]);
  int32_t count;
  NAPI_CALL(env, napi_get_value_int32(env, args[2], &count));
  bool closed;
  NAPI_CALL(env, napi_get_value_bool(env, args[3], &closed));

  VGUErrorCode errorCode = vguPolygon(
    (VGPath) path,
    points.pointer(),
    (VGint) count,
    (VGboolean) closed
  );

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, errorCode, &result));
  return result;
}

napi_value openvg::vgu::Rect(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 5);
  CheckArgs5(env, info, rect, argc, args, number, number, number, number, number);

  uint32_t path;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &path));
  double x;
  NAPI_CALL(env, napi_get_value_double(env, args[1], &x));
  double y;
  NAPI_CALL(env, napi_get_value_double(env, args[2], &y));
  double width;
  NAPI_CALL(env, napi_get_value_double(env, args[3], &width));
  double height;
  NAPI_CALL(env, napi_get_value_double(env, args[4], &height));

  VGUErrorCode errorCode = vguRect(
    (VGPath) path,
    (VGfloat) x,
    (VGfloat) y,
    (VGfloat) width,
    (VGfloat) height
  );

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, errorCode, &result));
  return result;
}

napi_value openvg::vgu::RoundRect(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 7);
  CheckArgs7(env, info, rect, argc, args, number, number, number, number, number, number, number);

  uint32_t path;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &path));
  double x;
  NAPI_CALL(env, napi_get_value_double(env, args[1], &x));
  double y;
  NAPI_CALL(env, napi_get_value_double(env, args[2], &y));
  double width;
  NAPI_CALL(env, napi_get_value_double(env, args[3], &width));
  double height;
  NAPI_CALL(env, napi_get_value_double(env, args[4], &height));
  double arcWidth;
  NAPI_CALL(env, napi_get_value_double(env, args[5], &arcWidth));
  double arcHeight;
  NAPI_CALL(env, napi_get_value_double(env, args[6], &arcHeight));

  VGUErrorCode errorCode = vguRoundRect(
    (VGPath) path,
    (VGfloat) x,
    (VGfloat) y,
    (VGfloat) width,
    (VGfloat) height,
    (VGfloat) arcWidth,
    (VGfloat) arcHeight
  );

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, errorCode, &result));
  return result;
}

napi_value openvg::vgu::Ellipse(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 5);
  CheckArgs5(env, info, ellipse, argc, args, number, number, number, number, number);

  uint32_t path;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &path));
  double cx;
  NAPI_CALL(env, napi_get_value_double(env, args[1], &cx));
  double cy;
  NAPI_CALL(env, napi_get_value_double(env, args[2], &cy));
  double width;
  NAPI_CALL(env, napi_get_value_double(env, args[3], &width));
  double height;
  NAPI_CALL(env, napi_get_value_double(env, args[4], &height));

  VGUErrorCode errorCode = vguEllipse(
    (VGPath) path,
    (VGfloat) cx,
    (VGfloat) cy,
    (VGfloat) width,
    (VGfloat) height
  );

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, errorCode, &result));
  return result;
}

napi_value openvg::vgu::Arc(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 8);
  CheckArgs8(env, info, arc, argc, args, number, number, number, number, number, number, number, number);

  uint32_t path;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &path));
  double x;
  NAPI_CALL(env, napi_get_value_double(env, args[1], &x));
  double y;
  NAPI_CALL(env, napi_get_value_double(env, args[2], &y));
  double width;
  NAPI_CALL(env, napi_get_value_double(env, args[3], &width));
  double height;
  NAPI_CALL(env, napi_get_value_double(env, args[4], &height));
  double startAngle;
  NAPI_CALL(env, napi_get_value_double(env, args[5], &startAngle));
  double endAngle;
  NAPI_CALL(env, napi_get_value_double(env, args[6], &endAngle));
  uint32_t arcType;
  NAPI_CALL(env, napi_get_value_uint32(env, args[7], &arcType));

  VGUErrorCode errorCode = vguArc(
    (VGPath) path,
    (VGfloat) x,
    (VGfloat) y,
    (VGfloat) width,
    (VGfloat) height,
    (VGfloat) startAngle,
    (VGfloat) endAngle,
    static_cast<VGUArcType>(arcType)
  );

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, errorCode, &result));
  return result;
}

napi_value openvg::vgu::ComputeWarpQuadToSquare(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 9);
  CheckArgs9(env, info, computeWarpQuadToSquare, argc, args, number, number, number, number, number, number, number, number, object);

  double sx0;
  NAPI_CALL(env, napi_get_value_double(env, args[0], &sx0));
  double sy0;
  NAPI_CALL(env, napi_get_value_double(env, args[1], &sy0));
  double sx1;
  NAPI_CALL(env, napi_get_value_double(env, args[2], &sx1));
  double sy1;
  NAPI_CALL(env, napi_get_value_double(env, args[3], &sy1));
  double sx2;
  NAPI_CALL(env, napi_get_value_double(env, args[4], &sx2));
  double sy2;
  NAPI_CALL(env, napi_get_value_double(env, args[5], &sy2));
  double sx3;
  NAPI_CALL(env, napi_get_value_double(env, args[6], &sx3));
  double sy3;
  NAPI_CALL(env, napi_get_value_double(env, args[7], &sy3));
  TypedArrayWrapper<VGfloat> matrix(env, args[8]);

  VGUErrorCode errorCode = vguComputeWarpQuadToSquare(
    (VGfloat) sx0,
    (VGfloat) sy0,
    (VGfloat) sx1,
    (VGfloat) sy1,
    (VGfloat) sx2,
    (VGfloat) sy2,
    (VGfloat) sx3,
    (VGfloat) sy3,
    matrix.pointer()
  );

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, errorCode, &result));
  return result;
}

napi_value openvg::vgu::ComputeWarpSquareToQuad(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 9);
  CheckArgs9(env, info, computeWarpSquareToQuad, argc, args, number, number, number, number, number, number, number, number, object);

  double dx0;
  NAPI_CALL(env, napi_get_value_double(env, args[0], &dx0));
  double dy0;
  NAPI_CALL(env, napi_get_value_double(env, args[1], &dy0));
  double dx1;
  NAPI_CALL(env, napi_get_value_double(env, args[2], &dx1));
  double dy1;
  NAPI_CALL(env, napi_get_value_double(env, args[3], &dy1));
  double dx2;
  NAPI_CALL(env, napi_get_value_double(env, args[4], &dx2));
  double dy2;
  NAPI_CALL(env, napi_get_value_double(env, args[5], &dy2));
  double dx3;
  NAPI_CALL(env, napi_get_value_double(env, args[6], &dx3));
  double dy3;
  NAPI_CALL(env, napi_get_value_double(env, args[7], &dy3));
  TypedArrayWrapper<VGfloat> matrix(env, args[8]);

  VGUErrorCode errorCode = vguComputeWarpSquareToQuad(
    (VGfloat) dx0,
    (VGfloat) dy0,
    (VGfloat) dx1,
    (VGfloat) dy1,
    (VGfloat) dx2,
    (VGfloat) dy2,
    (VGfloat) dx3,
    (VGfloat) dy3,
    matrix.pointer()
  );

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, errorCode, &result));
  return result;
}

napi_value openvg::vgu::ComputeWarpQuadToQuad(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 17);
  CheckArgs17(env, info, computeWarpSquareToQuad, argc, args, number, number, number, number, number, number, number, number, number, number, number, number, number, number, number, number, object);

  double dx0;
  NAPI_CALL(env, napi_get_value_double(env, args[0], &dx0));
  double dy0;
  NAPI_CALL(env, napi_get_value_double(env, args[1], &dy0));
  double dx1;
  NAPI_CALL(env, napi_get_value_double(env, args[2], &dx1));
  double dy1;
  NAPI_CALL(env, napi_get_value_double(env, args[3], &dy1));
  double dx2;
  NAPI_CALL(env, napi_get_value_double(env, args[4], &dx2));
  double dy2;
  NAPI_CALL(env, napi_get_value_double(env, args[5], &dy2));
  double dx3;
  NAPI_CALL(env, napi_get_value_double(env, args[6], &dx3));
  double dy3;
  NAPI_CALL(env, napi_get_value_double(env, args[7], &dy3));
  double sx0;
  NAPI_CALL(env, napi_get_value_double(env, args[8], &sx0));
  double sy0;
  NAPI_CALL(env, napi_get_value_double(env, args[9], &sy0));
  double sx1;
  NAPI_CALL(env, napi_get_value_double(env, args[10], &sx1));
  double sy1;
  NAPI_CALL(env, napi_get_value_double(env, args[11], &sy1));
  double sx2;
  NAPI_CALL(env, napi_get_value_double(env, args[12], &sx2));
  double sy2;
  NAPI_CALL(env, napi_get_value_double(env, args[13], &sy2));
  double sx3;
  NAPI_CALL(env, napi_get_value_double(env, args[14], &sx3));
  double sy3;
  NAPI_CALL(env, napi_get_value_double(env, args[15], &sy3));
  TypedArrayWrapper<VGfloat> matrix(env, args[16]);

  VGUErrorCode errorCode = vguComputeWarpQuadToQuad(
    (VGfloat) dx0,
    (VGfloat) dy0,
    (VGfloat) dx1,
    (VGfloat) dy1,
    (VGfloat) dx2,
    (VGfloat) dy2,
    (VGfloat) dx3,
    (VGfloat) dy3,
    (VGfloat) dx0,
    (VGfloat) dy0,
    (VGfloat) dx1,
    (VGfloat) dy1,
    (VGfloat) dx2,
    (VGfloat) dy2,
    (VGfloat) dx3,
    (VGfloat) dy3,
    matrix.pointer()
  );

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, errorCode, &result));
  return result;
}


/* KHR extensions */


napi_value openvg::ext::CreateEGLImageTargetKHR(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, createEGLImageTargetKHR, argc, args, object);

#ifdef VG_VGEXT_PROTOTYPES
  void* image;
  NAPI_CALL(env, napi_get_value_external(env, args[0], &image));

  VGImage vgImage = vgCreateEGLImageTargetKHR((VGeglImageKHR) image);

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, vgImage, &result));
  return result;
#else
  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
#endif
}

napi_value openvg::ext::IterativeAverageBlurKHR(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 6);
  CheckArgs6(env, info, iterativeAverageBlurKHR, argc, args, number, number, number, number, number, object);

#ifdef VG_VGEXT_PROTOTYPES
  uint32_t dstImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &dstImage));
  uint32_t srcImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &srcImage));
  double dimX;
  NAPI_CALL(env, napi_get_value_double(env, args[2], &dimX));
  double dimY;
  NAPI_CALL(env, napi_get_value_double(env, args[3], &dimY));
  uint32_t iterative;
  NAPI_CALL(env, napi_get_value_uint32(env, args[4], &iterative));
  uint32_t tilingMode;
  NAPI_CALL(env, napi_get_value_uint32(env, args[5], &tilingMode));

  vgIterativeAverageBlurKHR(
    (VGImage) dstImage,
    (VGImage) srcImage,
    (VGfloat) dimX,
    (VGfloat) dimY,
    (VGuint) iterative,
    static_cast<VGTilingMode>(tilingMode)
  );
#endif
  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::ext::ParametricFilterKHR(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 9);
  CheckArgs9(env, info, iterativeAverageBlurKHR, argc, args, number, number, number, number, number, number, number, number, number);

#ifdef VG_VGEXT_PROTOTYPES
  uint32_t dstImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &dstImage));
  uint32_t srcImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &srcImage));
  uint32_t blur;
  NAPI_CALL(env, napi_get_value_uint32(env, args[2], &blur));
  double strength;
  NAPI_CALL(env, napi_get_value_double(env, args[3], &strength));
  double offsetX;
  NAPI_CALL(env, napi_get_value_double(env, args[4], &offsetX));
  double offsetY;
  NAPI_CALL(env, napi_get_value_double(env, args[5], &offsetY));
  uint32_t filterFlags;
  NAPI_CALL(env, napi_get_value_uint32(env, args[6], &filterFlags));
  uint32_t highlightPaint;
  NAPI_CALL(env, napi_get_value_uint32(env, args[7], &highlightPaint));
  uint32_t shadowPaint;
  NAPI_CALL(env, napi_get_value_uint32(env, args[8], &shadowPaint));

  vgParametricFilterKHR(
    (VGImage) dstImage,
    (VGImage) srcImage,
    (VGImage) blur,
    (VGfloat) strength,
    (VGfloat) offsetX,
    (VGfloat) offsetY,
    (VGbitfield) filterFlags,
    (VGPaint) highlightPaint,
    (VGPaint) shadowPaint
  );
#endif

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::ext::DropShadowKHR(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 11);
  CheckArgs11(env, info, dropShadowKHR, argc, args, number, number, number, number, number, number, number, number, number, number, number);

#ifdef VG_VGEXT_PROTOTYPES
  uint32_t dstImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &dstImage));
  uint32_t srcImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &srcImage));
  double dimX;
  NAPI_CALL(env, napi_get_value_double(env, args[2], &dimX));
  double dimY;
  NAPI_CALL(env, napi_get_value_double(env, args[3], &dimY));
  uint32_t iterative;
  NAPI_CALL(env, napi_get_value_uint32(env, args[4], &iterative));
  double strength;
  NAPI_CALL(env, napi_get_value_double(env, args[5], &strength));
  double distance;
  NAPI_CALL(env, napi_get_value_double(env, args[6], &distance));
  double angle;
  NAPI_CALL(env, napi_get_value_double(env, args[7], &angle));
  uint32_t filterFlags;
  NAPI_CALL(env, napi_get_value_uint32(env, args[8], &filterFlags));
  uint32_t allowedQuality;
  NAPI_CALL(env, napi_get_value_uint32(env, args[9], &allowedQuality));
  uint32_t shadowColorRGBA;
  NAPI_CALL(env, napi_get_value_uint32(env, args[10], &shadowColorRGBA));

  VGUErrorCode errorCode = vguDropShadowKHR(
    (VGImage) dstImage,
    (VGImage) srcImage,
    (VGfloat) dimX,
    (VGfloat) dimY,
    (VGuint) iterative,
    (VGfloat) strength,
    (VGfloat) distance,
    (VGfloat) angle,
    (VGbitfield) filterFlags,
    (VGbitfield) allowedQuality,
    (VGuint) shadowColorRGBA
  );

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, errorCode, &result));
  return result;
#else
  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
#endif
}

napi_value openvg::ext::GlowKHR(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 9);
  CheckArgs9(env, info, glowKHR, argc, args, number, number, number, number, number, number, number, number, number);

#ifdef VG_VGEXT_PROTOTYPES
  uint32_t dstImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &dstImage));
  uint32_t srcImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &srcImage));
  double dimX;
  NAPI_CALL(env, napi_get_value_double(env, args[2], &dimX));
  double dimY;
  NAPI_CALL(env, napi_get_value_double(env, args[3], &dimY));
  uint32_t iterative;
  NAPI_CALL(env, napi_get_value_uint32(env, args[4], &iterative));
  double strength;
  NAPI_CALL(env, napi_get_value_double(env, args[5], &strength));
  uint32_t filterFlags;
  NAPI_CALL(env, napi_get_value_uint32(env, args[6], &filterFlags));
  uint32_t allowedQuality;
  NAPI_CALL(env, napi_get_value_uint32(env, args[7], &allowedQuality));
  uint32_t shadowColorRGBA;
  NAPI_CALL(env, napi_get_value_uint32(env, args[8], &shadowColorRGBA));

  VGUErrorCode errorCode = vguGlowKHR(
    (VGImage) dstImage,
    (VGImage) srcImage,
    (VGfloat) dimX,
    (VGfloat) dimY,
    (VGuint) iterative,
    (VGfloat) strength,
    (VGbitfield) filterFlags,
    (VGbitfield) allowedQuality,
    (VGuint) shadowColorRGBA
  );

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, errorCode, &result));
  return result;
#else
  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
#endif
}

napi_value openvg::ext::BevelKHR(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 12);
  CheckArgs12(env, info, bevelKHR, argc, args, number, number, number, number, number, number, number, number, number, number, number, number);

#ifdef VG_VGEXT_PROTOTYPES
  uint32_t dstImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &dstImage));
  uint32_t srcImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &srcImage));
  double dimX;
  NAPI_CALL(env, napi_get_value_double(env, args[2], &dimX));
  double dimY;
  NAPI_CALL(env, napi_get_value_double(env, args[3], &dimY));
  uint32_t iterative;
  NAPI_CALL(env, napi_get_value_uint32(env, args[4], &iterative));
  double strength;
  NAPI_CALL(env, napi_get_value_double(env, args[5], &strength));
  double distance;
  NAPI_CALL(env, napi_get_value_double(env, args[6], &distance));
  double angle;
  NAPI_CALL(env, napi_get_value_double(env, args[7], &angle));
  uint32_t filterFlags;
  NAPI_CALL(env, napi_get_value_uint32(env, args[8], &filterFlags));
  uint32_t allowedQuality;
  NAPI_CALL(env, napi_get_value_uint32(env, args[9], &allowedQuality));
  uint32_t highlightColorRGBA;
  NAPI_CALL(env, napi_get_value_uint32(env, args[10], &highlightColorRGBA));
  uint32_t shadowColorRGBA;
  NAPI_CALL(env, napi_get_value_uint32(env, args[11], &shadowColorRGBA));

  VGUErrorCode errorCode = vguBevelKHR(
    (VGImage) dstImage,
    (VGImage) srcImage,
    (VGfloat) dimX,
    (VGfloat) dimY,
    (VGuint) iterative,
    (VGfloat) strength,
    (VGfloat) distance,
    (VGfloat) angle,
    (VGbitfield) filterFlags,
    (VGbitfield) allowedQuality,
    (VGuint) highlightColorRGBA,
    (VGuint) shadowColorRGBA
  );

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, errorCode, &result));
  return result;
#else
  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
#endif
}

napi_value openvg::ext::GradientGlowKHR(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 12);
  CheckArgs12(env, info, gradientGlowKHR, argc, args, number, number, number, number, number, number, number, number, number, number, number, object);

#ifdef VG_VGEXT_PROTOTYPES
  uint32_t dstImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &dstImage));
  uint32_t srcImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &srcImage));
  double dimX;
  NAPI_CALL(env, napi_get_value_double(env, args[2], &dimX));
  double dimY;
  NAPI_CALL(env, napi_get_value_double(env, args[3], &dimY));
  uint32_t iterative;
  NAPI_CALL(env, napi_get_value_uint32(env, args[4], &iterative));
  double strength;
  NAPI_CALL(env, napi_get_value_double(env, args[5], &strength));
  double distance;
  NAPI_CALL(env, napi_get_value_double(env, args[6], &distance));
  double angle;
  NAPI_CALL(env, napi_get_value_double(env, args[7], &angle));
  uint32_t filterFlags;
  NAPI_CALL(env, napi_get_value_uint32(env, args[8], &filterFlags));
  uint32_t allowedQuality;
  NAPI_CALL(env, napi_get_value_uint32(env, args[9], &allowedQuality));
  uint32_t stopsCount;
  NAPI_CALL(env, napi_get_value_uint32(env, args[10], &stopsCount));
  TypedArrayWrapper<VGfloat> glowColorRampStops(env, args[11]);

  VGUErrorCode errorCode = vguGradientGlowKHR(
    (VGImage) dstImage,
    (VGImage) srcImage,
    (VGfloat) dimX,
    (VGfloat) dimY,
    (VGuint) iterative,
    (VGfloat) strength,
    (VGfloat) distance,
    (VGfloat) angle,
    (VGbitfield) filterFlags,
    (VGbitfield) allowedQuality,
    (VGuint) stopsCount,
    glowColorRampStops.pointer()
  );

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, errorCode, &result));
  return result;
#else
  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
#endif
}

napi_value openvg::ext::GradientBevelKHR(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 12);
  CheckArgs12(env, info, gradientBevelKHR, argc, args, number, number, number, number, number, number, number, number, number, number, number, object);

#ifdef VG_VGEXT_PROTOTYPES
  uint32_t dstImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[0], &dstImage));
  uint32_t srcImage;
  NAPI_CALL(env, napi_get_value_uint32(env, args[1], &srcImage));
  double dimX;
  NAPI_CALL(env, napi_get_value_double(env, args[2], &dimX));
  double dimY;
  NAPI_CALL(env, napi_get_value_double(env, args[3], &dimY));
  uint32_t iterative;
  NAPI_CALL(env, napi_get_value_uint32(env, args[4], &iterative));
  double strength;
  NAPI_CALL(env, napi_get_value_double(env, args[5], &strength));
  double distance;
  NAPI_CALL(env, napi_get_value_double(env, args[6], &distance));
  double angle;
  NAPI_CALL(env, napi_get_value_double(env, args[7], &angle));
  uint32_t filterFlags;
  NAPI_CALL(env, napi_get_value_uint32(env, args[8], &filterFlags));
  uint32_t allowedQuality;
  NAPI_CALL(env, napi_get_value_uint32(env, args[9], &allowedQuality));
  uint32_t stopsCount;
  NAPI_CALL(env, napi_get_value_uint32(env, args[10], &stopsCount));
  TypedArrayWrapper<VGfloat> bevelColorRampStops(env, args[11]);

  VGUErrorCode errorCode = vguGradientBevelKHR(
    (VGImage) dstImage,
    (VGImage) srcImage,
    (VGfloat) dimX,
    (VGfloat) dimY,
    (VGuint) iterative,
    (VGfloat) strength,
    (VGfloat) distance,
    (VGfloat) angle,
    (VGbitfield) filterFlags,
    (VGbitfield) allowedQuality,
    (VGuint) stopsCount,
    bevelColorRampStops.pointer()
  );

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, errorCode, &result));
  return result;
#else
  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
#endif
}

napi_value openvg::ext::ProjectiveMatrixNDS(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 1);
  CheckArgs1(env, info, projectiveMatrixNDS, argc, args, boolean);

#ifdef VG_VGEXT_PROTOTYPES
  bool enable;
  NAPI_CALL(env, napi_get_value_bool(env, args[0], &enable));

  vgProjectiveMatrixNDS((VGboolean) enable);
#endif

  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
}

napi_value openvg::ext::TransformClipLineNDS(napi_env env, napi_callback_info info) {
  DeclareArgs(argc, args, 8);
  CheckArgs8(env, info, gradientBevelKHR, argc, args, number, number, number, object, boolean, object, object, object);

#ifdef VG_VGEXT_PROTOTYPES
  double Ain;
  NAPI_CALL(env, napi_get_value_double(env, args[0], &Ain));
  double Bin;
  NAPI_CALL(env, napi_get_value_double(env, args[1], &Bin));
  double Cin;
  NAPI_CALL(env, napi_get_value_double(env, args[2], &Cin));
  TypedArrayWrapper<VGfloat> matrix(env, args[3]);
  bool inverse;
  NAPI_CALL(env, napi_get_value_bool(env, args[4], &inverse));
  TypedArrayWrapper<VGfloat> Aout(env, args[5]);
  TypedArrayWrapper<VGfloat> Bout(env, args[6]);
  TypedArrayWrapper<VGfloat> Cout(env, args[7]);

  VGUErrorCode errorCode = vguTransformClipLineNDS(
    (VGfloat) Ain,
    (VGfloat) Bin,
    (VGfloat) Cin,
    matrix.pointer(),
    (VGboolean) inverse,
    Aout.pointer(),
    Bout.pointer(),
    Cout.pointer()
  );

  napi_value result;
  NAPI_CALL(env, napi_create_number(env, errorCode, &result));
  return result;
#else
  napi_value result;
  NAPI_CALL(env, napi_get_undefined(env, &result));
  return result;
#endif
}
