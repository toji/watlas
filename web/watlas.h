#ifndef _WATLAS_H_
#define _WATLAS_H_

#include <stdlib.h>
#include <optional>
#include <emscripten/bind.h>

#include "../source/xatlas/xatlas.h"

struct WChart {
  emscripten::val faceArray;
  uint32_t atlasIndex; // Sub-atlas index.
  uint32_t faceCount;
  xatlas::ChartType type;
  uint32_t material;
};

struct WVertex {
  int32_t atlasIndex; // Sub-atlas index. -1 if the vertex doesn't exist in any atlas.
  int32_t chartIndex; // -1 if the vertex doesn't exist in any chart.
  float uv[2]; // Not normalized - values are in Atlas width and height range.
  uint32_t xref; // Index of input vertex from which this output vertex originated.
};

struct WMesh{
  std::vector<WChart> chartArray;
  emscripten::val indexArray;
  std::vector<WVertex> vertexArray;
  uint32_t chartCount;
  uint32_t indexCount;
  uint32_t vertexCount;
};

struct WAtlasResult {
  //uint32_t *image;
  std::vector<WMesh> meshes;
  emscripten::val utilization;
  uint32_t width;
  uint32_t height;
  uint32_t atlasCount;
  uint32_t chartCount;
  uint32_t meshCount;
  float texelsPerUnit;
};

struct WMeshDecl {
  emscripten::val vertexPositionData;
  std::optional<emscripten::val> vertexNormalData;
  std::optional<emscripten::val> vertexUvData;
  std::optional<emscripten::val> indexData;

  std::optional<emscripten::val> faceMaterialData;
  std::optional<emscripten::val> faceVertexCount;

  uint32_t vertexCount;
  uint32_t vertexPositionStride;
  std::optional<uint32_t> vertexNormalStride;
  std::optional<uint32_t> vertexUvStride;
  std::optional<uint32_t> indexCount;
  std::optional<int32_t> indexOffset;
  std::optional<uint32_t> faceCount;
  std::optional<xatlas::IndexFormat> indexFormat;
  std::optional<float> epsilon;
};

struct WUvMeshDecl {
  emscripten::val vertexUvData;
  std::optional<emscripten::val> indexData;
  std::optional<emscripten::val> faceMaterialData;
  uint32_t vertexCount;
  uint32_t vertexStride;
  std::optional<uint32_t> indexCount;
  std::optional<int32_t> indexOffset;
  std::optional<xatlas::IndexFormat> indexFormat;
};

struct WChartOptions {
  std::optional<float> maxChartArea;
  std::optional<float> maxBoundaryLength;

  std::optional<float> normalDeviationWeight;
  std::optional<float> roundnessWeight;
  std::optional<float> straightnessWeight;
  std::optional<float> normalSeamWeight;
  std::optional<float> textureSeamWeight;

  std::optional<float> maxCost;
  std::optional<uint32_t> maxIterations;

  std::optional<bool> useInputMeshUvs;
  std::optional<bool> fixWinding;
};

struct WPackOptions {
  std::optional<uint32_t> maxChartSize;
  std::optional<uint32_t> padding;
  std::optional<float> texelsPerUnit;
  std::optional<uint32_t> resolution;
  std::optional<bool> bilinear;
  std::optional<bool> blockAlign;
  std::optional<bool> bruteForce;
  std::optional<bool> rotateChartsToAxis;
  std::optional<bool> rotateCharts;
};

class WAtlas {
 public:
  WAtlas();
  ~WAtlas();

  xatlas::AddMeshError addMesh(WMeshDecl meshDecl);
  xatlas::AddMeshError addUvMesh(WUvMeshDecl meshDecl);

  void computeCharts(WChartOptions options);
  void packCharts(WPackOptions options);
  void generate(WChartOptions chartOptions, WPackOptions packOptions);

  WAtlasResult getResult();

 private:
  xatlas::Atlas* atlas;
};

#endif