var openVG = require('../openvg');

openVG.init();

console.log("Screen dimensions: " + openVG.screen.width + " x " + openVG.screen.height);

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

console.log("Gradient Max Stops: " + openVG.getI(openVG.VGParamType.VG_MAX_COLOR_RAMP_STOPS))
openVG.finish();
