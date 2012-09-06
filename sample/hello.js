//
// Based on https://github.com/ajstarks/openvg hellovg.c/go
//

var openVG = require('../openvg');

var util = require('./util');
var text = require('./text');

var countdown = 5;
(function terminator() {
  countdown--;
  setTimeout(countdown === 1 ? util.finish : terminator, 1000);
})();

var width, height;

console.log("Reading fonts...");

util.init();

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

util.start();                             // Start the picture
util.background(0, 0, 0);                 // Black background
util.fill(44, 77, 232, 1);                // Big blue marble
util.circle(width/2, 0, width);           // The "world"
util.fill(255, 255, 255, 1);              // White text
text.textMiddle(width/2, height/2,
                "hello, world",
                util.serifTypeface, width/10); // Greetings
util.end();                               // End the picture
