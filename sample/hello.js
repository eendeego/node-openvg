//
// Based on https://github.com/ajstarks/openvg hellovg.c/go
//

var openVG = require('../openvg');

var util = require('./modules/util');
var text = require('./modules/text');

var countdown = 5;
(function terminator() {
  countdown--;
  setTimeout(countdown === 1 ? util.finish : terminator, 1000);
})();

var width, height;

util.init();

width  = openVG.screen.width;
height = openVG.screen.height;

util.start();                             // Start the picture
util.background(0, 0, 0);                 // Black background
util.fill(44, 77, 232, 1);                // Big blue marble
util.circle(width/2, 0, width);           // The "world"
util.fill(255, 255, 255, 1);              // White text
text.textMiddle(width/2, height/2,
                "hello, world",
                util.serifTypeface, width/10); // Greetings
util.end();                               // End the picture
