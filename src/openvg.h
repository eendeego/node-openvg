#ifndef NODE_OPENVG_H_
#define NODE_OPENVG_H_

#include <node.h>
#include <v8.h>
#include "EGL/egl.h"

#include "v8_helpers.h"

using namespace v8;

namespace openvg {

V8_METHOD_DECL(StartUp);
V8_METHOD_DECL(Shutdown);

V8_METHOD_DECL(GetError);

V8_METHOD_DECL(Flush);
V8_METHOD_DECL(Finish);

/* Getters and Setters */
V8_METHOD_DECL(SetF);
V8_METHOD_DECL(SetI);
V8_METHOD_DECL(SetFV);
V8_METHOD_DECL(SetIV);
V8_METHOD_DECL(SetFVOL);
V8_METHOD_DECL(SetIVOL);

V8_METHOD_DECL(GetF);
V8_METHOD_DECL(GetI);
V8_METHOD_DECL(GetVectorSize);
V8_METHOD_DECL(GetFV);
V8_METHOD_DECL(GetIV);
V8_METHOD_DECL(GetFVOL);
V8_METHOD_DECL(GetIVOL);

V8_METHOD_DECL(SetParameterF);
V8_METHOD_DECL(SetParameterI);
V8_METHOD_DECL(SetParameterFV);
V8_METHOD_DECL(SetParameterIV);
V8_METHOD_DECL(SetParameterFVOL);
V8_METHOD_DECL(SetParameterIVOL);

V8_METHOD_DECL(GetParameterF);
V8_METHOD_DECL(GetParameterI);
V8_METHOD_DECL(GetParameterVectorSize);
V8_METHOD_DECL(GetParameterFV);
V8_METHOD_DECL(GetParameterIV);
V8_METHOD_DECL(GetParameterFVOL);
V8_METHOD_DECL(GetParameterIVOL);

/* Matrix Manipulation */
V8_METHOD_DECL(LoadIdentity);
V8_METHOD_DECL(LoadMatrix);
V8_METHOD_DECL(GetMatrix);
V8_METHOD_DECL(MultMatrix);
V8_METHOD_DECL(Translate);
V8_METHOD_DECL(Scale);
V8_METHOD_DECL(Shear);
V8_METHOD_DECL(Rotate);

/* Masking and Clearing */
V8_METHOD_DECL(Mask);
V8_METHOD_DECL(RenderToMask);
V8_METHOD_DECL(CreateMaskLayer);
V8_METHOD_DECL(DestroyMaskLayer);
V8_METHOD_DECL(FillMaskLayer);
V8_METHOD_DECL(CopyMask);
V8_METHOD_DECL(Clear);

/* Paths */
V8_METHOD_DECL(CreatePath);
V8_METHOD_DECL(ClearPath);
V8_METHOD_DECL(DestroyPath);
V8_METHOD_DECL(RemovePathCapabilities);
V8_METHOD_DECL(GetPathCapabilities);
V8_METHOD_DECL(AppendPath);
V8_METHOD_DECL(AppendPathData);
V8_METHOD_DECL(AppendPathDataO);
V8_METHOD_DECL(ModifyPathCoords);
V8_METHOD_DECL(TransformPath);
V8_METHOD_DECL(InterpolatePath);
V8_METHOD_DECL(PathLength);
V8_METHOD_DECL(PointAlongPath);
V8_METHOD_DECL(PathBounds);
V8_METHOD_DECL(PathTransformedBounds);
V8_METHOD_DECL(DrawPath);

/* Paint */
V8_METHOD_DECL(CreatePaint);
V8_METHOD_DECL(DestroyPaint);
V8_METHOD_DECL(SetPaint);
V8_METHOD_DECL(GetPaint);
V8_METHOD_DECL(SetColor);
V8_METHOD_DECL(GetColor);
V8_METHOD_DECL(PaintPattern);

/* Images */
V8_METHOD_DECL(CreateImage);
V8_METHOD_DECL(DestroyImage);
V8_METHOD_DECL(ClearImage);
V8_METHOD_DECL(ImageSubData);
V8_METHOD_DECL(GetImageSubData);
V8_METHOD_DECL(ChildImage);
V8_METHOD_DECL(GetParent);
V8_METHOD_DECL(CopyImage);
V8_METHOD_DECL(DrawImage);
V8_METHOD_DECL(SetPixels);
V8_METHOD_DECL(WritePixels);
V8_METHOD_DECL(GetPixels);
V8_METHOD_DECL(ReadPixels);
V8_METHOD_DECL(CopyPixels);

/* Text */
V8_METHOD_DECL(CreateFont);
V8_METHOD_DECL(DestroyFont);
V8_METHOD_DECL(SetGlyphToPath);
V8_METHOD_DECL(SetGlyphToImage);
V8_METHOD_DECL(ClearGlyph);
V8_METHOD_DECL(DrawGlyph);
V8_METHOD_DECL(DrawGlyphs);

/* Image Filters */
V8_METHOD_DECL(ColorMatrix);
V8_METHOD_DECL(Convolve);
V8_METHOD_DECL(SeparableConvolve);
V8_METHOD_DECL(GaussianBlur);
V8_METHOD_DECL(Lookup);
V8_METHOD_DECL(LookupSingle);

/* Hardware Queries */
V8_METHOD_DECL(HardwareQuery);

/* Renderer and Extension Information */
V8_METHOD_DECL(GetString);

namespace vgu {

V8_METHOD_DECL(Line);
V8_METHOD_DECL(Polygon);
V8_METHOD_DECL(Rect);
V8_METHOD_DECL(RoundRect);
V8_METHOD_DECL(Ellipse);
V8_METHOD_DECL(Arc);
V8_METHOD_DECL(ComputeWarpQuadToSquare);
V8_METHOD_DECL(ComputeWarpSquareToQuad);
V8_METHOD_DECL(ComputeWarpQuadToQuad);

}

namespace ext {

V8_METHOD_DECL(CreateEGLImageTargetKHR);

V8_METHOD_DECL(IterativeAverageBlurKHR);

V8_METHOD_DECL(ParametricFilterKHR);
V8_METHOD_DECL(DropShadowKHR);
V8_METHOD_DECL(GlowKHR);
V8_METHOD_DECL(BevelKHR);
V8_METHOD_DECL(GradientGlowKHR);
V8_METHOD_DECL(GradientBevelKHR);

V8_METHOD_DECL(ProjectiveMatrixNDS);
V8_METHOD_DECL(TransformClipLineNDS);

}

}

#endif
