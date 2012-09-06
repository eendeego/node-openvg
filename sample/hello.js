var fs = require('fs');

var openVG = require('../openvg');

function finish() {
  unloadFont(sansTypeface);
  unloadFont(serifTypeface);
  unloadFont(sansMonoTypeface);
  openVG.finish();
}

var countdown = 5;
(function terminator() {
  countdown--;
  setTimeout(countdown === 1 ? finish : terminator, 1000);
})();

// RGBA fills a color vectors from a RGBA quad.
function RGBA(r, g, b, a, color) {
  if (r > 255) { r = 255; } else if (r < 0) { r = 0; }
  if (g > 255) { g = 255; } else if (g < 0) { g = 0; }
  if (b > 255) { b = 255; } else if (b < 0) { b = 0; }
  if (a < 0.0 || a > 1.0) { a = 1.0; }

  color[0] = r / 255.0;
  color[1] = g / 255.0;
  color[2] = b / 255.0;
  color[3] = a;
}

function start() {
  var color = new Float32Array([255, 255, 255, 1]);
  openVG.setFV(openVG.VGParamType.VG_CLEAR_COLOR, color);
  openVG.clear(0, 0, openVG.screen.width, openVG.screen.height);
  color[0] = 0, color[1] = 0, color[2] = 0;
  setFill(color);
  setStroke(color);
  strokeWidth(0);
  openVG.loadIdentity();
}

function end() {
  openVG.egl.swapBuffers(openVG.screen.display, openVG.screen.surface);
}

function setFill(color) {
  var fillPaint = openVG.createPaint();
  openVG.setParameterI(fillPaint, openVG.VGPaintParamType.VG_PAINT_TYPE, openVG.VGPaintType.VG_PAINT_TYPE_COLOR);
  openVG.setParameterFV(fillPaint, openVG.VGPaintParamType.VG_PAINT_COLOR, color);
  openVG.setPaint(fillPaint, openVG.VGPaintMode.VG_FILL_PATH);
  openVG.destroyPaint(fillPaint);
}

function setStroke(color) {
  var strokePaint = openVG.createPaint();
  openVG.setParameterI(strokePaint, openVG.VGPaintParamType.VG_PAINT_TYPE, openVG.VGPaintType.VG_PAINT_TYPE_COLOR);
  openVG.setParameterFV(strokePaint, openVG.VGPaintParamType.VG_PAINT_COLOR, color);
  openVG.setPaint(strokePaint, openVG.VGPaintMode.VG_STROKE_PATH);
  openVG.destroyPaint(strokePaint);
}

function strokeWidth(width) {
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

// Rect makes a rectangle at the specified location and dimensions
function rect(x, y, w, h) {
  var path = newPath();
  openVG.vgu.rect(path, x, y, w, h);
  openVG.drawPath(path, openVG.VGPaintMode.VG_FILL_PATH | openVG.VGPaintMode.VG_STROKE_PATH);
  openVG.destroyPath(path);
}

// Ellipse makes an ellipse at the specified location and dimensions
function ellipse(x, y, w, h) {
  var path = newPath();
  openVG.vgu.ellipse(path, x, y, w, h);
  openVG.drawPath(path, openVG.VGPaintMode.VG_FILL_PATH | openVG.VGPaintMode.VG_STROKE_PATH);
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
  var color = new Float32Array(4);
  RGBA(r, g, b, a, color);
  setFill(color);
}

function checkVGError(msg) {
  var err = openVG.getError();
  if (err !== 0) {
    console.log("vgError: " + msg + " : " + openVG.VGErrorCodeReverse[err]);
  }
}

// textwidth returns the width of a text string at the specified font and size.
function textWidth(s, f, size) {
  var tw = 0.0;
  for (var i = 0; i < s.length; i++) {
    var character = s.charCodeAt(i);
    var glyph = f.characterMap[character];
    if (glyph == -1) {
      continue; //glyph is undefined
    }
    tw += size * f.glyphAdvances[glyph] / 65536.0;
  }
  return tw;
}

// Text renders a string of text at a specified location, size, using the specified font glyphs
// derived from http://web.archive.org/web/20070808195131/http://developer.hybrid.fi/font2openvg/renderFont.cpp.txt
function text(x, y, s, f, pointsize) {
  var size = pointsize, xx = x, mm = new Float32Array(9);

  openVG.getMatrix(mm);
  checkVGError("getMatrix");
  for (var i = 0; i < s.length; i++) {
    var character = s.charCodeAt(i);
    var glyph = f.characterMap[character];
    if (glyph == -1) {
      continue;  //glyph is undefined
    }
    var mat = new Float32Array([
      size, 0.0, 0.0,
      0.0, size, 0.0,
      xx, y, 1.0
    ]);
    openVG.loadMatrix(mm);
    checkVGError("loadMatrix.");
    openVG.multMatrix(mat);
    checkVGError("multMatrix.");
    openVG.drawPath(f.glyphs[glyph], openVG.VGPaintMode.VG_FILL_PATH | openVG.VGPaintMode.VG_STROKE_PATH);
    checkVGError("drawPath.");
    xx += size * f.glyphAdvances[glyph] / 65536.0;
  }
  openVG.loadMatrix(mm);
  checkVGError("loadMatrix");
}

function textMiddle(x, y, s, f, pointsize) {
  var tw = textWidth(s, f, pointsize);
  text(x - (tw / 2.0), y, s, f, pointsize);
}

var MAXFONTPATH = 256;

function loadFont(name) {
  var jsonf = JSON.parse(fs.readFileSync(name));
  var f = { glyphs: new Uint32Array(MAXFONTPATH), characterMap: jsonf.characterMap, glyphAdvances: jsonf.glyphAdvances, count: jsonf.glyphCount };

  var glyphPoints = new Int32Array(jsonf.glyphPoints);
  var glyphInstructions = new Uint8Array(jsonf.glyphInstructions);

  if (jsonf.glyphCount > MAXFONTPATH) {
    // return f;
    return { err: "Font is too big" };
  }
  for (var i = 0; i < jsonf.glyphCount; i++) {
    var instructions      = glyphInstructions.subarray(jsonf.glyphInstructionIndices[i]);
    var instructionCounts = jsonf.glyphInstructionCounts[i];

    var path = openVG.createPath(openVG.VG_PATH_FORMAT_STANDARD, openVG.VGPathDatatype.VG_PATH_DATATYPE_S_32,
                                 1.0 / 65536.0, 0.0, 0, 0,
                                 openVG.VGPathCapabilities.VG_PATH_CAPABILITY_ALL);

    f.glyphs[i] = path;
    if (instructionCounts) {
      var offset = 2 * jsonf.glyphPointIndices[i];
      var points = glyphPoints.subarray(offset);

      openVG.appendPathData(path, instructionCounts, instructions, points);
    }
  }
  return f;
}

// unloadfont frees font path data
function unloadFont(f) {
  for (var i = 0; i < f.glyphs.length; i++) {
    openVG.destroyPath(f.glyphs[i]);
  }
}

var width, height;

openVG.init();

console.log("Reading fonts...");

var sansTypeface     = loadFont("sample/sans.json");
var serifTypeface    = loadFont("sample/serif.json");
var sansMonoTypeface = loadFont("sample/sans-mono.json");

console.log("Done.");

width  = openVG.screen.width;
height = openVG.screen.height;

console.log("Screen dimensions: " + width + " x " + height);

console.log("Hardware Image Formats:");
Object.keys(openVG.VGImageFormat).map(function(fmt) {
  console.log("  [" + fmt + "]: " +
              (openVG.hardwareQuery(openVG.VGHardwareQueryType.VG_IMAGE_FORMAT_QUERY, openVG.VGImageFormat[fmt]) == openVG.VGHardwareQueryResult.VG_HARDWARE_ACCELERATED ? "✓" : ""));
  });

console.log("Hardware Path Data Types:");
Object.keys(openVG.VGPathDatatype).map(function(type) {
  console.log("  [" + type + "]: " +
              (openVG.hardwareQuery(openVG.VGHardwareQueryType.VG_PATH_DATATYPE_QUERY, openVG.VGPathDatatype[type]) == openVG.VGHardwareQueryResult.VG_HARDWARE_ACCELERATED ? "✓" : ""));
  });

console.log("Renderer and Extension Information:");
Object.keys(openVG.VGStringID).map(function(string) {
  console.log("  [" + string + "]: " + openVG.getString(openVG.VGStringID[string]));
  });

start();                             // Start the picture
background(0, 0, 0);                 // Black background
fill(44, 77, 232, 1);                // Big blue marble
circle(width/2, 0, width);           // The "world"
fill(255, 255, 255, 1);              // White text
textMiddle(width/2, height/2,
           "hello, world",
           serifTypeface, width/10); // Greetings
end();                               // End the picture
