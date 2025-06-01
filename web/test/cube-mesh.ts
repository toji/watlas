export const CubeMeshIndexed = {
  vertexCount: 8,
  vertexPositionData: new Float32Array([
    -1, -1, -1,
     1, -1, -1,
    -1,  1, -1,
     1,  1, -1,
    -1, -1,  1,
     1, -1,  1,
    -1,  1,  1,
     1,  1,  1,
  ]),
  vertexPositionStride: 12,
  indexData: new Uint16Array([
    0, 1, 2,
    1, 3, 2,

    4, 5, 6,
    5, 7, 6,

    0, 4, 2,
    4, 6, 2,

    1, 5, 3,
    5, 7, 3,

    0, 4, 1,
    4, 5, 1,

    2, 6, 3,
    6, 7, 3,
  ]),
  indexCount: 36,
};

export const CubeMesh = {
  vertexCount: CubeMeshIndexed.indexCount,
  vertexPositionData: (() => {
    const pos = new Float32Array(CubeMeshIndexed.indexCount);
    for (let i = 0; i < CubeMeshIndexed.indexCount; ++i) {
      const index = CubeMeshIndexed.indexData[i];
      pos[(i*3) + 0] = CubeMeshIndexed.vertexPositionData[(index*3) + 0];
      pos[(i*3) + 1] = CubeMeshIndexed.vertexPositionData[(index*3) + 1];
      pos[(i*3) + 2] = CubeMeshIndexed.vertexPositionData[(index*3) + 2];
    }
    return pos;
  })(),
  vertexPositionStride: 12,
};