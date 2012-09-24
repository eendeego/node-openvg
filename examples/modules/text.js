// Text functions shared by examples
//
// Based on https://github.com/ajstarks/openvg libshapes.c/go
//

var text = module.exports;

var fs = require('fs');

var openVG = require('../../openvg');
var util = require('./util');

// textwidth returns the width of a text string at the specified font and size.
var textWidth = text.textWidth = function(s, f, size) {
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
var drawText = text.drawText = function(x, y, s, f, pointsize) {
  var size = pointsize, xx = x, mm = new Float32Array(9);

  openVG.getMatrix(mm);
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
    openVG.multMatrix(mat);
    openVG.drawPath(f.glyphs[glyph], openVG.VGPaintMode.VG_FILL_PATH | openVG.VGPaintMode.VG_STROKE_PATH);
    xx += size * f.glyphAdvances[glyph] / 65536.0;
  }
  openVG.loadMatrix(mm);
}

var textMiddle = text.textMiddle = function(x, y, s, f, pointsize) {
  var tw = textWidth(s, f, pointsize);
  drawText(x - (tw / 2.0), y, s, f, pointsize);
}

var MAXFONTPATH = 256;

var loadFont = text.loadFont = function(name) {
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
var unloadFont = text.unloadFont = function(f) {
  for (var i = 0; i < f.glyphs.length; i++) {
    openVG.destroyPath(f.glyphs[i]);
  }
}
