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

console.log('Calling init.');
openVG.init();
console.log('Got back from init.');
