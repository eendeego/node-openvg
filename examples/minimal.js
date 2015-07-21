var vg = require('../openvg');

vg.init();

var color = new Float32Array(4);
color[0] = 0.0;
color[1] = 0.0;
color[2] = 0.0;
color[3] = 1.0;

var fillPaint = vg.createPaint();
vg.setParameterI(fillPaint, vg.VGPaintParamType.VG_PAINT_TYPE, vg.VGPaintType.VG_PAINT_TYPE_COLOR);
vg.setParameterFVOL(fillPaint, vg.VGPaintParamType.VG_PAINT_COLOR, color, 0, 4);
vg.setPaint(fillPaint, vg.VGPaintMode.VG_FILL_PATH);
vg.destroyPaint(fillPaint);

vg.egl.swapBuffers(vg.screen.display, vg.screen.surface);
