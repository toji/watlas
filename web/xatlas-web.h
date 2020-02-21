#ifndef _XATLAS_WEB_H_
#define _XATLAS_WEB_H_

#include <stdlib.h>
#include <emscripten/bind.h>
#include "../xatlas.h"

struct MeshBufferInfo {
  uint32_t meshId;
  uint32_t indexOffset;
  uint32_t positionOffset;
  uint32_t normalOffset;
  uint32_t uvOffset;
};

struct UvMeshBufferInfo {
  uint32_t meshId;
  uint32_t indexOffset;
  uint32_t uvOffset;
};

struct AtlasMeshBufferInfo {
  uint32_t newVertexCount;
  uint32_t newIndexCount;
  uint32_t indexOffset;
  uint32_t originalIndexOffset;
  uint32_t uvOffset;
};

extern void createAtlas();

extern MeshBufferInfo createMesh(uint32_t vertexCount, uint32_t indexCount, bool normals, bool uvs);

extern UvMeshBufferInfo createUvMesh(int vertexCount, int indexCount);

extern xatlas::AddMeshError::Enum addMesh();

extern xatlas::AddMeshError::Enum addUvMesh();

extern void generateAtlas();

extern AtlasMeshBufferInfo getMeshData(uint32_t meshId);

extern void destroyAtlas();

#endif