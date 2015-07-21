var vg = require('../openvg');

var util = require('./modules/util');

var width, height;

var starPoints = 5;
var outerRadius = 1;

var star;

// -----

function init() {
  vg.init();

  width  = vg.screen.width;
  height = vg.screen.height;

  util.init();

  vg.setFV(vg.VGParamType.VG_CLEAR_COLOR, new Float32Array([ 0.0, 0.0, 0.0, 0.0 ]));

  util.setFillColor  (1.0,   0,   0, 1.0);
  util.setStrokeColor(1.0, 1.0, 1.0, 1.0);
  util.strokeWidth(2);

  vg.setI(vg.VGParamType.VG_STROKE_CAP_STYLE , vg.VGCapStyle.VG_CAP_BUTT);
  vg.setI(vg.VGParamType.VG_STROKE_JOIN_STYLE, vg.VGJoinStyle.VG_JOIN_MITER);

  star = createStar(starPoints, outerRadius * 2 * height / 6);
}

function createStar(starPoints, outerRadius) {
  var innerRadius = outerRadius *
                    (Math.sin(Math.PI / 2 - 2 * Math.PI / starPoints) /
                     Math.sin(Math.PI / 2 - 2 * Math.PI / starPoints / 2));

  var path = new util.PathBuffer();

  var angle = 0;
  var rho = Math.PI * 2 / starPoints / 2;

  path.moveTo(outerRadius * Math.cos(angle), outerRadius * Math.sin(angle));

  for(var i = 1; i < starPoints * 2; i++) {
    angle += rho;
    path.lineTo(innerRadius * Math.cos(angle), innerRadius * Math.sin(angle));
    angle += rho; i++;
    path.lineTo(outerRadius * Math.cos(angle), outerRadius * Math.sin(angle));
  }

  var vgPath = vg.createPath(vg.VG_PATH_FORMAT_STANDARD,
                             vg.VGPathDatatype.VG_PATH_DATATYPE_F,
                             1.0 /* scale */ , 0.0 /* bias */,
                             path.segmentsPos /* segCapacityHint */,
                             path.dataPos /* coordCapacityHint */,
                             vg.VGPathCapabilities.VG_PATH_CAPABILITY_ALL);
  vg.appendPathData(vgPath, path.segmentsPos, path.segments, path.data);

  return vgPath;
}

function paint(t) {
  util.clearBackground();

  var baseAngle = 90 + t * 0.00005 * 360;

  vg.translate(width / 2, height / 2);
  vg.rotate(baseAngle);
  vg.drawPath(star, vg.VGPaintMode.VG_FILL_PATH | vg.VGPaintMode.VG_STROKE_PATH);
  vg.rotate(-baseAngle);
  vg.translate(-width / 2, -height / 2);

  util.swapBuffers();
}

var fps = 120;

var firstRefresh;
var paintInterval;
function refresh() {
  var t = Date.now();
  paint(t - firstRefresh);
  var tt = Date.now();
  paintInterval = setTimeout(refresh, 1000/fps - (tt-t));
}

function terminate() {
  console.log("Terminating...");
  clearTimeout(paintInterval);
  util.finish();
  vg.destroyPath(star);
  vg.finish();
  console.log("Making a clean exit.");
}

init();

setTimeout(terminate, 5000);

// var paintInterval = setInterval(refresh, animTime);
firstRefresh = Date.now();
refresh();

process.on('exit', terminate);


// console.log("Press return to exit.");
// process.stdin.resume();
// process.stdin.setEncoding('utf8');

// process.stdin.once('data', function (chunk) {
//   clearInterval(paintInterval);
//   process.stdin.pause();
// });
