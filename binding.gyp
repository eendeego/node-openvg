{
  'targets': [
    {
      # have to specify 'liblib' here since gyp will remove the first one :\
      'target_name': 'libnode-openvg',
      'sources': [
        'src/openvg.cc',
        'src/egl.cc'
      ],
      'ldflags': [
        "-L/opt/vc/lib",
        "-lGLESv2"
      ],
      'cflags': [
        "-DENABLE_GDB_JIT_INTERFACE",
        "-Wall",
        "-I/opt/vc/include",
        "-I/opt/vc/include/interface/vcos/pthreads"
      ],
    },
    {
      # have to specify 'liblib' here since gyp will remove the first one :\
      'target_name': 'libinit-egl',
      'sources': [
        'src/init-egl.c'
      ],
      'ldflags': [
        "-L/opt/vc/lib",
        "-lGLESv2"
      ],
      'cflags': [
        "-DENABLE_GDB_JIT_INTERFACE",
        "-Wall",
        "-I/opt/vc/include",
        "-I/opt/vc/include/interface/vcos/pthreads"
      ],
    },
  ]
}
