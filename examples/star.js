'use strict';

const vg = require('../index');
const util = require('./modules/util');

util.init({loadFonts: false});

const {width, height} = vg.screen;

const starPoints = 5;
let outerRadius = 1;
let innerRadius =
  Math.sin(Math.PI / 2 - 2 * Math.PI / starPoints) /
  Math.sin(Math.PI / 2 - 2 * Math.PI / starPoints / 2);

const segments = new Uint8Array(4 * 1024);
const data = new Float32Array(4 * 1024);
let segmentsPos = 0;
let dataPos = 0;

function moveTo(x, y) {
  segments[segmentsPos++] = vg.VGPathCommand.VG_MOVE_TO_ABS;
  data[dataPos++] = x;
  data[dataPos++] = y;
}

function lineTo(x, y) {
  segments[segmentsPos++] = vg.VGPathCommand.VG_LINE_TO_ABS;
  data[dataPos++] = x;
  data[dataPos++] = y;
}

function init() {
  util.fill(255, 0, 0, 1);
  util.stroke(255, 255, 255, 1.0);
  util.strokeWidth(2);

  return vg.createPath(
    vg.VG_PATH_FORMAT_STANDARD,
    vg.VGPathDatatype.VG_PATH_DATATYPE_F,
    1.0 /* scale */,
    0.0 /* bias */,
    0 /* segCapacityHint */,
    0 /* coordCapacityHint */,
    vg.VGPathCapabilities.VG_PATH_CAPABILITY_ALL
  );
}

function paint(path) {
  util.background(0, 0, 0);

  vg.clearPath(path, vg.VGPathCapabilities.VG_PATH_CAPABILITY_ALL);

  segmentsPos = 0;
  dataPos = 0;

  const baseAngle = Math.PI / 2 + Date.now() / 1000 / 2 / (Math.PI / 2);
  const angle = 0 * Math.PI * 2 / starPoints / 2 + baseAngle;

  moveTo(
    width / 2 + outerRadius * Math.cos(angle),
    height / 2 + outerRadius * Math.sin(angle)
  );

  for (let i = 1; i < starPoints * 2; i++) {
    let angle = i * Math.PI * 2 / starPoints / 2 + baseAngle;
    lineTo(
      width / 2 + innerRadius * Math.cos(angle),
      height / 2 + innerRadius * Math.sin(angle)
    );
    i++;
    angle = i * Math.PI * 2 / starPoints / 2 + baseAngle;
    lineTo(
      width / 2 + outerRadius * Math.cos(angle),
      height / 2 + outerRadius * Math.sin(angle)
    );
  }

  vg.appendPathData(path, segmentsPos, segments, data);

  util.fill(255, 0, 0, 1);
  vg.drawPath(
    path,
    vg.VGPaintMode.VG_FILL_PATH | vg.VGPaintMode.VG_STROKE_PATH
  );

  util.end();
}

function terminate() {
  util.finish();
  console.log('Making a clean exit.');
}

outerRadius *= 2 * height / 6;
innerRadius *= 2 * height / 6;

util.start();

const animTime = 1000 / 120;

const path = init();
const paintInterval = setInterval(() => paint(path), animTime);

process.on('exit', terminate);

console.log('Press return to exit.');
process.stdin.resume();
process.stdin.setEncoding('utf8');

process.stdin.once('data', () => {
  clearInterval(paintInterval);
  process.stdin.pause();
});
