#ifndef _WATLAS_H_
#define _WATLAS_H_

#include <stdlib.h>

#include "../source/xatlas/xatlas.h"

struct WAtlasResult {
  uint32_t width = 0;
  uint32_t height = 0;
  uint32_t atlasCount = 0;
  uint32_t chartCount = 0;
  uint32_t meshCount = 0;
  float texelsPerUnit = 0.0f;
};

struct WMeshDecl {
  uint32_t vertexCount = 0;
  uint32_t vertexPositionStride = 0;
  uint32_t vertexNormalStride = 0;
  uint32_t vertexUvStride = 0;
  uint32_t indexCount = 0;
  int32_t indexOffset = 0;
  uint32_t faceCount = 0;
  xatlas::IndexFormat indexFormat = xatlas::IndexFormat::UInt16;

  float epsilon = 1.192092896e-07f;
};

class WAtlas {
 public:
  WAtlas();
  ~WAtlas();

  xatlas::AddMeshError addMesh(WMeshDecl meshDecl, uint32_t meshCountHint = 0);

  void computeCharts(xatlas::ChartOptions options = xatlas::ChartOptions());
  void packCharts(xatlas::PackOptions options = xatlas::PackOptions());
  void generate(xatlas::ChartOptions chartOptions = xatlas::ChartOptions(), xatlas::PackOptions packOptions = xatlas::PackOptions());

  WAtlasResult getResult();

 private:
  xatlas::Atlas* atlas;
};

#endif