#ifndef NODE_OPENVG_H_
#define NODE_OPENVG_H_

#include <node.h>
#include <v8.h>
#include "EGL/egl.h"

using namespace v8;

namespace openvg {
  static Handle<Value> StartUp(const Arguments& args);
  static Handle<Value> Shutdown(const Arguments& args);

  static Handle<Value> GetError(const Arguments& args);

  static Handle<Value> Flush(const Arguments& args);
  static Handle<Value> Finish(const Arguments& args);

  /* Getters and Setters */
  static Handle<Value> SetF(const Arguments& args);
  static Handle<Value> SetI(const Arguments& args);
  static Handle<Value> SetFV(const Arguments& args);
  static Handle<Value> SetIV(const Arguments& args);

  static Handle<Value> GetF(const Arguments& args);
  static Handle<Value> GetI(const Arguments& args);
  static Handle<Value> GetVectorSize(const Arguments& args);
  static Handle<Value> GetFV(const Arguments& args);
  static Handle<Value> GetIV(const Arguments& args);

  static Handle<Value> SetParameterF(const Arguments& args);
  static Handle<Value> SetParameterI(const Arguments& args);
  static Handle<Value> SetParameterFV(const Arguments& args);
  static Handle<Value> SetParameterIV(const Arguments& args);

  static Handle<Value> GetParameterF(const Arguments& args);
  static Handle<Value> GetParameterI(const Arguments& args);
  static Handle<Value> GetParameterVectorSize(const Arguments& args);
  static Handle<Value> GetParameterFV(const Arguments& args);
  static Handle<Value> GetParameterIV(const Arguments& args);

  /* Matrix Manipulation */
  static Handle<Value> LoadIdentity(const Arguments& args);
  static Handle<Value> LoadMatrix(const Arguments& args);
  static Handle<Value> GetMatrix(const Arguments& args);
  static Handle<Value> MultMatrix(const Arguments& args);
  static Handle<Value> Translate(const Arguments& args);
  static Handle<Value> Scale(const Arguments& args);
  static Handle<Value> Shear(const Arguments& args);
  static Handle<Value> Rotate(const Arguments& args);

  /* Masking and Clearing */
  static Handle<Value> Mask(const Arguments& args);
  static Handle<Value> RenderToMask(const Arguments& args);
  static Handle<Value> CreateMaskLayer(const Arguments& args);
  static Handle<Value> DestroyMaskLayer(const Arguments& args);
  static Handle<Value> FillMaskLayer(const Arguments& args);
  static Handle<Value> CopyMask(const Arguments& args);
  static Handle<Value> Clear(const Arguments& args);

  /* Paths */
  static Handle<Value> CreatePath(const Arguments& args);
  static Handle<Value> ClearPath(const Arguments& args);
  static Handle<Value> DestroyPath(const Arguments& args);
  static Handle<Value> RemovePathCapabilities(const Arguments& args);
  static Handle<Value> GetPathCapabilities(const Arguments& args);
  static Handle<Value> AppendPath(const Arguments& args);
  static Handle<Value> AppendPathData(const Arguments& args);
  static Handle<Value> ModifyPathCoords(const Arguments& args);
  static Handle<Value> TransformPath(const Arguments& args);
  static Handle<Value> InterpolatePath(const Arguments& args);
  static Handle<Value> PathLength(const Arguments& args);
  static Handle<Value> PointAlongPath(const Arguments& args);
  static Handle<Value> PathBounds(const Arguments& args);
  static Handle<Value> PathTransformedBounds(const Arguments& args);
  static Handle<Value> DrawPath(const Arguments& args);

  /* Paint */
  static Handle<Value> CreatePaint(const Arguments& args);
  static Handle<Value> DestroyPaint(const Arguments& args);
  static Handle<Value> SetPaint(const Arguments& args);
  static Handle<Value> GetPaint(const Arguments& args);
  static Handle<Value> SetColor(const Arguments& args);
  static Handle<Value> GetColor(const Arguments& args);
  static Handle<Value> PaintPattern(const Arguments& args);

  /* Images */
  static Handle<Value> CreateImage(const Arguments& args);
  static Handle<Value> DestroyImage(const Arguments& args);
  static Handle<Value> ClearImage(const Arguments& args);
  static Handle<Value> ImageSubData(const Arguments& args);
  static Handle<Value> GetImageSubData(const Arguments& args);
  static Handle<Value> ChildImage(const Arguments& args);
  static Handle<Value> GetParent(const Arguments& args);
  static Handle<Value> CopyImage(const Arguments& args);
  static Handle<Value> DrawImage(const Arguments& args);
  static Handle<Value> SetPixels(const Arguments& args);
  static Handle<Value> WritePixels(const Arguments& args);
  static Handle<Value> GetPixels(const Arguments& args);
  static Handle<Value> ReadPixels(const Arguments& args);
  static Handle<Value> CopyPixels(const Arguments& args);

  /* Text */
  static Handle<Value> CreateFont(const Arguments& args);
  static Handle<Value> DestroyFont(const Arguments& args);
  static Handle<Value> SetGlyphToPath(const Arguments& args);
  static Handle<Value> SetGlyphToImage(const Arguments& args);
  static Handle<Value> ClearGlyph(const Arguments& args);
  static Handle<Value> DrawGlyph(const Arguments& args);
  static Handle<Value> DrawGlyphs(const Arguments& args);

  /* Image Filters */
  static Handle<Value> ColorMatrix(const Arguments& args);
  static Handle<Value> Convolve(const Arguments& args);
  static Handle<Value> SeparableConvolve(const Arguments& args);
  static Handle<Value> GaussianBlur(const Arguments& args);
  static Handle<Value> Lookup(const Arguments& args);
  static Handle<Value> LookupSingle(const Arguments& args);

  /* Hardware Queries */
  static Handle<Value> HardwareQuery(const Arguments& args);

  /* Renderer and Extension Information */
  static Handle<Value> GetString(const Arguments& args);

  namespace vgu {
    static Handle<Value> Line(const Arguments& args);
    static Handle<Value> Polygon(const Arguments& args);
    static Handle<Value> Rect(const Arguments& args);
    static Handle<Value> RoundRect(const Arguments& args);
    static Handle<Value> Ellipse(const Arguments& args);
    static Handle<Value> Arc(const Arguments& args);
    static Handle<Value> ComputeWarpQuadToSquare(const Arguments& args);
    static Handle<Value> ComputeWarpSquareToQuad(const Arguments& args);
    static Handle<Value> ComputeWarpQuadToQuad(const Arguments& args);
  }

  static Handle<Value> End(const Arguments& args);
  static Handle<Value> TextMiddle(const Arguments& args);
}

#endif
