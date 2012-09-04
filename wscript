#!/usr/bin/env python

from os import popen

srcdir = '.'
blddir = 'build'
VERSION = '0.0.1'

def set_options(opt):
  opt.tool_options('compiler_cxx')

def configure(conf):
  conf.check_tool('compiler_cxx')
  conf.check_tool('node_addon')

def build(bld):
  obj = bld.new_task_gen('cxx', 'shlib', 'node_addon')
  obj.target = "node-openvg"
  obj.cxxflags = ["-pthread", "-Wall", "-DARG_CHECKS"]
  obj.linkflags = ["-L/opt/vc/lib", "-lGLESv2"]
  obj.includes = ["/opt/vc/include", "/opt/vc/include/interface/vcos/pthreads"]
  obj.source = ["src/openvg.cc", "src/egl.cc"]

  obj = bld.new_task_gen('cxx', 'shlib', 'node_addon')
  obj.target = "init-egl"
  obj.cxxflags = ["-Wall", "-pthread", "-fPIC"]
  obj.linkflags = ["-shared", "-Wl,-soname,libinit-egl.so", "-L/opt/vc/lib", "-lGLESv2"]
  obj.includes = ["/opt/vc/include", "/opt/vc/include/interface/vcos/pthreads"]
  obj.source = ["src/init-egl.cc"]
