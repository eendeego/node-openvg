# node-openvg (OpenVG bindings for node.js)

This module implemements bindings for OpenVG. It is targeted to the raspberry-pi.

It is heavily inspired in [node-sdl](https://github.com/creationix/node-sdl) and [openvg](https://github.com/ajstarks/openvg).

## 0. Installation

The same instructions for node-sdl apply here, namely: get the package and build
it through node-waf.

To install node on the raspberry follow the instructions on the gist [Node.js for Raspberry Pi](https://gist.github.com/3245130). Node 0.8.8 is known to work.

For now there are no extra dependencies other than node itself.

Fetch the source:

    git clone https://github.com/luismreis/node-openvg.git

Build the package:

    cd node-openvg
    node-waf --targets=node-openvg,init-egl configure build

To test:

    (LD_PRELOAD=build/Release/init-egl.node node sample/hello.js)

The LD_PRELOAD trick is required because EGL initialization interferes with V8 contexts. This seems to be related with memory regions being mapped/allocated by EGL. By initializing EGL before any node/V8 execution, at least the bootstrap process works.
