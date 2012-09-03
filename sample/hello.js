var openVG = require('../openvg');

var countdown = 5;
(function terminator() {
  countdown--;
  if(countdown === 0) {
    console.log('Calling finish.');
    openVG.finish();
    console.log('Got back from finish.');
  } else {
    console.log('Waiting ' + countdown + ' seconds');
    setTimeout(terminator, 1000);
  }
})();

var width, height;

console.log('Calling init.');
openVG.init();
console.log('Got back from init.');

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

console.log('Painted!');
