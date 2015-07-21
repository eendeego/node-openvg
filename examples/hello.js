//
// Based on https://github.com/ajstarks/openvg hellovg.c/go
//

var vg = require('../openvg');

var util = require('./modules/util');
var text = require('./modules/text');

var width, height;

vg.init();
util.init();

var width  = vg.screen.width;
var height = vg.screen.height;

var serifTypeface = text.loadFont("examples/fonts/serif.json");

vg.setFV(vg.VGParamType.VG_CLEAR_COLOR, new Float32Array([ 0.0, 0.0, 0.0, 0.0 ]));

util.strokeWidth(0);

util.clearBackground();
util.setFillColor(0.17255, 0.3020, 0.9098, 1.0); // Big blue marble
util.circle(width/2, 0, width);                 // The "world"
util.setFillColor(1.0, 1.0, 1.0, 1.0);          // White text
text.textMiddle(width/2, height/2,
                "hello, world",
                serifTypeface, width/10); // Greetings
util.swapBuffers();

function terminate() {
  console.log("Terminating...");
  text.unloadFont(serifTypeface);
  util.finish();
  vg.finish();
  console.log("Making a clean exit.");
}

setTimeout(terminate, 5000);
