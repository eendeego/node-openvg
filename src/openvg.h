#ifndef NODE_OPENVG_H_
#define NODE_OPENVG_H_

#include <node.h>
#include <v8.h>
#include <node_api.h>
#include "EGL/egl.h"

#include "node-common.h"

using namespace v8;

namespace openvg {

napi_value NativeInit(napi_env env, napi_callback_info info);
napi_value NativeDeinit(napi_env env, napi_callback_info info);

napi_value GetError(napi_env env, napi_callback_info info);

napi_value Flush(napi_env env, napi_callback_info info);
napi_value Finish(napi_env env, napi_callback_info info);

/* Getters and Setters */
napi_value SetF(napi_env env, napi_callback_info info);
napi_value SetI(napi_env env, napi_callback_info info);
napi_value SetFV(napi_env env, napi_callback_info info);
napi_value SetIV(napi_env env, napi_callback_info info);
napi_value SetFVOL(napi_env env, napi_callback_info info);
napi_value SetIVOL(napi_env env, napi_callback_info info);

napi_value GetF(napi_env env, napi_callback_info info);
napi_value GetI(napi_env env, napi_callback_info info);
napi_value GetVectorSize(napi_env env, napi_callback_info info);
napi_value GetFV(napi_env env, napi_callback_info info);
napi_value GetIV(napi_env env, napi_callback_info info);
napi_value GetFVOL(napi_env env, napi_callback_info info);
napi_value GetIVOL(napi_env env, napi_callback_info info);

napi_value SetParameterF(napi_env env, napi_callback_info info);
napi_value SetParameterI(napi_env env, napi_callback_info info);
napi_value SetParameterFV(napi_env env, napi_callback_info info);
napi_value SetParameterIV(napi_env env, napi_callback_info info);
napi_value SetParameterFVOL(napi_env env, napi_callback_info info);
napi_value SetParameterIVOL(napi_env env, napi_callback_info info);

napi_value GetParameterF(napi_env env, napi_callback_info info);
napi_value GetParameterI(napi_env env, napi_callback_info info);
napi_value GetParameterVectorSize(napi_env env, napi_callback_info info);
napi_value GetParameterFV(napi_env env, napi_callback_info info);
napi_value GetParameterIV(napi_env env, napi_callback_info info);
napi_value GetParameterFVOL(napi_env env, napi_callback_info info);
napi_value GetParameterIVOL(napi_env env, napi_callback_info info);

/* Matrix Manipulation */
napi_value LoadIdentity(napi_env env, napi_callback_info info);
napi_value LoadMatrix(napi_env env, napi_callback_info info);
napi_value GetMatrix(napi_env env, napi_callback_info info);
napi_value MultMatrix(napi_env env, napi_callback_info info);
napi_value Translate(napi_env env, napi_callback_info info);
napi_value Scale(napi_env env, napi_callback_info info);
napi_value Shear(napi_env env, napi_callback_info info);
napi_value Rotate(napi_env env, napi_callback_info info);

/* Masking and Clearing */
napi_value Mask(napi_env env, napi_callback_info info);
napi_value RenderToMask(napi_env env, napi_callback_info info);
napi_value CreateMaskLayer(napi_env env, napi_callback_info info);
napi_value DestroyMaskLayer(napi_env env, napi_callback_info info);
napi_value FillMaskLayer(napi_env env, napi_callback_info info);
napi_value CopyMask(napi_env env, napi_callback_info info);
napi_value Clear(napi_env env, napi_callback_info info);

/* Paths */
napi_value CreatePath(napi_env env, napi_callback_info info);
napi_value ClearPath(napi_env env, napi_callback_info info);
napi_value DestroyPath(napi_env env, napi_callback_info info);
napi_value RemovePathCapabilities(napi_env env, napi_callback_info info);
napi_value GetPathCapabilities(napi_env env, napi_callback_info info);
napi_value AppendPath(napi_env env, napi_callback_info info);
napi_value AppendPathData(napi_env env, napi_callback_info info);
napi_value AppendPathDataO(napi_env env, napi_callback_info info);
napi_value ModifyPathCoords(napi_env env, napi_callback_info info);
napi_value TransformPath(napi_env env, napi_callback_info info);
napi_value InterpolatePath(napi_env env, napi_callback_info info);
napi_value PathLength(napi_env env, napi_callback_info info);
napi_value PointAlongPath(napi_env env, napi_callback_info info);
napi_value PathBounds(napi_env env, napi_callback_info info);
napi_value PathTransformedBounds(napi_env env, napi_callback_info info);
napi_value DrawPath(napi_env env, napi_callback_info info);

/* Paint */
napi_value CreatePaint(napi_env env, napi_callback_info info);
napi_value DestroyPaint(napi_env env, napi_callback_info info);
napi_value SetPaint(napi_env env, napi_callback_info info);
napi_value GetPaint(napi_env env, napi_callback_info info);
napi_value SetColor(napi_env env, napi_callback_info info);
napi_value GetColor(napi_env env, napi_callback_info info);
napi_value PaintPattern(napi_env env, napi_callback_info info);

/* Images */
napi_value CreateImage(napi_env env, napi_callback_info info);
napi_value DestroyImage(napi_env env, napi_callback_info info);
napi_value ClearImage(napi_env env, napi_callback_info info);
napi_value ImageSubData(napi_env env, napi_callback_info info);
napi_value GetImageSubData(napi_env env, napi_callback_info info);
napi_value ChildImage(napi_env env, napi_callback_info info);
napi_value GetParent(napi_env env, napi_callback_info info);
napi_value CopyImage(napi_env env, napi_callback_info info);
napi_value DrawImage(napi_env env, napi_callback_info info);
napi_value SetPixels(napi_env env, napi_callback_info info);
napi_value WritePixels(napi_env env, napi_callback_info info);
napi_value GetPixels(napi_env env, napi_callback_info info);
napi_value ReadPixels(napi_env env, napi_callback_info info);
napi_value CopyPixels(napi_env env, napi_callback_info info);

/* Text */
napi_value CreateFont(napi_env env, napi_callback_info info);
napi_value DestroyFont(napi_env env, napi_callback_info info);
napi_value SetGlyphToPath(napi_env env, napi_callback_info info);
napi_value SetGlyphToImage(napi_env env, napi_callback_info info);
napi_value ClearGlyph(napi_env env, napi_callback_info info);
napi_value DrawGlyph(napi_env env, napi_callback_info info);
napi_value DrawGlyphs(napi_env env, napi_callback_info info);

/* Image Filters */
napi_value ColorMatrix(napi_env env, napi_callback_info info);
napi_value Convolve(napi_env env, napi_callback_info info);
napi_value SeparableConvolve(napi_env env, napi_callback_info info);
napi_value GaussianBlur(napi_env env, napi_callback_info info);
napi_value Lookup(napi_env env, napi_callback_info info);
napi_value LookupSingle(napi_env env, napi_callback_info info);

/* Hardware Queries */
napi_value HardwareQuery(napi_env env, napi_callback_info info);

/* Renderer and Extension Information */
napi_value GetString(napi_env env, napi_callback_info info);

namespace vgu {

napi_value Line(napi_env env, napi_callback_info info);
napi_value Polygon(napi_env env, napi_callback_info info);
napi_value Rect(napi_env env, napi_callback_info info);
napi_value RoundRect(napi_env env, napi_callback_info info);
napi_value Ellipse(napi_env env, napi_callback_info info);
napi_value Arc(napi_env env, napi_callback_info info);
napi_value ComputeWarpQuadToSquare(napi_env env, napi_callback_info info);
napi_value ComputeWarpSquareToQuad(napi_env env, napi_callback_info info);
napi_value ComputeWarpQuadToQuad(napi_env env, napi_callback_info info);

}

namespace ext {

napi_value CreateEGLImageTargetKHR(napi_env env, napi_callback_info info);

napi_value IterativeAverageBlurKHR(napi_env env, napi_callback_info info);

napi_value ParametricFilterKHR(napi_env env, napi_callback_info info);
napi_value DropShadowKHR(napi_env env, napi_callback_info info);
napi_value GlowKHR(napi_env env, napi_callback_info info);
napi_value BevelKHR(napi_env env, napi_callback_info info);
napi_value GradientGlowKHR(napi_env env, napi_callback_info info);
napi_value GradientBevelKHR(napi_env env, napi_callback_info info);

napi_value ProjectiveMatrixNDS(napi_env env, napi_callback_info info);
napi_value TransformClipLineNDS(napi_env env, napi_callback_info info);

}

}

#endif
