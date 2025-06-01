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

// TypeScript definitions (manually maintained.)

export type WChart = {
  getFaceArray(jsArray: Uint32Array): boolean;
  get faceCount(): number,
  get atlasIndex(): number,
  get type(): WAtlas.ChartType,
  get material(): number
};

export type WVertex = {
  atlasIndex: number,
  chartIndex: number,
  uv: [number, number],
  xref: number
};

export declare class WMesh {
  getChart(index: number): WChart;
  getIndexArray(jsArray: Uint32Array): bool;
  getVertex(index: number): WVertex;
  get chartCount(): number;
  get indexCount(): number;
  get vertexCount(): number;
};

export type WMeshDecl = {
  vertexPositionData: Float32Array,
  vertexCount: number,
  vertexPositionStride: number,

  vertexNormalData?: Float32Array,
  vertexUvData?: Float32Array,
  indexData?: Uint32Array | Uint16Array,
  faceMaterialData?: Uint32Array,
  faceVertexCount?: Uint32Array,
  vertexNormalStride?: number,
  vertexUvStride?: number,
  indexCount?: number,
  indexOffset?: number,
  faceCount?: number,
  epsilon?: number,
};

export type WUvMeshDecl = {
  vertexUvData: Float32Array,
  vertexCount: number,
  vertexStride: number,

  indexData?: Uint32Array | Uint16Array,
  faceMaterialData?: Uint32Array,
  indexCount?: number,
  indexOffset?: number,
};

export type WChartOptions = {
  maxChartArea?: number,
  maxBoundaryLength?: number,
  normalDeviationWeight?: number,
  roundnessWeight?: number,
  straightnessWeight?: number,
  normalSeamWeight?: number,
  textureSeamWeight?: number,
  maxCost?: number,
  maxIterations?: number,
  useInputMeshUvs?: boolean,
  fixWinding?: boolean,
};

export type WPackOptions = {
  maxChartSize?: number,
  padding?: number,
  texelsPerUnit?: number,
  resolution?: number,
  bilinear?: boolean,
  blockAlign?: boolean,
  bruteForce?: boolean,
  rotateChartsToAxis?: boolean,
  rotateCharts?: boolean,
};

export declare class WAtlas {
  static async Initialize(): Promise<void>;

  ChartType: {
    Planar: 0,
    Ortho: 1,
    LSCM: 2,
    Piecewise: 3,
    Invalid: 4,
  };

  new(): WAtlas;
  delete(): void;

  addMesh(meshDecl: WMeshDecl): void;
  addUvMesh(meshDecl: WUvMeshDecl): void;
  computeCharts(options: WChartOptions): void;
  packCharts(options: WPackOptions): void;
  generate(chartOptions: WChartOptions, packOptions: WPackOptions): void;

  getMesh(index: number): WMesh;
  getUtilization(jsArray: Float32Array): boolean;
  get width(): number;
  get height(): number;
  get atlasCount(): number;
  get chartCount(): number;
  get meshCount(): number;
  get texelsPerUnit(): number;
}
