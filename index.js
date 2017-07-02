'use strict';

const openVG = require('./build/Release/openvg.node');

const screen = {
  width: null,
  height: null,
};

function reverseMap(map) {
  const result = {};
  Object.keys(map).forEach(key => (result[map[key]] = key));
  return result;
}

const VGErrorCode = {
  VG_NO_ERROR: 0x0000,
  VG_BAD_HANDLE_ERROR: 0x1000,
  VG_ILLEGAL_ARGUMENT_ERROR: 0x1001,
  VG_OUT_OF_MEMORY_ERROR: 0x1002,
  VG_PATH_CAPABILITY_ERROR: 0x1003,
  VG_UNSUPPORTED_IMAGE_FORMAT_ERROR: 0x1004,
  VG_UNSUPPORTED_PATH_FORMAT_ERROR: 0x1005,
  VG_IMAGE_IN_USE_ERROR: 0x1006,
  VG_NO_CONTEXT_ERROR: 0x1007,
};

const VGErrorCodeReverse = reverseMap(VGErrorCode);

const VGParamType = {
  /* Mode settings */
  VG_MATRIX_MODE: 0x1100,
  VG_FILL_RULE: 0x1101,
  VG_IMAGE_QUALITY: 0x1102,
  VG_RENDERING_QUALITY: 0x1103,
  VG_BLEND_MODE: 0x1104,
  VG_IMAGE_MODE: 0x1105,

  /* Scissoring rectangles */
  VG_SCISSOR_RECTS: 0x1106,

  /* Color Transformation */
  VG_COLOR_TRANSFORM: 0x1170,
  VG_COLOR_TRANSFORM_VALUES: 0x1171,

  /* Stroke parameters */
  VG_STROKE_LINE_WIDTH: 0x1110,
  VG_STROKE_CAP_STYLE: 0x1111,
  VG_STROKE_JOIN_STYLE: 0x1112,
  VG_STROKE_MITER_LIMIT: 0x1113,
  VG_STROKE_DASH_PATTERN: 0x1114,
  VG_STROKE_DASH_PHASE: 0x1115,
  VG_STROKE_DASH_PHASE_RESET: 0x1116,

  /* Edge fill color for VG_TILE_FILL tiling mode */
  VG_TILE_FILL_COLOR: 0x1120,

  /* Color for vgClear */
  VG_CLEAR_COLOR: 0x1121,

  /* Glyph origin */
  VG_GLYPH_ORIGIN: 0x1122,

  /* Enable/disable alpha masking and scissoring */
  VG_MASKING: 0x1130,
  VG_SCISSORING: 0x1131,

  /* Pixel layout information */
  VG_PIXEL_LAYOUT: 0x1140,
  VG_SCREEN_LAYOUT: 0x1141,

  /* Source format selection for image filters */
  VG_FILTER_FORMAT_LINEAR: 0x1150,
  VG_FILTER_FORMAT_PREMULTIPLIED: 0x1151,

  /* Destination write enable mask for image filters */
  VG_FILTER_CHANNEL_MASK: 0x1152,

  /* Implementation limits (read-only) */
  VG_MAX_SCISSOR_RECTS: 0x1160,
  VG_MAX_DASH_COUNT: 0x1161,
  VG_MAX_KERNEL_SIZE: 0x1162,
  VG_MAX_SEPARABLE_KERNEL_SIZE: 0x1163,
  VG_MAX_COLOR_RAMP_STOPS: 0x1164,
  VG_MAX_IMAGE_WIDTH: 0x1165,
  VG_MAX_IMAGE_HEIGHT: 0x1166,
  VG_MAX_IMAGE_PIXELS: 0x1167,
  VG_MAX_IMAGE_BYTES: 0x1168,
  VG_MAX_FLOAT: 0x1169,
  VG_MAX_GAUSSIAN_STD_DEVIATION: 0x116a,
};

const VGParamTypeReverse = reverseMap(VGParamType);

const VGRenderingQuality = {
  VG_RENDERING_QUALITY_NONANTIALIASED: 0x1200,
  VG_RENDERING_QUALITY_FASTER: 0x1201,
  VG_RENDERING_QUALITY_BETTER: 0x1202 /* Default */,
};

const VGRenderingQualityReverse = reverseMap(VGRenderingQuality);

const VGPixelLayout = {
  VG_PIXEL_LAYOUT_UNKNOWN: 0x1300,
  VG_PIXEL_LAYOUT_RGB_VERTICAL: 0x1301,
  VG_PIXEL_LAYOUT_BGR_VERTICAL: 0x1302,
  VG_PIXEL_LAYOUT_RGB_HORIZONTAL: 0x1303,
  VG_PIXEL_LAYOUT_BGR_HORIZONTAL: 0x1304,
};

const VGPixelLayoutReverse = reverseMap(VGPixelLayout);

const VGMatrixMode = {
  VG_MATRIX_PATH_USER_TO_SURFACE: 0x1400,
  VG_MATRIX_IMAGE_USER_TO_SURFACE: 0x1401,
  VG_MATRIX_FILL_PAINT_TO_USER: 0x1402,
  VG_MATRIX_STROKE_PAINT_TO_USER: 0x1403,
  VG_MATRIX_GLYPH_USER_TO_SURFACE: 0x1404,
};

const VGMatrixModeReverse = reverseMap(VGMatrixMode);

const VGMaskOperation = {
  VG_CLEAR_MASK: 0x1500,
  VG_FILL_MASK: 0x1501,
  VG_SET_MASK: 0x1502,
  VG_UNION_MASK: 0x1503,
  VG_INTERSECT_MASK: 0x1504,
  VG_SUBTRACT_MASK: 0x1505,
};

const VGMaskOperationReverse = reverseMap(VGMaskOperation);

const VG_PATH_FORMAT_STANDARD = 0;

const VGPathDatatype = {
  VG_PATH_DATATYPE_S_8: 0,
  VG_PATH_DATATYPE_S_16: 1,
  VG_PATH_DATATYPE_S_32: 2,
  VG_PATH_DATATYPE_F: 3,
};

const VGPathDatatypeReverse = reverseMap(VGPathDatatype);

const VGPathAbsRel = {
  VG_ABSOLUTE: 0,
  VG_RELATIVE: 1,
};

const VGPathAbsRelReverse = reverseMap(VGPathAbsRel);

const VGPathSegment = {
  VG_CLOSE_PATH: 0 << 1,
  VG_MOVE_TO: 1 << 1,
  VG_LINE_TO: 2 << 1,
  VG_HLINE_TO: 3 << 1,
  VG_VLINE_TO: 4 << 1,
  VG_QUAD_TO: 5 << 1,
  VG_CUBIC_TO: 6 << 1,
  VG_SQUAD_TO: 7 << 1,
  VG_SCUBIC_TO: 8 << 1,
  VG_SCCWARC_TO: 9 << 1,
  VG_SCWARC_TO: 10 << 1,
  VG_LCCWARC_TO: 11 << 1,
  VG_LCWARC_TO: 12 << 1,
};

const VGPathSegmentReverse = reverseMap(VGPathSegment);

const VGPathCommand = {
  VG_MOVE_TO_ABS: VGPathSegment.VG_MOVE_TO | VGPathAbsRel.VG_ABSOLUTE,
  VG_MOVE_TO_REL: VGPathSegment.VG_MOVE_TO | VGPathAbsRel.VG_RELATIVE,
  VG_LINE_TO_ABS: VGPathSegment.VG_LINE_TO | VGPathAbsRel.VG_ABSOLUTE,
  VG_LINE_TO_REL: VGPathSegment.VG_LINE_TO | VGPathAbsRel.VG_RELATIVE,
  VG_HLINE_TO_ABS: VGPathSegment.VG_HLINE_TO | VGPathAbsRel.VG_ABSOLUTE,
  VG_HLINE_TO_REL: VGPathSegment.VG_HLINE_TO | VGPathAbsRel.VG_RELATIVE,
  VG_VLINE_TO_ABS: VGPathSegment.VG_VLINE_TO | VGPathAbsRel.VG_ABSOLUTE,
  VG_VLINE_TO_REL: VGPathSegment.VG_VLINE_TO | VGPathAbsRel.VG_RELATIVE,
  VG_QUAD_TO_ABS: VGPathSegment.VG_QUAD_TO | VGPathAbsRel.VG_ABSOLUTE,
  VG_QUAD_TO_REL: VGPathSegment.VG_QUAD_TO | VGPathAbsRel.VG_RELATIVE,
  VG_CUBIC_TO_ABS: VGPathSegment.VG_CUBIC_TO | VGPathAbsRel.VG_ABSOLUTE,
  VG_CUBIC_TO_REL: VGPathSegment.VG_CUBIC_TO | VGPathAbsRel.VG_RELATIVE,
  VG_SQUAD_TO_ABS: VGPathSegment.VG_SQUAD_TO | VGPathAbsRel.VG_ABSOLUTE,
  VG_SQUAD_TO_REL: VGPathSegment.VG_SQUAD_TO | VGPathAbsRel.VG_RELATIVE,
  VG_SCUBIC_TO_ABS: VGPathSegment.VG_SCUBIC_TO | VGPathAbsRel.VG_ABSOLUTE,
  VG_SCUBIC_TO_REL: VGPathSegment.VG_SCUBIC_TO | VGPathAbsRel.VG_RELATIVE,
  VG_SCCWARC_TO_ABS: VGPathSegment.VG_SCCWARC_TO | VGPathAbsRel.VG_ABSOLUTE,
  VG_SCCWARC_TO_REL: VGPathSegment.VG_SCCWARC_TO | VGPathAbsRel.VG_RELATIVE,
  VG_SCWARC_TO_ABS: VGPathSegment.VG_SCWARC_TO | VGPathAbsRel.VG_ABSOLUTE,
  VG_SCWARC_TO_REL: VGPathSegment.VG_SCWARC_TO | VGPathAbsRel.VG_RELATIVE,
  VG_LCCWARC_TO_ABS: VGPathSegment.VG_LCCWARC_TO | VGPathAbsRel.VG_ABSOLUTE,
  VG_LCCWARC_TO_REL: VGPathSegment.VG_LCCWARC_TO | VGPathAbsRel.VG_RELATIVE,
  VG_LCWARC_TO_ABS: VGPathSegment.VG_LCWARC_TO | VGPathAbsRel.VG_ABSOLUTE,
  VG_LCWARC_TO_REL: VGPathSegment.VG_LCWARC_TO | VGPathAbsRel.VG_RELATIVE,
};

const VGPathCommandReverse = reverseMap(VGPathCommand);

const VGPathCapabilities = {
  VG_PATH_CAPABILITY_APPEND_FROM: 1 << 0,
  VG_PATH_CAPABILITY_APPEND_TO: 1 << 1,
  VG_PATH_CAPABILITY_MODIFY: 1 << 2,
  VG_PATH_CAPABILITY_TRANSFORM_FROM: 1 << 3,
  VG_PATH_CAPABILITY_TRANSFORM_TO: 1 << 4,
  VG_PATH_CAPABILITY_INTERPOLATE_FROM: 1 << 5,
  VG_PATH_CAPABILITY_INTERPOLATE_TO: 1 << 6,
  VG_PATH_CAPABILITY_PATH_LENGTH: 1 << 7,
  VG_PATH_CAPABILITY_POINT_ALONG_PATH: 1 << 8,
  VG_PATH_CAPABILITY_TANGENT_ALONG_PATH: 1 << 9,
  VG_PATH_CAPABILITY_PATH_BOUNDS: 1 << 10,
  VG_PATH_CAPABILITY_PATH_TRANSFORMED_BOUNDS: 1 << 11,
  VG_PATH_CAPABILITY_ALL: (1 << 12) - 1,
};

const VGPathCapabilitiesReverse = reverseMap(VGPathCapabilities);

const VGPathParamType = {
  VG_PATH_FORMAT: 0x1600,
  VG_PATH_DATATYPE: 0x1601,
  VG_PATH_SCALE: 0x1602,
  VG_PATH_BIAS: 0x1603,
  VG_PATH_NUM_SEGMENTS: 0x1604,
  VG_PATH_NUM_COORDS: 0x1605,
};

const VGPathParamTypeReverse = reverseMap(VGPathParamType);

const VGCapStyle = {
  VG_CAP_BUTT: 0x1700,
  VG_CAP_ROUND: 0x1701,
  VG_CAP_SQUARE: 0x1702,
};

const VGCapStyleReverse = reverseMap(VGCapStyle);

const VGJoinStyle = {
  VG_JOIN_MITER: 0x1800,
  VG_JOIN_ROUND: 0x1801,
  VG_JOIN_BEVEL: 0x1802,
};

const VGJoinStyleReverse = reverseMap(VGJoinStyle);

const VGFillRule = {
  VG_EVEN_ODD: 0x1900,
  VG_NON_ZERO: 0x1901,
};

const VGFillRuleReverse = reverseMap(VGFillRule);

const VGPaintMode = {
  VG_STROKE_PATH: 1 << 0,
  VG_FILL_PATH: 1 << 1,
};

const VGPaintModeReverse = reverseMap(VGPaintMode);

const VGPaintParamType = {
  /* Color paint parameters */
  VG_PAINT_TYPE: 0x1a00,
  VG_PAINT_COLOR: 0x1a01,
  VG_PAINT_COLOR_RAMP_SPREAD_MODE: 0x1a02,
  VG_PAINT_COLOR_RAMP_PREMULTIPLIED: 0x1a07,
  VG_PAINT_COLOR_RAMP_STOPS: 0x1a03,

  /* Linear gradient paint parameters */
  VG_PAINT_LINEAR_GRADIENT: 0x1a04,

  /* Radial gradient paint parameters */
  VG_PAINT_RADIAL_GRADIENT: 0x1a05,

  /* Pattern paint parameters */
  VG_PAINT_PATTERN_TILING_MODE: 0x1a06,
};

const VGPaintParamTypeReverse = reverseMap(VGPaintParamType);

const VGPaintType = {
  VG_PAINT_TYPE_COLOR: 0x1b00,
  VG_PAINT_TYPE_LINEAR_GRADIENT: 0x1b01,
  VG_PAINT_TYPE_RADIAL_GRADIENT: 0x1b02,
  VG_PAINT_TYPE_PATTERN: 0x1b03,
};

const VGPaintTypeReverse = reverseMap(VGPaintType);

const VGColorRampSpreadMode = {
  VG_COLOR_RAMP_SPREAD_PAD: 0x1c00,
  VG_COLOR_RAMP_SPREAD_REPEAT: 0x1c01,
  VG_COLOR_RAMP_SPREAD_REFLECT: 0x1c02,
};

const VGColorRampSpreadModeReverse = reverseMap(VGColorRampSpreadMode);

const VGTilingMode = {
  VG_TILE_FILL: 0x1d00,
  VG_TILE_PAD: 0x1d01,
  VG_TILE_REPEAT: 0x1d02,
  VG_TILE_REFLECT: 0x1d03,
};

const VGTilingModeReverse = reverseMap(VGTilingMode);

const VGImageFormat = {
  /* RGB{A,X} channel ordering */
  VG_sRGBX_8888: 0,
  VG_sRGBA_8888: 1,
  VG_sRGBA_8888_PRE: 2,
  VG_sRGB_565: 3,
  VG_sRGBA_5551: 4,
  VG_sRGBA_4444: 5,
  VG_sL_8: 6,
  VG_lRGBX_8888: 7,
  VG_lRGBA_8888: 8,
  VG_lRGBA_8888_PRE: 9,
  VG_lL_8: 10,
  VG_A_8: 11,
  VG_BW_1: 12,
  VG_A_1: 13,
  VG_A_4: 14,

  /* {A,X}RGB channel ordering */
  VG_sXRGB_8888: 0 | (1 << 6),
  VG_sARGB_8888: 1 | (1 << 6),
  VG_sARGB_8888_PRE: 2 | (1 << 6),
  VG_sARGB_1555: 4 | (1 << 6),
  VG_sARGB_4444: 5 | (1 << 6),
  VG_lXRGB_8888: 7 | (1 << 6),
  VG_lARGB_8888: 8 | (1 << 6),
  VG_lARGB_8888_PRE: 9 | (1 << 6),

  /* BGR{A,X} channel ordering */
  VG_sBGRX_8888: 0 | (1 << 7),
  VG_sBGRA_8888: 1 | (1 << 7),
  VG_sBGRA_8888_PRE: 2 | (1 << 7),
  VG_sBGR_565: 3 | (1 << 7),
  VG_sBGRA_5551: 4 | (1 << 7),
  VG_sBGRA_4444: 5 | (1 << 7),
  VG_lBGRX_8888: 7 | (1 << 7),
  VG_lBGRA_8888: 8 | (1 << 7),
  VG_lBGRA_8888_PRE: 9 | (1 << 7),

  /* {A,X}BGR channel ordering */
  VG_sXBGR_8888: 0 | (1 << 6) | (1 << 7),
  VG_sABGR_8888: 1 | (1 << 6) | (1 << 7),
  VG_sABGR_8888_PRE: 2 | (1 << 6) | (1 << 7),
  VG_sABGR_1555: 4 | (1 << 6) | (1 << 7),
  VG_sABGR_4444: 5 | (1 << 6) | (1 << 7),
  VG_lXBGR_8888: 7 | (1 << 6) | (1 << 7),
  VG_lABGR_8888: 8 | (1 << 6) | (1 << 7),
  VG_lABGR_8888_PRE: 9 | (1 << 6) | (1 << 7),
};

const VGImageFormatReverse = reverseMap(VGImageFormat);

const VGImageQuality = {
  VG_IMAGE_QUALITY_NONANTIALIASED: 1 << 0,
  VG_IMAGE_QUALITY_FASTER: 1 << 1,
  VG_IMAGE_QUALITY_BETTER: 1 << 2,
};

const VGImageQualityReverse = reverseMap(VGImageQuality);

const VGImageParamType = {
  VG_IMAGE_FORMAT: 0x1e00,
  VG_IMAGE_WIDTH: 0x1e01,
  VG_IMAGE_HEIGHT: 0x1e02,
};

const VGImageParamTypeReverse = reverseMap(VGImageParamType);

const VGImageMode = {
  VG_DRAW_IMAGE_NORMAL: 0x1f00,
  VG_DRAW_IMAGE_MULTIPLY: 0x1f01,
  VG_DRAW_IMAGE_STENCIL: 0x1f02,
};

const VGImageModeReverse = reverseMap(VGImageMode);

const VGImageChannel = {
  VG_RED: 1 << 3,
  VG_GREEN: 1 << 2,
  VG_BLUE: 1 << 1,
  VG_ALPHA: 1 << 0,
};

const VGImageChannelReverse = reverseMap(VGImageChannel);

const VGBlendMode = {
  VG_BLEND_SRC: 0x2000,
  VG_BLEND_SRC_OVER: 0x2001,
  VG_BLEND_DST_OVER: 0x2002,
  VG_BLEND_SRC_IN: 0x2003,
  VG_BLEND_DST_IN: 0x2004,
  VG_BLEND_MULTIPLY: 0x2005,
  VG_BLEND_SCREEN: 0x2006,
  VG_BLEND_DARKEN: 0x2007,
  VG_BLEND_LIGHTEN: 0x2008,
  VG_BLEND_ADDITIVE: 0x2009,
};

const VGBlendModeReverse = reverseMap(VGBlendMode);

const VGFontParamType = {
  VG_FONT_NUM_GLYPHS: 0x2f00,
};

const VGFontParamTypeReverse = reverseMap(VGFontParamType);

const VGHardwareQueryType = {
  VG_IMAGE_FORMAT_QUERY: 0x2100,
  VG_PATH_DATATYPE_QUERY: 0x2101,
};

const VGHardwareQueryTypeReverse = reverseMap(VGHardwareQueryType);

const VGHardwareQueryResult = {
  VG_HARDWARE_ACCELERATED: 0x2200,
  VG_HARDWARE_UNACCELERATED: 0x2201,
};

const VGHardwareQueryResultReverse = reverseMap(VGHardwareQueryResult);

const VGStringID = {
  VG_VENDOR: 0x2300,
  VG_RENDERER: 0x2301,
  VG_VERSION: 0x2302,
  VG_EXTENSIONS: 0x2303,
};

const VGStringIDReverse = reverseMap(VGStringID);

const VGUErrorCode = {
  VGU_NO_ERROR: 0,
  VGU_BAD_HANDLE_ERROR: 0xf000,
  VGU_ILLEGAL_ARGUMENT_ERROR: 0xf001,
  VGU_OUT_OF_MEMORY_ERROR: 0xf002,
  VGU_PATH_CAPABILITY_ERROR: 0xf003,
  VGU_BAD_WARP_ERROR: 0xf004,
};

const VGUErrorCodeReverse = reverseMap(VGUErrorCode);

const VGUArcType = {
  VGU_ARC_OPEN: 0xf100,
  VGU_ARC_CHORD: 0xf101,
  VGU_ARC_PIE: 0xf102,
};

const VGUArcTypeReverse = reverseMap(VGUErrorCode);

const VGParamTypeKHR = {
  VG_MAX_AVERAGE_BLUR_DIMENSION_KHR: 0x116b,
  VG_AVERAGE_BLUR_DIMENSION_RESOLUTION_KHR: 0x116c,
  VG_MAX_AVERAGE_BLUR_ITERATIONS_KHR: 0x116d,
};

const VGParamTypeKHRReverse = reverseMap(VGParamTypeKHR);

const VGBlendModeKHR = {
  VG_BLEND_OVERLAY_KHR: 0x2010,
  VG_BLEND_HARDLIGHT_KHR: 0x2011,
  VG_BLEND_SOFTLIGHT_SVG_KHR: 0x2012,
  VG_BLEND_SOFTLIGHT_KHR: 0x2013,
  VG_BLEND_COLORDODGE_KHR: 0x2014,
  VG_BLEND_COLORBURN_KHR: 0x2015,
  VG_BLEND_DIFFERENCE_KHR: 0x2016,
  VG_BLEND_SUBTRACT_KHR: 0x2017,
  VG_BLEND_INVERT_KHR: 0x2018,
  VG_BLEND_EXCLUSION_KHR: 0x2019,
  VG_BLEND_LINEARDODGE_KHR: 0x201a,
  VG_BLEND_LINEARBURN_KHR: 0x201b,
  VG_BLEND_VIVIDLIGHT_KHR: 0x201c,
  VG_BLEND_LINEARLIGHT_KHR: 0x201d,
  VG_BLEND_PINLIGHT_KHR: 0x201e,
  VG_BLEND_HARDMIX_KHR: 0x201f,
  VG_BLEND_CLEAR_KHR: 0x2020,
  VG_BLEND_DST_KHR: 0x2021,
  VG_BLEND_SRC_OUT_KHR: 0x2022,
  VG_BLEND_DST_OUT_KHR: 0x2023,
  VG_BLEND_SRC_ATOP_KHR: 0x2024,
  VG_BLEND_DST_ATOP_KHR: 0x2025,
  VG_BLEND_XOR_KHR: 0x2026,
};

const VGBlendModeKHRReverse = reverseMap(VGBlendModeKHR);

const VGPfTypeKHR = {
  VG_PF_OBJECT_VISIBLE_FLAG_KHR: 1 << 0,
  VG_PF_KNOCKOUT_FLAG_KHR: 1 << 1,
  VG_PF_OUTER_FLAG_KHR: 1 << 2,
  VG_PF_INNER_FLAG_KHR: 1 << 3,
};

const VGPfTypeKHRReverse = reverseMap(VGPfTypeKHR);

const VGUErrorCodeKHR = {
  VGU_IMAGE_IN_USE_ERROR: 0xf010,
};

const VGUErrorCodeKHRReverse = reverseMap(VGUErrorCodeKHR);

const VGPaintParamTypeNds = {
  VG_PAINT_COLOR_RAMP_LINEAR_NDS: 0x1a10,
  VG_COLOR_MATRIX_NDS: 0x1a11,
  VG_PAINT_COLOR_TRANSFORM_LINEAR_NDS: 0x1a12,
};

const VGPaintParamTypeNdsReverse = reverseMap(VGPaintParamTypeNds);

const VGImageModeNds = {
  VG_DRAW_IMAGE_COLOR_MATRIX_NDS: 0x1f10,
};

const VGImageModeNdsReverse = reverseMap(VGImageModeNds);

const VGParamTypeNds = {
  VG_CLIP_MODE_NDS: 0x1180,
  VG_CLIP_LINES_NDS: 0x1181,
  VG_MAX_CLIP_LINES_NDS: 0x1182,
};

const VGParamTypeNdsReverse = reverseMap(VGParamTypeNds);

const VGClipModeNds = {
  VG_CLIPMODE_NONE_NDS: 0x3000,
  VG_CLIPMODE_CLIP_CLOSED_NDS: 0x3001,
  VG_CLIPMODE_CLIP_OPEN_NDS: 0x3002,
  VG_CLIPMODE_CULL_NDS: 0x3003,
};

const VGClipModeNdsReverse = reverseMap(VGClipModeNds);

const VGPathSegmentNds = {
  VG_RQUAD_TO_NDS: 13 << 1,
  VG_RCUBIC_TO_NDS: 14 << 1,
};

const VGPathSegmentNdsReverse = reverseMap(VGPathSegmentNds);

const VGPathCommandNds = {
  VG_RQUAD_TO_ABS_NDS:
    VGPathSegmentNds.VG_RQUAD_TO_NDS | VGPathSegmentNds.VG_ABSOLUTE,
  VG_RQUAD_TO_REL_NDS:
    VGPathSegmentNds.VG_RQUAD_TO_NDS | VGPathSegmentNds.VG_RELATIVE,
  VG_RCUBIC_TO_ABS_NDS:
    VGPathSegmentNds.VG_RCUBIC_TO_NDS | VGPathSegmentNds.VG_ABSOLUTE,
  VG_RCUBIC_TO_REL_NDS:
    VGPathSegmentNds.VG_RCUBIC_TO_NDS | VGPathSegmentNds.VG_RELATIVE,
};

const VGPathCommandNdsReverse = reverseMap(VGPathCommandNds);

function init() {
  openVG.nativeInit(screen);
}

function deinit() {
  openVG.nativeDeinit();
}

module.exports = Object.assign(openVG, {
  init,
  deinit,
  screen,
  VGErrorCode,
  VGErrorCodeReverse,
  VGParamType,
  VGParamTypeReverse,
  VGRenderingQuality,
  VGRenderingQualityReverse,
  VGPixelLayout,
  VGPixelLayoutReverse,
  VGMatrixMode,
  VGMatrixModeReverse,
  VGMaskOperation,
  VGMaskOperationReverse,
  VG_PATH_FORMAT_STANDARD,
  VGPathDatatype,
  VGPathDatatypeReverse,
  VGPathAbsRel,
  VGPathAbsRelReverse,
  VGPathSegment,
  VGPathSegmentReverse,
  VGPathCommand,
  VGPathCommandReverse,
  VGPathCapabilities,
  VGPathCapabilitiesReverse,
  VGPathParamType,
  VGPathParamTypeReverse,
  VGCapStyle,
  VGCapStyleReverse,
  VGJoinStyle,
  VGJoinStyleReverse,
  VGFillRule,
  VGFillRuleReverse,
  VGPaintMode,
  VGPaintModeReverse,
  VGPaintParamType,
  VGPaintParamTypeReverse,
  VGPaintType,
  VGPaintTypeReverse,
  VGColorRampSpreadMode,
  VGColorRampSpreadModeReverse,
  VGTilingMode,
  VGTilingModeReverse,
  VGImageFormat,
  VGImageFormatReverse,
  VGImageQuality,
  VGImageQualityReverse,
  VGImageParamType,
  VGImageParamTypeReverse,
  VGImageMode,
  VGImageModeReverse,
  VGImageChannel,
  VGImageChannelReverse,
  VGBlendMode,
  VGBlendModeReverse,
  VGFontParamType,
  VGFontParamTypeReverse,
  VGHardwareQueryType,
  VGHardwareQueryTypeReverse,
  VGHardwareQueryResult,
  VGHardwareQueryResultReverse,
  VGStringID,
  VGStringIDReverse,
  VGUErrorCode,
  VGUErrorCodeReverse,
  VGUArcType,
  VGUArcTypeReverse,
  VGParamTypeKHR,
  VGParamTypeKHRReverse,
  VGBlendModeKHR,
  VGBlendModeKHRReverse,
  VGPfTypeKHR,
  VGPfTypeKHRReverse,
  VGUErrorCodeKHR,
  VGUErrorCodeKHRReverse,
  VGPaintParamTypeNds,
  VGPaintParamTypeNdsReverse,
  VGImageModeNds,
  VGImageModeNdsReverse,
  VGParamTypeNds,
  VGParamTypeNdsReverse,
  VGClipModeNds,
  VGClipModeNdsReverse,
  VGPathSegmentNds,
  VGPathSegmentNdsReverse,
  VGPathCommandNds,
  VGPathCommandNdsReverse,
});
