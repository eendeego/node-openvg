var openVG = module.exports = require('./build/Release/node-openvg.node');

var screen = openVG.screen = {
  width : null,
  height : null
};

openVG.init = function() {
  openVG.lowLevelInit(screen);
};

openVG.finish = function() {
  openVG.lowLevelFinish();
};

openVG.background = function(r, g, b) {
  openVG.fill(r, g, b, 1.0);
  openVG.rect(0, 0, screen.width, screen.height);
};

openVG.circle = function(x, y, r) {
  openVG.ellipse(x, y, r, r);
};
