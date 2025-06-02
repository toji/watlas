#include "watlas.h"
#include <stdio.h>

namespace watlas {

#define SET_OPTIONAL(out, in, propName) \
  if (in.propName.has_value()) { out.propName = in.propName.value(); }

template <typename T> std::vector<T> vecFromJSArray(const emscripten::val &jsArray) {
    const auto len = jsArray["length"].as<uint32_t>();

    std::vector<T> heapVec;
    heapVec.resize(len);

    emscripten::val memoryView{emscripten::typed_memory_view(len, heapVec.data())};
    memoryView.call<void>("set", jsArray);

    return heapVec;
}

template <typename T> bool fillJSArrayWithData(const uint32_t dataLen, const T* data, const emscripten::val &jsArray) {
    const auto jsLen = jsArray["length"].as<uint32_t>();
    if (jsLen < dataLen) {
      return false;
    }

    emscripten::val memoryView{emscripten::typed_memory_view(dataLen, data)};
    jsArray.call<void>("set", memoryView);

    return true;
}

Atlas::Atlas() : atlas(xatlas::Create()) {
}

Atlas::~Atlas() {
  xatlas::Destroy(atlas);
}

uint32_t Atlas::addMesh(MeshDecl meshDecl) {
  xatlas::MeshDecl mesh;

  std::vector<float> posData = vecFromJSArray<float>(meshDecl.vertexPositionData);
  mesh.vertexPositionData = posData.data();

  std::vector<float> normalData;
  if (meshDecl.vertexNormalData.has_value()) {
    normalData = vecFromJSArray<float>(meshDecl.vertexNormalData.value());
    mesh.vertexNormalData = normalData.data();
  }

  std::vector<float> uvData;
  if (meshDecl.vertexUvData.has_value()) {
    uvData = vecFromJSArray<float>(meshDecl.vertexUvData.value());
    mesh.vertexNormalData = uvData.data();
  }

  std::vector<uint16_t> indexData16;
  std::vector<uint32_t> indexData32;
  if (meshDecl.indexData.has_value()) {
    emscripten::val v = meshDecl.indexData.value();
    const auto bytesPerElement = v["BYTES_PER_ELEMENT"].as<uint32_t>();

    switch(bytesPerElement) {
      case 4:
        indexData32 = vecFromJSArray<uint32_t>(v);
        mesh.indexData = indexData32.data();
        mesh.indexFormat = xatlas::IndexFormat::UInt32;
        break;
      case 2:
        indexData16 = vecFromJSArray<uint16_t>(v);
        mesh.indexData = indexData16.data();
        mesh.indexFormat = xatlas::IndexFormat::UInt16;
        break;
      default:
        return static_cast<uint32_t>(xatlas::AddMeshError::Error);
    }
  }

  std::vector<uint32_t> faceMaterialData;
  if (meshDecl.faceMaterialData.has_value()) {
    faceMaterialData = vecFromJSArray<uint32_t>(meshDecl.faceMaterialData.value());
    mesh.faceMaterialData = faceMaterialData.data();
  }

  std::vector<uint8_t> faceVertexCount;
  if (meshDecl.faceVertexCount.has_value()) {
    faceVertexCount = vecFromJSArray<uint8_t>(meshDecl.faceVertexCount.value());
    mesh.faceVertexCount = faceVertexCount.data();
  }

  mesh.vertexCount = meshDecl.vertexCount;
  mesh.vertexPositionStride = meshDecl.vertexPositionStride;

  SET_OPTIONAL(mesh, meshDecl, vertexNormalStride)
  SET_OPTIONAL(mesh, meshDecl, vertexUvStride)
  SET_OPTIONAL(mesh, meshDecl, indexCount)
  SET_OPTIONAL(mesh, meshDecl, indexOffset)
  SET_OPTIONAL(mesh, meshDecl, faceCount)
  SET_OPTIONAL(mesh, meshDecl, epsilon)

  return static_cast<uint32_t>(xatlas::AddMesh(atlas, mesh));
}

uint32_t Atlas::addUvMesh(UvMeshDecl meshDecl) {
  xatlas::UvMeshDecl mesh;

  std::vector<float> vertexUvData = vecFromJSArray<float>(meshDecl.vertexUvData);
  mesh.vertexUvData = vertexUvData.data();

  std::vector<uint16_t> indexData16;
  std::vector<uint32_t> indexData32;
  if (meshDecl.indexData.has_value()) {
    emscripten::val v = meshDecl.indexData.value();
    const auto bytesPerElement = v["BYTES_PER_ELEMENT"].as<uint32_t>();

    switch(bytesPerElement) {
      case 4:
        indexData32 = vecFromJSArray<uint32_t>(v);
        mesh.indexData = indexData32.data();
        mesh.indexFormat = xatlas::IndexFormat::UInt32;
        break;
      case 2:
        indexData16 = vecFromJSArray<uint16_t>(v);
        mesh.indexData = indexData16.data();
        mesh.indexFormat = xatlas::IndexFormat::UInt16;
        break;
      default:
        return static_cast<uint32_t>(xatlas::AddMeshError::Error);
    }
  }

  std::vector<uint32_t> faceMaterialData;
  if (meshDecl.faceMaterialData.has_value()) {
    faceMaterialData = vecFromJSArray<uint32_t>(meshDecl.faceMaterialData.value());
    mesh.faceMaterialData = faceMaterialData.data();
  }

  mesh.vertexCount = meshDecl.vertexCount;
  mesh.vertexStride = meshDecl.vertexStride;

  SET_OPTIONAL(mesh, meshDecl, indexCount)
  SET_OPTIONAL(mesh, meshDecl, indexOffset)
  SET_OPTIONAL(mesh, meshDecl, indexFormat)

  return static_cast<uint32_t>(xatlas::AddUvMesh(atlas, mesh));
}

void Atlas::computeCharts(ChartOptions options) {
  xatlas::ChartOptions chart;
  SET_OPTIONAL(chart, options, maxChartArea)
  SET_OPTIONAL(chart, options, maxBoundaryLength)
  SET_OPTIONAL(chart, options, normalDeviationWeight)
  SET_OPTIONAL(chart, options, roundnessWeight)
  SET_OPTIONAL(chart, options, straightnessWeight)
  SET_OPTIONAL(chart, options, normalSeamWeight)
  SET_OPTIONAL(chart, options, textureSeamWeight)
  SET_OPTIONAL(chart, options, maxCost)
  SET_OPTIONAL(chart, options, maxIterations)
  SET_OPTIONAL(chart, options, useInputMeshUvs)
  SET_OPTIONAL(chart, options, fixWinding)

  xatlas::ComputeCharts(atlas, chart);
}

void Atlas::packCharts(PackOptions options) {
  xatlas::PackOptions pack;
  SET_OPTIONAL(pack, options, maxChartSize)
  SET_OPTIONAL(pack, options, padding)
  SET_OPTIONAL(pack, options, texelsPerUnit)
  SET_OPTIONAL(pack, options, resolution)
  SET_OPTIONAL(pack, options, bilinear)
  SET_OPTIONAL(pack, options, blockAlign)
  SET_OPTIONAL(pack, options, bruteForce)
  SET_OPTIONAL(pack, options, rotateChartsToAxis)
  SET_OPTIONAL(pack, options, rotateCharts)

  xatlas::PackCharts(atlas, pack);
}

void Atlas::generate(ChartOptions chartOptions, PackOptions packOptions) {
  computeCharts(chartOptions);
  packCharts(packOptions);
}

emscripten::val Atlas::getMesh(uint32_t index) const {
  if (index >= atlas->meshCount) {
    return emscripten::val::undefined();
  }

  return emscripten::val(atlas->meshes[index]);
}

bool Atlas::getUtilization(const emscripten::val& jsArray) const {
  return fillJSArrayWithData(atlas->atlasCount, atlas->utilization, jsArray);
}

uint32_t Atlas::width() const {
  return atlas->width;
}

uint32_t Atlas::height() const {
  return atlas->height;
}

uint32_t Atlas::atlasCount() const {
  return atlas->atlasCount;
}

uint32_t Atlas::chartCount() const {
  return atlas->chartCount;
}

uint32_t Atlas::meshCount() const {
  return atlas->meshCount;
}

float Atlas::texelsPerUnit() const {
  return atlas->texelsPerUnit;
}

emscripten::val getMeshChart(const xatlas::Mesh& mesh, uint32_t index) {
  if (index >= mesh.chartCount) {
    return emscripten::val::undefined();
  }

  return emscripten::val(mesh.chartArray[index]);
}

bool getMeshIndexArray(const xatlas::Mesh& mesh, const emscripten::val& jsArray) {
  return fillJSArrayWithData(mesh.indexCount, mesh.indexArray, jsArray);
}

emscripten::val getMeshVertex(const xatlas::Mesh& mesh, uint32_t index) {
  if (index >= mesh.vertexCount) {
    return emscripten::val::undefined();
  }

  return emscripten::val(mesh.vertexArray[index]);
}

uint32_t getChartType(const xatlas::Chart& chart) {
  return static_cast<uint32_t>(chart.type);
}

bool getChartFaceArray(const xatlas::Chart& chart, const emscripten::val& jsArray) {
  return fillJSArrayWithData(chart.faceCount, chart.faceArray, jsArray);
}

EMSCRIPTEN_BINDINGS(watlas) {
    emscripten::register_optional<uint32_t>();
    emscripten::register_optional<int32_t>();
    emscripten::register_optional<float>();
    emscripten::register_optional<bool>();
    emscripten::register_optional<xatlas::IndexFormat>();
    emscripten::register_optional<emscripten::val>();

    emscripten::enum_<xatlas::ChartType>("ChartType")
      .value("Planar", xatlas::ChartType::Planar)
      .value("Ortho", xatlas::ChartType::Ortho)
      .value("LSCM", xatlas::ChartType::LSCM)
      .value("Piecewise", xatlas::ChartType::Piecewise)
      .value("Invalid", xatlas::ChartType::Invalid);

    emscripten::class_<xatlas::Chart>("Chart")
      .function("getFaceArray", &getChartFaceArray)
      .property("atlasIndex", &xatlas::Chart::atlasIndex)
      .property("faceCount", &xatlas::Chart::faceCount)
      .property("type", &getChartType)
      .property("material", &xatlas::Chart::material);

    emscripten::value_array<std::array<float, 2>>("Uv")
      .element(emscripten::index<0>())
      .element(emscripten::index<1>());

    emscripten::value_object<xatlas::Vertex>("Vertex")
      .field("atlasIndex", &xatlas::Vertex::atlasIndex)
      .field("chartIndex", &xatlas::Vertex::chartIndex)
      .field("uv", &xatlas::Vertex::uv)
      .field("xref", &xatlas::Vertex::xref);

    emscripten::class_<xatlas::Mesh>("Mesh")
      .function("getChart", &getMeshChart)
      .function("getIndexArray", &getMeshIndexArray)
      .function("getVertex", &getMeshVertex)
      .property("chartCount", &xatlas::Mesh::chartCount)
      .property("indexCount", &xatlas::Mesh::indexCount)
      .property("vertexCount", &xatlas::Mesh::vertexCount);

    emscripten::enum_<xatlas::IndexFormat>("IndexFormat")
      .value("UInt16", xatlas::IndexFormat::UInt16)
      .value("UInt32", xatlas::IndexFormat::UInt32);

    emscripten::value_object<MeshDecl>("MeshDecl")
      .field("vertexPositionData", &MeshDecl::vertexPositionData)
      .field("vertexNormalData", &MeshDecl::vertexNormalData)
      .field("vertexUvData", &MeshDecl::vertexUvData)
      .field("indexData", &MeshDecl::indexData)
      .field("faceMaterialData", &MeshDecl::faceMaterialData)
      .field("faceVertexCount", &MeshDecl::faceVertexCount)
      .field("vertexCount", &MeshDecl::vertexCount)
      .field("vertexPositionStride", &MeshDecl::vertexPositionStride)
      .field("vertexNormalStride", &MeshDecl::vertexNormalStride)
      .field("vertexUvStride", &MeshDecl::vertexUvStride)
      .field("indexCount", &MeshDecl::indexCount)
      .field("indexOffset", &MeshDecl::indexOffset)
      .field("faceCount", &MeshDecl::faceCount)
      .field("indexFormat", &MeshDecl::indexFormat)
      .field("epsilon", &MeshDecl::epsilon);

    emscripten::value_object<UvMeshDecl>("UvMeshDecl")
      .field("vertexUvData", &UvMeshDecl::vertexUvData)
      .field("indexData", &UvMeshDecl::indexData)
      .field("faceMaterialData", &UvMeshDecl::faceMaterialData)
      .field("vertexCount", &UvMeshDecl::vertexCount)
      .field("vertexStride", &UvMeshDecl::vertexStride)
      .field("indexCount", &UvMeshDecl::indexCount)
      .field("indexOffset", &UvMeshDecl::indexOffset)
      .field("indexFormat", &UvMeshDecl::indexFormat);

    emscripten::value_object<ChartOptions>("ChartOptions")
      .field("maxChartArea", &ChartOptions::maxChartArea)
      .field("maxBoundaryLength", &ChartOptions::maxBoundaryLength)
      .field("normalDeviationWeight", &ChartOptions::normalDeviationWeight)
      .field("roundnessWeight", &ChartOptions::roundnessWeight)
      .field("straightnessWeight", &ChartOptions::straightnessWeight)
      .field("normalSeamWeight", &ChartOptions::normalSeamWeight)
      .field("textureSeamWeight", &ChartOptions::textureSeamWeight)
      .field("maxCost", &ChartOptions::maxCost)
      .field("maxIterations", &ChartOptions::maxIterations)
      .field("useInputMeshUvs", &ChartOptions::useInputMeshUvs)
      .field("fixWinding", &ChartOptions::fixWinding);

    emscripten::value_object<PackOptions>("PackOptions")
      .field("maxChartSize", &PackOptions::maxChartSize)
      .field("padding", &PackOptions::padding)
      .field("texelsPerUnit", &PackOptions::texelsPerUnit)
      .field("resolution", &PackOptions::resolution)
      .field("bilinear", &PackOptions::bilinear)
      .field("blockAlign", &PackOptions::blockAlign)
      .field("bruteForce", &PackOptions::bruteForce)
      .field("rotateChartsToAxis", &PackOptions::rotateChartsToAxis)
      .field("rotateCharts", &PackOptions::rotateCharts);

    emscripten::class_<Atlas>("Atlas")
      .constructor()
      .function("addMesh", &Atlas::addMesh)
      .function("addUvMesh", &Atlas::addUvMesh)
      .function("computeCharts", &Atlas::computeCharts)
      .function("packCharts", &Atlas::packCharts)
      .function("generate", &Atlas::generate)
      .function("getMesh", &Atlas::getMesh)
      .function("getUtilization", &Atlas::getUtilization)
      .property("width", &Atlas::width)
      .property("height", &Atlas::height)
      .property("atlasCount", &Atlas::atlasCount)
      .property("chartCount", &Atlas::chartCount)
      .property("meshCount", &Atlas::meshCount)
      .property("texelsPerUnit", &Atlas::texelsPerUnit)
      ;
}

}  // namespace watlas
