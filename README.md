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
    node-waf configure build

To test:

    bin/node-openvg sample/hello.js

## License

(The MIT License)

Copyright (c) 2012 Luis Reis

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
