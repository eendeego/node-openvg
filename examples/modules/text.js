// Text functions shared by examples
//
// Based on https://github.com/ajstarks/openvg libshapes.c/go
//

'use strict';

const fs = require('fs');

const openVG = require('../../index');

// textwidth returns the width of a text string at the specified font and size.
function textWidth(s, f, size) {
  let tw = 0.0;
  for (let i = 0; i < s.length; i++) {
    const character = s.charCodeAt(i);
    const glyph = f.characterMap[character];
    if (glyph == -1) {
      continue; //glyph is undefined
    }

    tw += size * f.glyphAdvances[glyph] / 65536.0;
  }

  return tw;
}

// Text renders a string of text at a specified location, size, using the
// specified font glyphs derived from
// http://web.archive.org/web/20070808195131/http://developer.hybrid.fi/font2openvg/renderFont.cpp.txt
function drawText(x, y, s, f, pointsize) {
  const size = pointsize;
  const baseMatrix = new Float32Array(9);
  let xx = x;

  openVG.getMatrix(baseMatrix);

  for (let i = 0; i < s.length; i++) {
    const character = s.charCodeAt(i);
    const glyph = f.characterMap[character];
    if (glyph == -1) {
      continue; //glyph is undefined
    }

    // prettier-ignore
    const glyphTransform = new Float32Array([
      size, 0.0, 0.0,
      0.0, size, 0.0,
      xx, y, 1.0,
    ]);
    openVG.loadMatrix(baseMatrix);
    openVG.multMatrix(glyphTransform);
    openVG.drawPath(
      f.glyphs[glyph],
      openVG.VGPaintMode.VG_FILL_PATH | openVG.VGPaintMode.VG_STROKE_PATH
    );
    xx += size * f.glyphAdvances[glyph] / 65536.0;
  }

  openVG.loadMatrix(baseMatrix);
}

function textMiddle(x, y, s, f, pointsize) {
  const tw = textWidth(s, f, pointsize);
  drawText(x - tw / 2.0, y, s, f, pointsize);
}

const MAXFONTPATH = 256;

function loadFont(name) {
  const jsonf = JSON.parse(fs.readFileSync(name));

  const f = {
    glyphs: new Uint32Array(MAXFONTPATH),
    characterMap: jsonf.characterMap,
    glyphAdvances: jsonf.glyphAdvances,
    count: jsonf.glyphCount,
  };

  const glyphPoints = new Int32Array(jsonf.glyphPoints);
  const glyphInstructions = new Uint8Array(jsonf.glyphInstructions);

  if (jsonf.glyphCount > MAXFONTPATH) {
    return {err: 'Font is too big'};
  }

  for (let i = 0; i < jsonf.glyphCount; i++) {
    const instructions = glyphInstructions.subarray(
      jsonf.glyphInstructionIndices[i]
    );
    const instructionCounts = jsonf.glyphInstructionCounts[i];

    const path = openVG.createPath(
      openVG.VG_PATH_FORMAT_STANDARD,
      openVG.VGPathDatatype.VG_PATH_DATATYPE_S_32,
      1.0 / 65536.0, // scale
      0.0, // bias
      0, // segmentCapacityHint
      0, // coordCapacityHint
      openVG.VGPathCapabilities.VG_PATH_CAPABILITY_ALL
    );

    f.glyphs[i] = path;
    if (instructionCounts) {
      const offset = 2 * jsonf.glyphPointIndices[i];
      const points = glyphPoints.subarray(offset);

      openVG.appendPathData(path, instructionCounts, instructions, points);
    }
  }

  return f;
}

// unloadfont frees font path data
function unloadFont(f) {
  for (let i = 0; i < f.glyphs.length; i++) {
    openVG.destroyPath(f.glyphs[i]);
  }
}

module.exports = {
  textWidth,
  drawText,
  textMiddle,
  loadFont,
  unloadFont,
};
