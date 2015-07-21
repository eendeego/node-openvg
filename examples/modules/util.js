// Utility functions shared by examples
//
// Loosely based on https://github.com/ajstarks/openvg libshapes.c/go
//
var vg = require('../../openvg');
var vgu = vg.vgu;

var util = module.exports = {};

// ----- Paints

var fillPaint, strokePaint;
var color = new Float32Array(4);

var assignColor = util.assignColor = function (r, g, b, a) {
  color[0] = r;
  color[1] = g;
  color[2] = b;
  color[3] = a;
}

var setColor = util.setColor = function (paint) {
  vg.setParameterI   (paint, vg.VGPaintParamType.VG_PAINT_TYPE,
                             vg.VGPaintType.VG_PAINT_TYPE_COLOR);
  vg.setParameterFVOL(paint, vg.VGPaintParamType.VG_PAINT_COLOR, color, 0, 4);
}

var setFillColor = util.setFillColor = function (r, g, b, a) {
  assignColor(r, g, b, a);
  setColor(fillPaint);
  vg.setPaint(fillPaint, vg.VGPaintMode.VG_FILL_PATH);
}

var setStrokeColor = util.setStrokeColor = function (r, g, b, a) {
  assignColor(r, g, b, a);
  setColor(strokePaint);
  vg.setPaint(strokePaint, vg.VGPaintMode.VG_STROKE_PATH);
}

// ----- Paths

var defaultPathBuffer;
var defaultPath;

var PathBuffer = util.PathBuffer = function () {
  this.segments = new Uint8Array  (4 * 1024);
  this.data     = new Float32Array(4 * 1024);
  this.segmentsPos = 0;
  this.dataPos     = 0;
}

PathBuffer.prototype.clear = function () {
  this.segmentsPos = 0;
  this.dataPos     = 0;
};

PathBuffer.prototype.moveTo = function (x, y) {
  this.segments[this.segmentsPos++] = vg.VGPathCommand.VG_MOVE_TO_ABS;
  this.data    [this.dataPos++    ] = x;
  this.data    [this.dataPos++    ] = y;
};

PathBuffer.prototype.lineTo = function (x, y) {
  this.segments[this.segmentsPos++] = vg.VGPathCommand.VG_LINE_TO_ABS;
  this.data    [this.dataPos++    ] = x;
  this.data    [this.dataPos++    ] = y;
};

PathBuffer.prototype.close = function () {
  this.segments[this.segmentsPos++] = vg.VGPathSegment.VG_CLOSE_PATH;
};

// ----- Misc

var clearBackground = util.clearBackground = function () {
  vg.clear(0, 0, vg.screen.width, vg.screen.height);
}

var strokeWidth = util.strokeWidth = function (width) {
  vg.setF(vg.VGParamType.VG_STROKE_LINE_WIDTH, width);
}

var swapBuffers = util.swapBuffers = function () {
  vg.egl.swapBuffers(vg.screen.display, vg.screen.surface);
}

// ----- Default path ops

var line = util.line = function(x0, y0, x1, y1) {
  vg.clearPath(defaultPath, vg.VGPathCapabilities.VG_PATH_CAPABILITY_ALL);
  vgu.line(defaultPath, x0, y0, x1, y1);
  vg.drawPath(defaultPath, vg.VGPaintMode.VG_FILL_PATH | vg.VGPaintMode.VG_STROKE_PATH);
}

var rect = util.rect = function (x, y, w, h) {
  vg.clearPath(defaultPath, vg.VGPathCapabilities.VG_PATH_CAPABILITY_ALL);
  vgu.rect(defaultPath, x, y, w, h);
  vg.drawPath(defaultPath, vg.VGPaintMode.VG_FILL_PATH | vg.VGPaintMode.VG_STROKE_PATH);
}

var ellipse = util.ellipse = function (x, y, w, h) {
  vg.clearPath(defaultPath, vg.VGPathCapabilities.VG_PATH_CAPABILITY_ALL);
  vgu.ellipse(defaultPath, x, y, w, h);
  vg.drawPath(defaultPath, vg.VGPaintMode.VG_FILL_PATH | vg.VGPaintMode.VG_STROKE_PATH);
}

var circle = util.circle = function (x, y, r) {
  ellipse(x, y, r, r);
}

// ----- 

var init = util.init = function () {
  fillPaint   = vg.createPaint();
  strokePaint = vg.createPaint();

  defaultPathBuffer = new PathBuffer();
  defaultPath = vg.createPath(vg.VG_PATH_FORMAT_STANDARD,
                              vg.VGPathDatatype.VG_PATH_DATATYPE_F,
                              1.0 /* scale */ , 0.0 /* bias */,
                              0 /* segCapacityHint */,
                              0 /* coordCapacityHint */,
                              vg.VGPathCapabilities.VG_PATH_CAPABILITY_ALL);
}

var finish = util.finish = function () {
  vg.destroyPath (defaultPath);
  vg.destroyPaint(fillPaint  );
  vg.destroyPaint(strokePaint);
}
