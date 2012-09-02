# node-openvg ( OpenVG bindings for node.js )

This module implemements bindings for OpenVG. It is targeted to the raspberry-pi.

It is heavily inspired in [node-sdl]: https://github.com/creationix/node-sdl and [openvg]: https://github.com/ajstarks/openvg .

## 0. Installation

The same instructions for node-sdl apply here, namely: get the package and build
it through node-waf.

To install node on the raspberry follow the instructions on this [gist]: https://gist.github.com/3245130 . Node 0.8.8 is
known to work.

For now there are no extra dependencies other than node itself.

Fetch the source:

    git clone https://github.com/luismreis/node-openvg.git

Build the package:

    cd node-openvg
    node-waf configure build

To test:

    node sample/hello.js
