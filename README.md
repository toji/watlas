## watlas

[![Build Status](https://github.com/toji/watlas/actions/workflows/build.yml/badge.svg)](https://github.com/toji/watlas/actions/workflows/build.yml) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

watlas is a WebAssembly wrapper for [xatlas](https://github.com/jpcy/xatlas), making is accessible to JavaScript in browsers and node.js. The primary difference between this as and other JS wrappers like [xatlas.js](https://github.com/repalash/xatlas.js) is that this wrapper attempts to expose the native API as closely as possible. This makes it potentially harder to use compared to xatlas.js for simple cases but should be more flexible for a wider range of uses.

This wrapper was written with the intent to be used by [gltf-transform](https://gltf-transform.dev/)

### Why the name watlas?

Because the much better name of xatlas.js was already taken. :)

The "w" in this case stands for "web", and happens to be right next to "x" in the (english) alphabet.

### Library Use

#### Initialization

You can install it locally via npm and import for node_modules

```
# Command Line
$> npm install watlas
```

```js
import * as watlas from 'watlas' // For node.js

import * as watlas from '../node_modules/watlas/dist/watlas.js' // For the browser
```

Or import the library from a CDN:

```js
// Import from a CDN
import * as watlas from 'https://cdn.jsdelivr.net/npm/watlas'
```

Whichever way you import it, to begin using the library you need to call `watlas.Initialize()` and
wait on the returned promise before making any other WAtlas calls.

```js
// IMPORANT! Initialize the WASM module prior to calling any API methods.
await watlas.Initialize();
```

#### Simple use

```js
// Create an empty atlas
const atlas = new watlas.Atlas();

// Load your mesh data into Typed Arrays
const positions = new Float32Array([ /*...*/ ]);
const indices = new Uint16Array([ /*...*/ ]);

// Add one or more meshes
atlas.addMesh({
  vertexPositionData: positions,
  vertexCount: 256,
  vertexPositionStride: 12, // In bytes
  indexData: indices, // Index format will be determined by Typed Array type
  indexCount: 512,
});

// Call generate on the atlas
atlas.generate(
  {}, // Chart options
  {}  // Pack options
);

// Packed atlas data is not available on the atlas object.
// See below for how to interpret results
console.log(`Atlas size: (${atlas.width}, ${atlas.height})`);

// When finished, manually delete the atlas
atlas.delete();
```

#### Tools/Editor integration

```js
// Add one or more meshes
atlas.addMesh({

});

// computeCharts segments meshes into charts and parameterize
atlas.computeCharts({
  maxChartArea: 256
});

// packCharts packs charts into one or more atlases. Can call multiple times
// to tweak options like texel scale and resolution
atlas.packCharts({
  padding: 1,
  bilinear: false,
});
```

#### Pack multiple atlases into a single atlas

```js
// Add one or more UV meshes
atlas.addUvMesh({

});

// Call packCharts
atlas.packCharts({
  padding: 1,
  bilinear: false,
});
```

#### Using Results

```js
// Loop through each mesh that was part of the atlas.
// (Returned in the order that they were added to the atlas)
for (let i = 0; i < atlas.meshCount; ++i) {
  const mesh = atlas.getMesh(i);

  // Allocate a large enough Uint32Array to hold the updated indicies.
  const indices = new Uint32Array(mesh.indexCount);
  mesh.getIndexArray(indices); // Populates indices with the updated mesh index data

  for (let j = 0; j < mesh.vertexCount; ++j) {
    const vertex = mesh.getVertex(j);
    // UVs are returned in texels, and will need to be normalized for most use cases.
    const u = vertex.uv[0] / atlas.width;
    const v = vertex.uv[1] / atlas.height;

    // xref gives the original vertex index associated with this new vertex.
    // Copy the attributes from the original mesh at that index.
    emitVertex(positions[vertex.xref], normals[vertex.xref], [u, v]);
  }
}
```

### Building

First, make sure [emscripten is installed](https://emscripten.org/docs/getting_started/downloads.html). watlas requires emscripten 4.0.9 or greater to support `std::optional` fields.

Also ensure that npm is installed, which is packaged with [node.js](https://nodejs.org/en)

Then run:

```
npm install
npm run build
```

This should produce `watlas.js`, `watlas.wasm`, and `watlas.d.ts` in the `dist\` folder.

> **Note for WSL users**
>
> If you are building via Windows Subsystem for Linux (WSL) you may run into the following error message while building:
> ```
> error while loading shared libraries: libatomic.so.1: cannot open shared object file: No such file or directory
> ```
> This can be resolved by running `apt install libatomic1` to install the missing library

### Testing

After building, you can run the test suite by calling

```
npm run test
```

## About xatlas

xatlas is a small C++11 library with no external dependencies that generates unique texture coordinates suitable for baking lightmaps or texture painting.

It is an independent fork of [thekla_atlas](https://github.com/Thekla/thekla_atlas), used by [The Witness](https://en.wikipedia.org/wiki/The_Witness_(2016_video_game)).

See the [xatlas GitHub](https://github.com/jpcy/xatlas) for more details.
