//
// Based on https://github.com/ajstarks/openvg hellovg.c/go
//

'use strict';

const openVG = require('../index');

const util = require('./modules/util');
const text = require('./modules/text');

util.init();

const width = openVG.screen.width;
const height = openVG.screen.height;

util.start(); // Start the picture
util.background(0, 0, 0); // Black background
util.fill(44, 77, 232, 1); // Big blue marble
util.circle(width / 2, 0, width); // The "world"
util.fill(255, 255, 255, 1); // White text

const message = 'hello, world';

text.textMiddle(
  width / 2,
  height / 2,
  message,
  util.fonts.serifTypeface,
  width / 10
); // Greetings

util.end(); // End the picture

process.on('exit', () => {
  util.finish();
  console.log('Making a clean exit.');
});

console.log('Press return to exit.');
process.stdin.resume();
process.stdin.setEncoding('utf8');

process.stdin.once('data', () => process.stdin.pause());
