// TypeScript bindings for emscripten-generated code.  Automatically generated at compile time.
interface WasmModule {
}

export interface ClassHandle {
  isAliasOf(other: ClassHandle): boolean;
  delete(): void;
  deleteLater(): this;
  isDeleted(): boolean;
  // @ts-ignore - If targeting lower than ESNext, this symbol might not exist.
  [Symbol.dispose](): void;
  clone(): this;
}
export interface WVertexVector extends ClassHandle {
  size(): number;
  get(_0: number): WVertex | undefined;
  push_back(_0: WVertex): void;
  resize(_0: number, _1: WVertex): void;
  set(_0: number, _1: WVertex): boolean;
}

export interface WMeshVector extends ClassHandle {
  size(): number;
  get(_0: number): WMesh | undefined;
  push_back(_0: WMesh): void;
  resize(_0: number, _1: WMesh): void;
  set(_0: number, _1: WMesh): boolean;
}

export interface WChartTypeValue<T extends number> {
  value: T;
}
export type WChartType = WChartTypeValue<0>|WChartTypeValue<1>|WChartTypeValue<2>|WChartTypeValue<3>|WChartTypeValue<4>;

export type WChart = {
  faceArray: any,
  atlasIndex: number,
  faceCount: number,
  type: WChartType,
  material: number
};

export type WUv = [ number, number ];

export type WVertex = {
  atlasIndex: number,
  chartIndex: number,
  uv: WUv,
  xref: number
};

export type WMesh = {
  indexArray: any,
  vertexArray: WVertexVector,
  chartCount: number,
  indexCount: number,
  vertexCount: number
};

export type WAtlasResult = {
  meshes: WMeshVector,
  utilization: any,
  width: number,
  height: number,
  atlasCount: number,
  chartCount: number,
  meshCount: number,
  texelsPerUnit: number
};

export interface WIndexFormatValue<T extends number> {
  value: T;
}
export type WIndexFormat = WIndexFormatValue<0>|WIndexFormatValue<1>;

export type WMeshDecl = {
  vertexPositionData: any,
  vertexNormalData: any | undefined,
  vertexUvData: any | undefined,
  indexData: any | undefined,
  faceMaterialData: any | undefined,
  faceVertexCount: any | undefined,
  vertexCount: number,
  vertexPositionStride: number,
  vertexNormalStride: number | undefined,
  vertexUvStride: number | undefined,
  indexCount: number | undefined,
  indexOffset: number | undefined,
  faceCount: number | undefined,
  indexFormat: WIndexFormat | undefined,
  epsilon: number | undefined
};

export type WUvMeshDecl = {
  vertexUvData: any,
  indexData: any | undefined,
  faceMaterialData: any | undefined,
  vertexCount: number,
  vertexStride: number,
  indexCount: number | undefined,
  indexOffset: number | undefined,
  indexFormat: WIndexFormat | undefined
};

export interface WAddMeshErrorValue<T extends number> {
  value: T;
}
export type WAddMeshError = WAddMeshErrorValue<0>|WAddMeshErrorValue<1>|WAddMeshErrorValue<2>|WAddMeshErrorValue<3>|WAddMeshErrorValue<4>;

export type WChartOptions = {
  maxChartArea: number | undefined,
  maxBoundaryLength: number | undefined,
  normalDeviationWeight: number | undefined,
  roundnessWeight: number | undefined,
  straightnessWeight: number | undefined,
  normalSeamWeight: number | undefined,
  textureSeamWeight: number | undefined,
  maxCost: number | undefined,
  maxIterations: number | undefined,
  useInputMeshUvs: boolean | undefined,
  fixWinding: boolean | undefined
};

export type WPackOptions = {
  maxChartSize: number | undefined,
  padding: number | undefined,
  texelsPerUnit: number | undefined,
  resolution: number | undefined,
  bilinear: boolean | undefined,
  blockAlign: boolean | undefined,
  bruteForce: boolean | undefined,
  rotateChartsToAxis: boolean | undefined,
  rotateCharts: boolean | undefined
};

export interface WAtlas extends ClassHandle {
  addMesh(_0: WMeshDecl): WAddMeshError;
  addUvMesh(_0: WUvMeshDecl): WAddMeshError;
  computeCharts(_0: WChartOptions): void;
  packCharts(_0: WPackOptions): void;
  generate(_0: WChartOptions, _1: WPackOptions): void;
  getResult(): WAtlasResult;
}

interface EmbindModule {
  WVertexVector: {
    new(): WVertexVector;
  };
  WMeshVector: {
    new(): WMeshVector;
  };
  WChartType: {Planar: WChartTypeValue<0>, Ortho: WChartTypeValue<1>, LSCM: WChartTypeValue<2>, Piecewise: WChartTypeValue<3>, Invalid: WChartTypeValue<4>};
  WIndexFormat: {UInt16: WIndexFormatValue<0>, UInt32: WIndexFormatValue<1>};
  WAddMeshError: {Success: WAddMeshErrorValue<0>, Error: WAddMeshErrorValue<1>, IndexOutOfRange: WAddMeshErrorValue<2>, InvalidFaceVertexCount: WAddMeshErrorValue<3>, InvalidIndexCount: WAddMeshErrorValue<4>};
  WAtlas: {
    new(): WAtlas;
  };
}

export type MainModule = WasmModule & EmbindModule;
export default function MainModuleFactory (options?: unknown): Promise<MainModule>;
