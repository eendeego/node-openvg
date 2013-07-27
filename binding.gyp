{
  "variables": {
    "buffer_impl" : "<!(node -pe 'v=process.versions.node.split(\".\");v[0] > 0 || v[0] == 0 && v[1] >= 11 ? \"POS_0_11\" : \"PRE_0_11\"')",
    "callback_style" : "<!(node -pe 'v=process.versions.v8.split(\".\");v[0] > 3 || v[0] == 3 && v[1] >= 20 ? \"POS_3_20\" : \"PRE_3_20\"')"
  },
  "targets": [
    {
      "target_name": "openvg",
      "sources": [
        "src/openvg.cc",
        "src/egl.cc"
      ],
      "defines": [
        "NODE_BUFFER_TYPE_<(buffer_impl)",
        "TYPED_ARRAY_TYPE_<(buffer_impl)",
        "V8_CALLBACK_STYLE_<(callback_style)"
      ],
      "ldflags": [
        "-L/opt/vc/lib",
        "-lGLESv2"
      ],
      "cflags": [
        "-DENABLE_GDB_JIT_INTERFACE",
        "-Wall",
        "-I/opt/vc/include",
        "-I/opt/vc/include/interface/vcos/pthreads",
        "-I/opt/vc/include/interface/vmcs_host/linux"
      ],
    },
    {
      "target_name": "init-egl",
      "sources": [
        "src/init-egl.cc"
      ],
      "ldflags": [
        "-L/opt/vc/lib",
        "-lGLESv2"
      ],
      "cflags": [
        "-DENABLE_GDB_JIT_INTERFACE",
        "-Wall",
        "-I/opt/vc/include",
        "-I/opt/vc/include/interface/vcos/pthreads",
        "-I/opt/vc/include/interface/vmcs_host/linux"
      ],
    },
  ]
}
