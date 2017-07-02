'use strict';

const openVG = require('../index');

openVG.init();

console.log('Screen dimensions:');
console.log('  ' + openVG.screen.width + ' x ' + openVG.screen.height);
console.log('');

const {
  VG_IMAGE_FORMAT_QUERY,
  VG_PATH_DATATYPE_QUERY,
} = openVG.VGHardwareQueryType;
const {
  VG_HARDWARE_ACCELERATED,
  VG_HARDWARE_UNACCELERATED,
} = openVG.VGHardwareQueryResult;

console.log('Hardware Image Formats:');
Object.keys(openVG.VGImageFormat).map(fmt => {
  const result = openVG.hardwareQuery(
    VG_IMAGE_FORMAT_QUERY,
    openVG.VGImageFormat[fmt]
  );

  console.log(
    '  ' +
      fmt +
      ': ' +
      (result === VG_HARDWARE_ACCELERATED
        ? 'VG_HARDWARE_ACCELERATED'
        : result === VG_HARDWARE_UNACCELERATED
          ? 'VG_HARDWARE_UNACCELERATED'
          : result === 0 ? '-' : '- (0x' + result.toString(16) + ')')
  );
});

console.log('');

console.log('Hardware Path Data Types:');
Object.keys(openVG.VGPathDatatype).map(type => {
  const result = openVG.hardwareQuery(
    VG_PATH_DATATYPE_QUERY,
    openVG.VGPathDatatype[type]
  );

  console.log(
    '  ' +
      type +
      ': ' +
      (result === VG_HARDWARE_ACCELERATED
        ? 'VG_HARDWARE_ACCELERATED'
        : result === VG_HARDWARE_UNACCELERATED
          ? 'VG_HARDWARE_UNACCELERATED'
          : result === 0 ? '-' : '- (0x' + result.toString(16) + ')')
  );
});

console.log('');

console.log('Renderer and Extension Information:');
Object.keys(openVG.VGStringID).map(string => {
  console.log(
    '  [' + string + ']: ' + openVG.getString(openVG.VGStringID[string])
  );
});

console.log('');
console.log('Params:');

console.log(
  '  Max Scissor Rects ........: ' +
    openVG.getI(openVG.VGParamType.VG_MAX_SCISSOR_RECTS)
);
console.log(
  '  Max Dash Count ...........: ' +
    openVG.getI(openVG.VGParamType.VG_MAX_DASH_COUNT)
);
console.log(
  '  Max Kernel Size ..........: ' +
    openVG.getI(openVG.VGParamType.VG_MAX_KERNEL_SIZE)
);
console.log(
  '  Max Separable Kernel Size : ' +
    openVG.getI(openVG.VGParamType.VG_MAX_SEPARABLE_KERNEL_SIZE)
);
console.log(
  '  Max Image Width ..........: ' +
    openVG.getI(openVG.VGParamType.VG_MAX_IMAGE_WIDTH)
);
console.log(
  '  Max Image Height .........: ' +
    openVG.getI(openVG.VGParamType.VG_MAX_IMAGE_HEIGHT)
);
console.log(
  '  Max Image Pixels .........: ' +
    openVG.getI(openVG.VGParamType.VG_MAX_IMAGE_PIXELS)
);
console.log(
  '  Max Image Bytes ..........: ' +
    openVG.getI(openVG.VGParamType.VG_MAX_IMAGE_BYTES)
);
console.log(
  '  Max Float ................: ' +
    openVG.getI(openVG.VGParamType.VG_MAX_FLOAT)
);
console.log(
  '  Max Gaussian Std Deviation: ' +
    openVG.getI(openVG.VGParamType.VG_MAX_GAUSSIAN_STD_DEVIATION)
);
openVG.finish();
