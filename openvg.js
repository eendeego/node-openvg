var openVG = module.exports = require('./build/Release/node-openvg.node');

openVG.init = function() {
  openVG.lowLevelInit();
};

openVG.finish = function() {
  openVG.lowLevelFinish();
};
