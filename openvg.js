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
