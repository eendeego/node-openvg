{
  "targets": [
    {
      "target_name": "openvg",
      "sources": [
        "src/openvg.cc",
        "src/egl.cc"
      ],
      "ldflags": [
        "-L/opt/vc/lib",
        "-lbrcmEGL",
        "-lbrcmGLESv2"
      ],
      "cflags": [
        "-DENABLE_GDB_JIT_INTERFACE",
        "-Wall"
      ],
      "include_dirs" : [
        "/opt/vc/include",
        "/opt/vc/include/interface/vcos/pthreads",
        "/opt/vc/include/interface/vmcs_host/linux"
      ]
    }
  ]
}
