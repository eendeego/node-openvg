#ifndef NODE_OPENVG_H_
#define NODE_OPENVG_H_

#include <node.h>
#include <v8.h>
#include "EGL/egl.h"

using namespace v8;

namespace openvg {

NAN_METHOD(StartUp);
NAN_METHOD(Shutdown);

NAN_METHOD(GetError);

NAN_METHOD(Flush);
NAN_METHOD(Finish);

/* Getters and Setters */
NAN_METHOD(SetF);
NAN_METHOD(SetI);
NAN_METHOD(SetFV);
NAN_METHOD(SetIV);
NAN_METHOD(SetFVOL);
NAN_METHOD(SetIVOL);

NAN_METHOD(GetF);
NAN_METHOD(GetI);
NAN_METHOD(GetVectorSize);
NAN_METHOD(GetFV);
NAN_METHOD(GetIV);
NAN_METHOD(GetFVOL);
NAN_METHOD(GetIVOL);

NAN_METHOD(SetParameterF);
NAN_METHOD(SetParameterI);
NAN_METHOD(SetParameterFV);
NAN_METHOD(SetParameterIV);
NAN_METHOD(SetParameterFVOL);
NAN_METHOD(SetParameterIVOL);

NAN_METHOD(GetParameterF);
NAN_METHOD(GetParameterI);
NAN_METHOD(GetParameterVectorSize);
NAN_METHOD(GetParameterFV);
NAN_METHOD(GetParameterIV);
NAN_METHOD(GetParameterFVOL);
NAN_METHOD(GetParameterIVOL);

/* Matrix Manipulation */
NAN_METHOD(LoadIdentity);
NAN_METHOD(LoadMatrix);
NAN_METHOD(GetMatrix);
NAN_METHOD(MultMatrix);
NAN_METHOD(Translate);
NAN_METHOD(Scale);
NAN_METHOD(Shear);
NAN_METHOD(Rotate);

/* Masking and Clearing */
NAN_METHOD(Mask);
NAN_METHOD(RenderToMask);
NAN_METHOD(CreateMaskLayer);
NAN_METHOD(DestroyMaskLayer);
NAN_METHOD(FillMaskLayer);
NAN_METHOD(CopyMask);
NAN_METHOD(Clear);

/* Paths */
NAN_METHOD(CreatePath);
NAN_METHOD(ClearPath);
NAN_METHOD(DestroyPath);
NAN_METHOD(RemovePathCapabilities);
NAN_METHOD(GetPathCapabilities);
NAN_METHOD(AppendPath);
NAN_METHOD(AppendPathData);
NAN_METHOD(AppendPathDataO);
NAN_METHOD(ModifyPathCoords);
NAN_METHOD(TransformPath);
NAN_METHOD(InterpolatePath);
NAN_METHOD(PathLength);
NAN_METHOD(PointAlongPath);
NAN_METHOD(PathBounds);
NAN_METHOD(PathTransformedBounds);
NAN_METHOD(DrawPath);

/* Paint */
NAN_METHOD(CreatePaint);
NAN_METHOD(DestroyPaint);
NAN_METHOD(SetPaint);
NAN_METHOD(GetPaint);
NAN_METHOD(SetColor);
NAN_METHOD(GetColor);
NAN_METHOD(PaintPattern);

/* Images */
NAN_METHOD(CreateImage);
NAN_METHOD(DestroyImage);
NAN_METHOD(ClearImage);
NAN_METHOD(ImageSubData);
NAN_METHOD(GetImageSubData);
NAN_METHOD(ChildImage);
NAN_METHOD(GetParent);
NAN_METHOD(CopyImage);
NAN_METHOD(DrawImage);
NAN_METHOD(SetPixels);
NAN_METHOD(WritePixels);
NAN_METHOD(GetPixels);
NAN_METHOD(ReadPixels);
NAN_METHOD(CopyPixels);

/* Text */
NAN_METHOD(CreateFont);
NAN_METHOD(DestroyFont);
NAN_METHOD(SetGlyphToPath);
NAN_METHOD(SetGlyphToImage);
NAN_METHOD(ClearGlyph);
NAN_METHOD(DrawGlyph);
NAN_METHOD(DrawGlyphs);

/* Image Filters */
NAN_METHOD(ColorMatrix);
NAN_METHOD(Convolve);
NAN_METHOD(SeparableConvolve);
NAN_METHOD(GaussianBlur);
NAN_METHOD(Lookup);
NAN_METHOD(LookupSingle);

/* Hardware Queries */
NAN_METHOD(HardwareQuery);

/* Renderer and Extension Information */
NAN_METHOD(GetString);

namespace vgu {

NAN_METHOD(Line);
NAN_METHOD(Polygon);
NAN_METHOD(Rect);
NAN_METHOD(RoundRect);
NAN_METHOD(Ellipse);
NAN_METHOD(Arc);
NAN_METHOD(ComputeWarpQuadToSquare);
NAN_METHOD(ComputeWarpSquareToQuad);
NAN_METHOD(ComputeWarpQuadToQuad);

}

namespace ext {

NAN_METHOD(CreateEGLImageTargetKHR);

NAN_METHOD(IterativeAverageBlurKHR);

NAN_METHOD(ParametricFilterKHR);
NAN_METHOD(DropShadowKHR);
NAN_METHOD(GlowKHR);
NAN_METHOD(BevelKHR);
NAN_METHOD(GradientGlowKHR);
NAN_METHOD(GradientBevelKHR);

NAN_METHOD(ProjectiveMatrixNDS);
NAN_METHOD(TransformClipLineNDS);

}

}

#endif
