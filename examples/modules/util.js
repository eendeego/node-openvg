// Utility functions shared by examples
//
// Based on https://github.com/ajstarks/openvg libshapes.c/go
//

var util = module.exports;

var openVG = require('../../openvg');
var text = require('./text');

var sansTypeface     = util.sansTypeface     = undefined;
var serifTypeface    = util.serifTypeface    = undefined;
var sansMonoTypeface = util.sansMonoTypeface = undefined;

var start = util.start = function() {
  var color = new Float32Array([255, 255, 255, 1]);
  openVG.setFVOL(openVG.VGParamType.VG_CLEAR_COLOR, color, 0, 4);
  openVG.clear(0, 0, openVG.screen.width, openVG.screen.height);
  color[0] = 0, color[1] = 0, color[2] = 0;
  setFill(color);
  setStroke(color);
  strokeWidth(0);
  openVG.loadIdentity();
}

var end = util.end = function() {
  openVG.egl.swapBuffers(openVG.screen.display, openVG.screen.surface);
}

var init = util.init = function(options) {
  if (options === undefined) {
    options = {};
  }
  if (options.loadFonts === undefined) { options.loadFonts = true; }

  openVG.init();

  if(options.loadFonts) {
    util.sansTypeface     = text.loadFont("examples/fonts/sans.json");
    util.serifTypeface    = text.loadFont("examples/fonts/serif.json");
    util.sansMonoTypeface = text.loadFont("examples/fonts/sans-mono.json");
  }
}

var finish = util.finish = function() {
  if (util.sansTypeface   ) { text.unloadFont(util.sansTypeface    ); }
  if (util.serifTypeface  ) { text.unloadFont(util.serifTypeface   ); }
  if (util.sansMonoypeface) { text.unloadFont(util.sansMonoTypeface); }
  openVG.finish();
}

// RGBA fills a color vectors from a RGBA quad.
var RGBA = util.RGBA = function(r, g, b, a, color) {
  if (r > 255) { r = 255; } else if (r < 0) { r = 0; }
  if (g > 255) { g = 255; } else if (g < 0) { g = 0; }
  if (b > 255) { b = 255; } else if (b < 0) { b = 0; }
  if (a < 0.0 || a > 1.0) { a = 1.0; }

  color[0] = r / 255.0;
  color[1] = g / 255.0;
  color[2] = b / 255.0;
  color[3] = a;
}

var setFill = util.setFill = function(color) {
  var fillPaint = openVG.createPaint();
  openVG.setParameterI(fillPaint, openVG.VGPaintParamType.VG_PAINT_TYPE, openVG.VGPaintType.VG_PAINT_TYPE_COLOR);
  openVG.setParameterFVOL(fillPaint, openVG.VGPaintParamType.VG_PAINT_COLOR, color, 0, 4);
  openVG.setPaint(fillPaint, openVG.VGPaintMode.VG_FILL_PATH);
  openVG.destroyPaint(fillPaint);
}

var setStroke = util.setStroke = function(color) {
  var strokePaint = openVG.createPaint();
  openVG.setParameterI(strokePaint, openVG.VGPaintParamType.VG_PAINT_TYPE, openVG.VGPaintType.VG_PAINT_TYPE_COLOR);
  openVG.setParameterFVOL(strokePaint, openVG.VGPaintParamType.VG_PAINT_COLOR, color, 0, 4);
  openVG.setPaint(strokePaint, openVG.VGPaintMode.VG_STROKE_PATH);
  openVG.destroyPaint(strokePaint);
}

var strokeWidth = util.strokeWidth = function(width) {
  openVG.setF(openVG.VGParamType.VG_STROKE_LINE_WIDTH, width);
  openVG.setI(openVG.VGParamType.VG_STROKE_CAP_STYLE, openVG.VGCapStyle.VG_CAP_BUTT);
  openVG.setI(openVG.VGParamType.VG_STROKE_JOIN_STYLE, openVG.VGJoinStyle.VG_JOIN_MITER);
}

function newPath() {
  return openVG.createPath(openVG.VG_PATH_FORMAT_STANDARD,
                           openVG.VGPathDatatype.VG_PATH_DATATYPE_F,
                           1.0, 0.0, 0, 0,
                           openVG.VGPathCapabilities.VG_PATH_CAPABILITY_ALL);
}

// Line makes a line at connecting the specified locations
var line = util.line = function(x0, y0, x1, y1) {
  var path = newPath();
  openVG.vgu.line(path, x0, y0, x1, y1);
  openVG.drawPath(path, openVG.VGPaintMode.VG_FILL_PATH | openVG.VGPaintMode.VG_STROKE_PATH);
  openVG.destroyPath(path);
}

// Rect makes a rectangle at the specified location and dimensions
var rect = util.rect = function(x, y, w, h) {
  var path = newPath();
  openVG.vgu.rect(path, x, y, w, h);
  openVG.drawPath(path, openVG.VGPaintMode.VG_FILL_PATH | openVG.VGPaintMode.VG_STROKE_PATH);
  openVG.destroyPath(path);
}

// Ellipse makes an ellipse at the specified location and dimensions
var ellipse = util.ellipse = function(x, y, w, h) {
  var path = newPath();
  openVG.vgu.ellipse(path, x, y, w, h);
  openVG.drawPath(path, openVG.VGPaintMode.VG_FILL_PATH | openVG.VGPaintMode.VG_STROKE_PATH);
  openVG.destroyPath(path);
}

var circle = util.circle = function(x, y, r) {
  ellipse(x, y, r, r);
}

var background = util.background = function(r, g, b) {
  fill(r, g, b, 1.0);
  rect(0, 0, openVG.screen.width, openVG.screen.height);
}

var fill = util.fill = function(r, g, b, a) {
  var color = new Float32Array(4);
  RGBA(r, g, b, a, color);
  setFill(color);
}

var stroke = util.stroke = function(r, g, b, a) {
  var color = new Float32Array(4);
  RGBA(r, g, b, a, color);
  setStroke(color);
}

var checkVGError = util.checkVGError = function(msg) {
  var err = openVG.getError();
  if (err !== 0) {
    console.log("vgError: " + msg + " : " + openVG.VGErrorCodeReverse[err]);
  }
}
