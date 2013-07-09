# node-openvg (OpenVG bindings for node.js)

This module implemements bindings for OpenVG. It is targeted to the raspberry-pi.

It is heavily inspired in [node-sdl](https://github.com/creationix/node-sdl) and [openvg](https://github.com/ajstarks/openvg).

## 0. Installation

Just install via npm:

    npm install openvg

## 1. Usage

This library makes a 1:1 mapping between OpenVG 1.1 (as currently bundled on raspian) and JS (via node.js/v8 bindings).

### Differences from the OpenVG APIs.

All array based APIs are implemented via JS typed arrays. As such, length and offsets are supposed to be handled by the typed array API.

Due to exceptional slowness of typed array creation (should be avoided at all costs either in node and in browser code), even when using [subarray](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Typed_arrays/Int8Array?redirectlocale=en-US&redirectslug=JavaScript%2FTyped_arrays%2FInt8Array#subarray%28%29), the APIs that receive typed arrays as parameters (setXxxFV, setXxxIV, etc.) also present OL (Offset + Length) or O (Offset) variants.

### Examples

This library was created as a base for [openvg-canvas](https://github.com/luismreis/node-openvg-canvas), but can be used standalone.

Check the examples directory.

## License

(The MIT License)

Copyright (c) 2012, 2013 Luis Reis

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
