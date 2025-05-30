export type WChart = {
  faceArray: Uint32Array,
  atlasIndex: number,
  type: WAtlas.ChartType,
  material: number
};

export type WVertex = {
  atlasIndex: number,
  chartIndex: number,
  uv: [number, number],
  xref: number
};

export type WMesh = {
  chartArray: WChart[],
  indexArray: Uint32Array,
  vertexArray: WVertex[],
};

export type WAtlasResult = {
  meshes: WMesh[],
  utilization: Float32Array,
  width: number,
  height: number,
  atlasCount: number,
  chartCount: number,
  texelsPerUnit: number
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

export interface WAtlas {
  static async Initialize(): Promise<void>;

  AddMeshError: {
    Success: 0,
    Error: 1,
    IndexOutOfRange: 2,
    InvalidFaceVertexCount: 3,
    InvalidIndexCount: 4,
  };

  ChartType: {
    Planar: 0,
    Ortho: 1,
    LSCM: 2,
    Piecewise: 3,
    Invalid: 4,
  };

  new(): WAtlas;
  delete(): void;

  addMesh(meshDecl: WMeshDecl): WAtlas.AddMeshError;
  addUvMesh(meshDecl: WUvMeshDecl): WAtlas.AddMeshError;
  computeCharts(options: WChartOptions): void;
  packCharts(options: WPackOptions): void;
  generate(chartOptions: WChartOptions, packOptions: WPackOptions): void;
  getResult(): WAtlasResult;
}
