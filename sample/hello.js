var openVG = require('../openvg');

var countdown = 5;
(function terminator() {
  countdown--;
  if(countdown === 0) {
    openVG.finish();
  } else {
    setTimeout(terminator, 1000);
  }
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

var width, height;

openVG.init();

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
// textMiddle(width/2, height/2,
//            "hello, world",
//            SerifTypeface, width/10); // Greetings 
openVG.end();                               // End the picture
