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

export enum ChartType {
  Planar = 0,
  Ortho = 1,
  LSCM = 2,
  Piecewise = 3,
  Invalid = 4,
};

export type Chart = {
  getFaceArray(jsArray: Uint32Array): boolean;
  get faceCount(): number,
  get atlasIndex(): number,
  get type(): ChartType,
  get material(): number
};

export type Vertex = {
  atlasIndex: number,
  chartIndex: number,
  uv: [number, number],
  xref: number
};

export declare class Mesh {
  getChart(index: number): Chart;
  getIndexArray(jsArray: Uint32Array): boolean;
  getVertex(index: number): Vertex;
  get chartCount(): number;
  get indexCount(): number;
  get vertexCount(): number;
};

export type MeshDecl = {
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

export type UvMeshDecl = {
  vertexUvData: Float32Array,
  vertexCount: number,
  vertexStride: number,

  indexData?: Uint32Array | Uint16Array,
  faceMaterialData?: Uint32Array,
  indexCount?: number,
  indexOffset?: number,
};

export type ChartOptions = {
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

export type PackOptions = {
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

export declare class Atlas {
  new(): Atlas;
  delete(): void;

  addMesh(meshDecl: MeshDecl): void;
  addUvMesh(meshDecl: UvMeshDecl): void;
  computeCharts(options: ChartOptions): void;
  packCharts(options: PackOptions): void;
  generate(chartOptions?: ChartOptions, packOptions?: PackOptions): void;

  getMesh(index: number): Mesh;
  getUtilization(jsArray: Float32Array): boolean;
  get width(): number;
  get height(): number;
  get atlasCount(): number;
  get chartCount(): number;
  get meshCount(): number;
  get texelsPerUnit(): number;
}

export function Initialize(): Promise<void>;
