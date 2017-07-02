// Utility functions shared by examples
//
// Based on https://github.com/ajstarks/openvg libshapes.c/go
//

'use strict';

const openVG = require('../../index');
const text = require('./text');

const {
  VG_CLEAR_COLOR,
  VG_STROKE_CAP_STYLE,
  VG_STROKE_JOIN_STYLE,
} = openVG.VGParamType;

const {VG_PAINT_COLOR, VG_PAINT_TYPE} = openVG.VGPaintParamType;

const {VG_PAINT_TYPE_COLOR} = openVG.VGPaintType;

const {VG_CAP_BUTT} = openVG.VGCapStyle;

const {VG_JOIN_MITER} = openVG.VGJoinStyle;

const {VG_FILL_PATH, VG_STROKE_PATH} = openVG.VGPaintMode;

const VG_FILL_AND_STROKE_PATH = VG_FILL_PATH | VG_STROKE_PATH;

const fonts = {
  sansTypeface: undefined,
  serifTypeface: undefined,
  sansMonoTypeface: undefined,
};

function start() {
  const color = new Float32Array([255, 255, 255, 0]);
  openVG.setFVOL(VG_CLEAR_COLOR, color, 0, 4);
  openVG.clear(0, 0, openVG.screen.width, openVG.screen.height);
  (color[0] = 0), (color[1] = 0), (color[2] = 0);
  setFill(color);
  setStroke(color);
  strokeWidth(0);
  openVG.loadIdentity();
}

function end() {
  openVG.egl.swapBuffers(openVG.screen.display, openVG.screen.surface);
}

function init(options) {
  if (options === undefined) {
    options = {};
  }

  if (options.loadFonts === undefined) {
    options.loadFonts = true;
  }

  openVG.init();

  if (options.loadFonts) {
    fonts.sansTypeface = text.loadFont('examples/fonts/sans.json');
    fonts.serifTypeface = text.loadFont('examples/fonts/serif.json');
    fonts.sansMonoTypeface = text.loadFont('examples/fonts/sans-mono.json');
  }
}

function deinit() {
  if (fonts.sansTypeface) {
    text.unloadFont(fonts.sansTypeface);
  }

  if (fonts.serifTypeface) {
    text.unloadFont(fonts.serifTypeface);
  }

  if (fonts.sansMonoypeface) {
    text.unloadFont(fonts.sansMonoTypeface);
  }

  openVG.deinit();
}

function capRGB(rgb) {
  return (rgb > 255 ? 255 : rgb < 0 ? 0 : rgb) / 255.0;
}

function capAlpha(a) {
  return a < 0.0 || a > 1.0 ? 1.0 : a;
}

// RGBA fills a color vectors from a RGBA quad.
function RGBA(r, g, b, a, color) {
  r = capRGB(r);
  g = capRGB(g);
  b = capRGB(b);
  a = capAlpha(a);

  color[0] = r;
  color[1] = g;
  color[2] = b;
  color[3] = a;
}

function setFill(color) {
  const fillPaint = openVG.createPaint();
  openVG.setParameterI(fillPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
  openVG.setParameterFV(fillPaint, VG_PAINT_COLOR, color);
  openVG.setPaint(fillPaint, VG_FILL_PATH);
  openVG.destroyPaint(fillPaint);
}

function setStroke(color) {
  const strokePaint = openVG.createPaint();
  openVG.setParameterI(strokePaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
  openVG.setParameterFV(strokePaint, VG_PAINT_COLOR, color);
  openVG.setPaint(strokePaint, VG_STROKE_PATH);
  openVG.destroyPaint(strokePaint);
}

function strokeWidth(width) {
  openVG.setF(openVG.VGParamType.VG_STROKE_LINE_WIDTH, width);
  openVG.setI(VG_STROKE_CAP_STYLE, VG_CAP_BUTT);
  openVG.setI(VG_STROKE_JOIN_STYLE, VG_JOIN_MITER);
}

function newPath() {
  return openVG.createPath(
    openVG.VG_PATH_FORMAT_STANDARD,
    openVG.VGPathDatatype.VG_PATH_DATATYPE_F,
    1.0,
    0.0,
    0,
    0,
    openVG.VGPathCapabilities.VG_PATH_CAPABILITY_ALL
  );
}

// Line makes a line at connecting the specified locations
function line(x0, y0, x1, y1) {
  const path = newPath();
  openVG.vgu.line(path, x0, y0, x1, y1);
  openVG.drawPath(path, VG_FILL_AND_STROKE_PATH);
  openVG.destroyPath(path);
}

// Rect makes a rectangle at the specified location and dimensions
function rect(x, y, w, h) {
  const path = newPath();
  openVG.vgu.rect(path, x, y, w, h);
  openVG.drawPath(path, VG_FILL_AND_STROKE_PATH);
  openVG.destroyPath(path);
}

// Ellipse makes an ellipse at the specified location and dimensions
function ellipse(x, y, w, h) {
  const path = newPath();
  openVG.vgu.ellipse(path, x, y, w, h);
  openVG.drawPath(path, VG_FILL_AND_STROKE_PATH);
  openVG.destroyPath(path);
}

function circle(x, y, r) {
  ellipse(x, y, r, r);
}

function background(r, g, b) {
  fill(r, g, b, 1.0);
  rect(0, 0, openVG.screen.width, openVG.screen.height);
}

function fill(r, g, b, a) {
  const color = new Float32Array(4);
  RGBA(r, g, b, a, color);
  setFill(color);
}

function stroke(r, g, b, a) {
  const color = new Float32Array(4);
  RGBA(r, g, b, a, color);
  setStroke(color);
}

function checkVGError(msg) {
  const err = openVG.getError();
  if (err !== 0) {
    console.log('vgError: ' + msg + ' : ' + openVG.VGErrorCodeReverse[err]);
  }
}

module.exports = {
  fonts,
  start,
  end,
  init,
  deinit,
  RGBA,
  setFill,
  setStroke,
  strokeWidth,
  line,
  rect,
  ellipse,
  circle,
  background,
  fill,
  stroke,
  checkVGError,
};
