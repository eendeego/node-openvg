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

var width, height;

openVG.init();

width  = openVG.screen.width;
height = openVG.screen.height;

openVG.start();                             // Start the picture
openVG.background(0, 0, 0);                 // Black background
openVG.fill(44, 77, 232, 1);                // Big blue marble
openVG.circle(width/2, 0, width);           // The "world"
openVG.fill(255, 255, 255, 1);              // White text
// openVG.textMiddle(width/2, height/2,
//                   "hello, world",
//                   SerifTypeface, width/10); // Greetings 
openVG.end();                               // End the picture
