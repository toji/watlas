// Copyright (c) 2025 Brandon Jones
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// Hand-written wrapper around the WASM module to improve API ergonomics.

function assertAddMeshSuccess(err) {
  switch(err) {
    case 0: // Success
      return;
    case 1: // Error
      throw new Error('Unspecified error occured while adding mesh');
    case 2: // IndexOutOfRange
      throw new Error('IndexOutOfRange error while adding mesh');
    case 3: // InvalidFaceVertexCount
      throw new Error('InvalidFaceVertexCount error while adding mesh');
    case 4: // InvalidIndexCount
      throw new Error('InvalidIndexCount error while adding mesh');
    default:
      throw new Error(`Unknown error (${err}) occured while adding mesh`);
  }
}

let modulePromise;
let module;
export async function Initialize() {
  if (!modulePromise) {
    modulePromise = Module();
    module = await modulePromise;
  } else {
    await modulePromise;
  }
}

export const ChartType = {
  Planar: 0,
  Ortho: 1,
  LSCM: 2,
  Piecewise: 3,
  Invalid: 4,
}

export class Atlas {
  #impl;
  constructor() {
    if (!module) {
      throw new Error('watlas not initialized! Call `await watlas.Initialize()` before constructing an Atlas instance.')
    }
    this.#impl = new module.Atlas();
  }

  delete() {
    this.#impl.delete();
    this.#impl = null;
  }

  addMesh(meshDecl) {
    assertAddMeshSuccess(this.#impl.addMesh(meshDecl));
  }

  addUvMesh(meshDecl) {
    assertAddMeshSuccess(this.#impl.addUvMesh(meshDecl));
  }

  computeCharts(options) {
    this.#impl.computeCharts(options);
  }

  packCharts(options) {
    this.#impl.packCharts(options);
  }

  generate(chartOptions = {}, packOptions = {}) {
    this.#impl.generate(chartOptions, packOptions);
  }

  getMesh(index) { return this.#impl.getMesh(index); }
  getUtilization(jsArray) { return this.#impl.getUtilization(jsArray); }
  get width() { return this.#impl.width; }
  get height() { return this.#impl.height; }
  get atlasCount() { return this.#impl.atlasCount; }
  get chartCount() { return this.#impl.chartCount; }
  get meshCount() { return this.#impl.meshCount; }
  get texelsPerUnit() { return this.#impl.texelsPerUnit; }
}
