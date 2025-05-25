#include "watlas.h"
#include <emscripten/bind.h>
//#include <unordered_map>

/*typedef std::unordered_map<WAtlasHandle, xatlas::Atlas*> AtlasHandleMap;

WAtlasHandle nextAtlasHandle = 1;
AtlasHandleMap atlasMap;*/

WAtlas::WAtlas() : atlas(xatlas::Create()) {
}

WAtlas::~WAtlas() {
  xatlas::Destroy(atlas);
}

xatlas::AddMeshError WAtlas::addMesh(WMeshDecl meshDecl, uint32_t meshCountHint) {
  xatlas::MeshDecl mesh;

  mesh.vertexCount = meshDecl.vertexCount;
	mesh.vertexPositionStride = meshDecl.vertexPositionStride;
	mesh.vertexNormalStride = meshDecl.vertexNormalStride;
	mesh.vertexUvStride = meshDecl.vertexUvStride;
	mesh.indexCount = meshDecl.indexCount;
	mesh.indexOffset = meshDecl.indexOffset;
	mesh.faceCount = meshDecl.faceCount;
	mesh.indexFormat = meshDecl.indexFormat;

  mesh.epsilon = meshDecl.epsilon;

  return xatlas::AddMesh(atlas, mesh, meshCountHint);
}

void WAtlas::computeCharts(xatlas::ChartOptions options) {
  xatlas::ComputeCharts(atlas, options);
}

void WAtlas::packCharts(xatlas::PackOptions options) {
  xatlas::PackCharts(atlas, options);
}

void WAtlas::generate(xatlas::ChartOptions chartOptions, xatlas::PackOptions packOptions) {
  xatlas::Generate(atlas, chartOptions, packOptions);
}

WAtlasResult WAtlas::getResult() {
  WAtlasResult result;

  result.width = atlas->width;
  result.height = atlas->height;
  result.atlasCount = atlas->atlasCount;
  result.chartCount = atlas->chartCount;
  result.meshCount = atlas->meshCount;
  result.texelsPerUnit = atlas->texelsPerUnit;

  return result;
}

/*MeshBufferInfo createMesh(uint32_t vertexCount, uint32_t indexCount, bool normals, bool uvs) {
  MeshBufferInfo meshBufferInfo;
  meshBufferInfo.meshId = nextMeshId++;

  meshDecl = new xatlas::MeshDecl;
  meshDecl->vertexCount = vertexCount;
  meshDecl->indexCount = indexCount;

  meshDecl->indexData = new int[indexCount];
  meshDecl->indexFormat = xatlas::IndexFormat::UInt16;
  meshBufferInfo.indexOffset = (uint32_t)meshDecl->indexData;

  meshDecl->vertexPositionData = new float[vertexCount * 3];
  meshDecl->vertexPositionStride = sizeof(float) * 3;
  meshBufferInfo.positionOffset = (uint32_t)meshDecl->vertexPositionData;

  if (normals) {
    meshDecl->vertexNormalData = new float[vertexCount * 3];
    meshDecl->vertexNormalStride = sizeof(float) * 3;
    meshBufferInfo.normalOffset = (uint32_t)meshDecl->vertexNormalData;
  }

  if (uvs) {
    meshDecl->vertexUvData = new float[vertexCount * 2];
    meshDecl->vertexUvStride = sizeof(float) * 2;
    meshBufferInfo.uvOffset = (uint32_t)meshDecl->vertexUvData;
  }

  return meshBufferInfo;
}

UvMeshBufferInfo createUvMesh(int vertexCount, int indexCount) {
  UvMeshBufferInfo uvMeshBufferInfo;
  uvMeshBufferInfo.meshId = nextMeshId++;

  uvMeshDecl = new xatlas::UvMeshDecl;
  uvMeshDecl->vertexCount = vertexCount;
  uvMeshDecl->indexCount = indexCount;

  uvMeshDecl->indexData = new uint16_t[indexCount];
  uvMeshDecl->indexFormat = xatlas::IndexFormat::UInt16;
  uvMeshBufferInfo.indexOffset = (uint32_t)uvMeshDecl->indexData;

  uvMeshDecl->vertexUvData = new float[vertexCount * 2];
  uvMeshDecl->vertexStride = sizeof(float) * 2;
  uvMeshBufferInfo.uvOffset = (uint32_t)uvMeshDecl->vertexUvData;

  return uvMeshBufferInfo;
}

uint32_t addMesh() {
  return (uint32_t)xatlas::AddMesh(atlas, *meshDecl);
}

uint32_t addUvMesh() {
  return (uint32_t)xatlas::AddUvMesh(atlas, *uvMeshDecl);
}

void generateAtlas() {
  xatlas::Generate(atlas);
}

AtlasMeshBufferInfo getMeshData(uint32_t meshId) {
  const xatlas::Mesh &mesh = atlas->meshes[meshId];

  uint32_t* originalIndexArray = new uint32_t[mesh.vertexCount];
  float* uvArray = new float[mesh.vertexCount * 2];

  for (uint32_t i = 0; i < mesh.vertexCount; i++) {
    const xatlas::Vertex &vertex = mesh.vertexArray[i];
    originalIndexArray[i] = vertex.xref;
    uvArray[i * 2] = vertex.uv[0] / atlas->width;
    uvArray[i * 2 + 1] = vertex.uv[1] / atlas->height;
  }

  AtlasMeshBufferInfo atlasMeshBufferInfo;

  atlasMeshBufferInfo.newVertexCount = mesh.vertexCount;
  atlasMeshBufferInfo.newIndexCount = mesh.indexCount;
  atlasMeshBufferInfo.indexOffset = (uint32_t)mesh.indexArray;
  atlasMeshBufferInfo.originalIndexOffset = (uint32_t)originalIndexArray;
  atlasMeshBufferInfo.uvOffset = (uint32_t)uvArray;

  return atlasMeshBufferInfo;
}

void destroyAtlas() {
  xatlas::Destroy(atlas);
  nextMeshId = 0;
}*/

EMSCRIPTEN_BINDINGS(watlas) {
    emscripten::value_object<WAtlasResult>("WAtlasResult")
      .field("width", &WAtlasResult::width)
      .field("height", &WAtlasResult::height)
      .field("atlasCount", &WAtlasResult::atlasCount)
      .field("chartCount", &WAtlasResult::chartCount)
      .field("meshCount", &WAtlasResult::meshCount)
      .field("texelsPerUnit", &WAtlasResult::texelsPerUnit);

    emscripten::value_object<WMeshDecl>("WMeshDecl")
      .field("vertexCount", &WMeshDecl::vertexCount)
      .field("vertexPositionStride", &WMeshDecl::vertexPositionStride)
      .field("vertexNormalStride", &WMeshDecl::vertexNormalStride)
      .field("vertexUvStride", &WMeshDecl::vertexUvStride)
      .field("indexCount", &WMeshDecl::indexCount)
      .field("indexOffset", &WMeshDecl::indexOffset)
      .field("faceCount", &WMeshDecl::faceCount)
      .field("indexFormat", &WMeshDecl::indexFormat);

    emscripten::value_object<xatlas::ChartOptions>("WChartOptions")
      .field("maxChartArea", &xatlas::ChartOptions::maxChartArea)
      .field("maxBoundaryLength", &xatlas::ChartOptions::maxBoundaryLength)
      .field("normalDeviationWeight", &xatlas::ChartOptions::normalDeviationWeight)
      .field("roundnessWeight", &xatlas::ChartOptions::roundnessWeight)
      .field("straightnessWeight", &xatlas::ChartOptions::straightnessWeight)
      .field("normalSeamWeight", &xatlas::ChartOptions::normalSeamWeight)
      .field("textureSeamWeight", &xatlas::ChartOptions::textureSeamWeight)
      .field("maxCost", &xatlas::ChartOptions::maxCost)
      .field("maxIterations", &xatlas::ChartOptions::maxIterations)
      .field("useInputMeshUvs", &xatlas::ChartOptions::useInputMeshUvs)
      .field("fixWinding", &xatlas::ChartOptions::fixWinding);

    emscripten::value_object<xatlas::PackOptions>("WPackOptions")
      .field("maxChartSize", &xatlas::PackOptions::maxChartSize)
      .field("padding", &xatlas::PackOptions::padding)
      .field("texelsPerUnit", &xatlas::PackOptions::texelsPerUnit)
      .field("resolution", &xatlas::PackOptions::resolution)
      .field("bilinear", &xatlas::PackOptions::bilinear)
      .field("blockAlign", &xatlas::PackOptions::blockAlign)
      .field("bruteForce", &xatlas::PackOptions::bruteForce)
      .field("rotateChartsToAxis", &xatlas::PackOptions::rotateChartsToAxis)
      .field("rotateCharts", &xatlas::PackOptions::rotateCharts);

    emscripten::class_<WAtlas>("WAtlas")
      .constructor()
      .function("addMesh", &WAtlas::addMesh)
      .function("computeCharts", &WAtlas::computeCharts)
      .function("packCharts", &WAtlas::packCharts)
      .function("generate", &WAtlas::generate)
      .function("getResult", &WAtlas::getResult);
}
