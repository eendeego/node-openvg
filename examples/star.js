var vg = require('../openvg');
var util = require('./modules/util');

var width, height;

var starPoints = 5;
var outerRadius = 1;
var innerRadius = Math.sin(Math.PI / 2 - 2 * Math.PI / starPoints) /
                  Math.sin(Math.PI / 2 - 2 * Math.PI / starPoints / 2);

var vgPath;
var segments = new Uint8Array(4*1024);
var data     = new Float32Array(4*1024);
var segmentsPos = 0;
var dataPos = 0;

var moveTo = function (x, y) {
  segments[segmentsPos++] = vg.VGPathCommand.VG_MOVE_TO_ABS;
  data[dataPos++] = x;
  data[dataPos++] = y;
};

var lineTo = function (x, y) {
  segments[segmentsPos++] = vg.VGPathCommand.VG_LINE_TO_ABS;
  data[dataPos++] = x;
  data[dataPos++] = y;
};

var close = function () {
  segments[segmentsPos++] = vg.VGPathSegment.VG_CLOSE_PATH;
};

function init() {
  util.fill(255, 0, 0, 1);
  util.stroke(255, 255, 255, 1.0);
  util.strokeWidth(2);

  vgPath = vg.createPath(vg.VG_PATH_FORMAT_STANDARD,
                         vg.VGPathDatatype.VG_PATH_DATATYPE_F,
                         1.0 /* scale */ , 0.0 /* bias */,
                         0 /* segCapacityHint */,
                         0 /* coordCapacityHint */,
                         vg.VGPathCapabilities.VG_PATH_CAPABILITY_ALL);
}

function paint() {
  util.strokeWidth(0);
  util.background(0, 0, 0);

  vg.clearPath(vgPath, vg.VGPathCapabilities.VG_PATH_CAPABILITY_ALL);

  segmentsPos = 0;
  dataPos = 0;

  var baseAngle = Math.PI / 2 + Date.now() / 1000 / 2 / (Math.PI / 2);
  var angle = 0 * Math.PI * 2 / starPoints / 2 + baseAngle;

  moveTo(width / 2 + outerRadius * Math.cos(angle), height / 2 + outerRadius * Math.sin(angle));

  for(var i = 1; i < starPoints * 2; i++) {
    var angle = i * Math.PI * 2 / starPoints / 2 + baseAngle;
    lineTo(width / 2 + innerRadius * Math.cos(angle), height / 2 + innerRadius * Math.sin(angle));
    i++;
    angle = i * Math.PI * 2 / starPoints / 2 + baseAngle;
    lineTo(width / 2 + outerRadius * Math.cos(angle), height / 2 + outerRadius * Math.sin(angle));
  }

  vg.appendPathData(vgPath, segmentsPos, segments, data);

  util.strokeWidth(2);
  vg.drawPath(vgPath, vg.VGPaintMode.VG_FILL_PATH | vg.VGPaintMode.VG_STROKE_PATH);

  util.end();
}

function terminate() {
  util.finish();
  console.log("Making a clean exit.");
}

util.init();

width  = vg.screen.width;
height = vg.screen.height;

outerRadius *= 2 * height / 6;
innerRadius *= 2 * height / 6;

util.start();

var animTime = 1000 / 120;

init();
var paintInterval = setInterval(paint, animTime);
// var paintInterval = setTimeout(paint, animTime);

process.on('exit', terminate);

console.log("Press return to exit.");
process.stdin.resume();
process.stdin.setEncoding('utf8');

process.stdin.once('data', function (chunk) {
  clearInterval(paintInterval);
  process.stdin.pause();
});
