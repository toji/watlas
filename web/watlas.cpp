#include "watlas.h"
#include <stdio.h>

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

WAtlasImpl::WAtlasImpl() : atlas(xatlas::Create()) {
}

WAtlasImpl::~WAtlasImpl() {
  xatlas::Destroy(atlas);
}

xatlas::AddMeshError WAtlasImpl::addMesh(WMeshDecl meshDecl) {
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
        break;
      case 2:
        indexData16 = vecFromJSArray<uint16_t>(v);
        mesh.indexData = indexData16.data();
        break;
      default:
        return xatlas::AddMeshError::Error;
        break;
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
  SET_OPTIONAL(mesh, meshDecl, indexFormat)
  SET_OPTIONAL(mesh, meshDecl, epsilon)

  return xatlas::AddMesh(atlas, mesh);
}

xatlas::AddMeshError WAtlasImpl::addUvMesh(WUvMeshDecl meshDecl) {
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
        break;
      case 2:
        indexData16 = vecFromJSArray<uint16_t>(v);
        mesh.indexData = indexData16.data();
        break;
      default:
        return xatlas::AddMeshError::Error;
        break;
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

  return xatlas::AddUvMesh(atlas, mesh);
}

void WAtlasImpl::computeCharts(WChartOptions options) {
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

void WAtlasImpl::packCharts(WPackOptions options) {
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

void WAtlasImpl::generate(WChartOptions chartOptions, WPackOptions packOptions) {
  computeCharts(chartOptions);
  packCharts(packOptions);
}

WAtlasResult WAtlasImpl::getResult() {
  WAtlasResult result;

  for (uint32_t i = 0; i < atlas->meshCount; ++i) {
    const xatlas::Mesh* atlasMesh = &(atlas->meshes[i]);

    WMesh mesh;

    for (uint32_t j = 0; j < atlasMesh->chartCount; ++j) {
      const xatlas::Chart* meshChart = &(atlasMesh->chartArray[j]);

      WChart chart;
      chart.faceArray = emscripten::val(
        emscripten::typed_memory_view(meshChart->faceCount, meshChart->faceArray)
      );
      chart.atlasIndex = meshChart->atlasIndex;
      chart.faceCount = meshChart->faceCount;
      chart.type = meshChart->type;
      chart.material = meshChart->material;

      mesh.chartArray.push_back(chart);
    }

    mesh.indexArray = emscripten::val(
      emscripten::typed_memory_view(atlasMesh->indexCount, atlasMesh->indexArray)
    );

    for (uint32_t j = 0; j < atlasMesh->vertexCount; ++j) {
      mesh.vertexArray.push_back(atlasMesh->vertexArray[j]);
    }

    mesh.chartCount = atlasMesh->chartCount;
    mesh.indexCount = atlasMesh->indexCount;
    mesh.vertexCount = atlasMesh->vertexCount;

    result.meshes.push_back(mesh);
  }


  result.utilization = emscripten::val(
    emscripten::typed_memory_view(atlas->atlasCount, atlas->utilization)
  );

  result.width = atlas->width;
  result.height = atlas->height;
  result.atlasCount = atlas->atlasCount;
  result.chartCount = atlas->chartCount;
  result.meshCount = atlas->meshCount;
  result.texelsPerUnit = atlas->texelsPerUnit;

  return result;
}

EMSCRIPTEN_BINDINGS(watlas) {
    emscripten::register_optional<uint32_t>();
    emscripten::register_optional<int32_t>();
    emscripten::register_optional<float>();
    emscripten::register_optional<bool>();
    emscripten::register_optional<xatlas::IndexFormat>();
    emscripten::register_optional<emscripten::val>();

    emscripten::register_vector<WChart>("WChartVector");
    emscripten::register_vector<xatlas::Vertex>("WVertexVector");
    emscripten::register_vector<WMesh>("WMeshVector");

    emscripten::enum_<xatlas::ChartType>("WChartType")
      .value("Planar", xatlas::ChartType::Planar)
      .value("Ortho", xatlas::ChartType::Ortho)
      .value("LSCM", xatlas::ChartType::LSCM)
      .value("Piecewise", xatlas::ChartType::Piecewise)
      .value("Invalid", xatlas::ChartType::Invalid);

    emscripten::value_object<WChart>("WChart")
      .field("faceArray", &WChart::faceArray)
      .field("atlasIndex", &WChart::atlasIndex)
      .field("faceCount", &WChart::faceCount)
      .field("type", &WChart::type)
      .field("material", &WChart::material);

    emscripten::value_array<std::array<float, 2>>("WUv")
      .element(emscripten::index<0>())
      .element(emscripten::index<1>());

    emscripten::value_object<xatlas::Vertex>("WVertex")
      .field("atlasIndex", &xatlas::Vertex::atlasIndex)
      .field("chartIndex", &xatlas::Vertex::chartIndex)
      .field("uv", &xatlas::Vertex::uv)
      .field("xref", &xatlas::Vertex::xref);

    emscripten::value_object<WMesh>("WMesh")
      .field("chartArray", &WMesh::chartArray)
      .field("indexArray", &WMesh::indexArray)
      .field("vertexArray", &WMesh::vertexArray)
      .field("chartCount", &WMesh::chartCount)
      .field("indexCount", &WMesh::indexCount)
      .field("vertexCount", &WMesh::vertexCount);

    emscripten::value_object<WAtlasResult>("WAtlasResult")
      .field("meshes", &WAtlasResult::meshes)
      .field("utilization", &WAtlasResult::utilization)
      .field("width", &WAtlasResult::width)
      .field("height", &WAtlasResult::height)
      .field("atlasCount", &WAtlasResult::atlasCount)
      .field("chartCount", &WAtlasResult::chartCount)
      .field("meshCount", &WAtlasResult::meshCount)
      .field("texelsPerUnit", &WAtlasResult::texelsPerUnit);

    emscripten::enum_<xatlas::IndexFormat>("WIndexFormat")
      .value("UInt16", xatlas::IndexFormat::UInt16)
      .value("UInt32", xatlas::IndexFormat::UInt32);

    emscripten::value_object<WMeshDecl>("WMeshDecl")
      .field("vertexPositionData", &WMeshDecl::vertexPositionData)
      .field("vertexNormalData", &WMeshDecl::vertexNormalData)
      .field("vertexUvData", &WMeshDecl::vertexUvData)
      .field("indexData", &WMeshDecl::indexData)
      .field("faceMaterialData", &WMeshDecl::faceMaterialData)
      .field("faceVertexCount", &WMeshDecl::faceVertexCount)
      .field("vertexCount", &WMeshDecl::vertexCount)
      .field("vertexPositionStride", &WMeshDecl::vertexPositionStride)
      .field("vertexNormalStride", &WMeshDecl::vertexNormalStride)
      .field("vertexUvStride", &WMeshDecl::vertexUvStride)
      .field("indexCount", &WMeshDecl::indexCount)
      .field("indexOffset", &WMeshDecl::indexOffset)
      .field("faceCount", &WMeshDecl::faceCount)
      .field("indexFormat", &WMeshDecl::indexFormat)
      .field("epsilon", &WMeshDecl::epsilon);

    emscripten::value_object<WUvMeshDecl>("WUvMeshDecl")
      .field("vertexUvData", &WUvMeshDecl::vertexUvData)
      .field("indexData", &WUvMeshDecl::indexData)
      .field("faceMaterialData", &WUvMeshDecl::faceMaterialData)
      .field("vertexCount", &WUvMeshDecl::vertexCount)
      .field("vertexStride", &WUvMeshDecl::vertexStride)
      .field("indexCount", &WUvMeshDecl::indexCount)
      .field("indexOffset", &WUvMeshDecl::indexOffset)
      .field("indexFormat", &WUvMeshDecl::indexFormat);

    emscripten::enum_<xatlas::AddMeshError>("WAddMeshError")
      .value("Success", xatlas::AddMeshError::Success)
      .value("Error", xatlas::AddMeshError::Error)
      .value("IndexOutOfRange", xatlas::AddMeshError::IndexOutOfRange)
      .value("InvalidFaceVertexCount", xatlas::AddMeshError::InvalidFaceVertexCount)
      .value("InvalidIndexCount", xatlas::AddMeshError::InvalidIndexCount);

    emscripten::value_object<WChartOptions>("WChartOptions")
      .field("maxChartArea", &WChartOptions::maxChartArea)
      .field("maxBoundaryLength", &WChartOptions::maxBoundaryLength)
      .field("normalDeviationWeight", &WChartOptions::normalDeviationWeight)
      .field("roundnessWeight", &WChartOptions::roundnessWeight)
      .field("straightnessWeight", &WChartOptions::straightnessWeight)
      .field("normalSeamWeight", &WChartOptions::normalSeamWeight)
      .field("textureSeamWeight", &WChartOptions::textureSeamWeight)
      .field("maxCost", &WChartOptions::maxCost)
      .field("maxIterations", &WChartOptions::maxIterations)
      .field("useInputMeshUvs", &WChartOptions::useInputMeshUvs)
      .field("fixWinding", &WChartOptions::fixWinding);

    emscripten::value_object<WPackOptions>("WPackOptions")
      .field("maxChartSize", &WPackOptions::maxChartSize)
      .field("padding", &WPackOptions::padding)
      .field("texelsPerUnit", &WPackOptions::texelsPerUnit)
      .field("resolution", &WPackOptions::resolution)
      .field("bilinear", &WPackOptions::bilinear)
      .field("blockAlign", &WPackOptions::blockAlign)
      .field("bruteForce", &WPackOptions::bruteForce)
      .field("rotateChartsToAxis", &WPackOptions::rotateChartsToAxis)
      .field("rotateCharts", &WPackOptions::rotateCharts);

    emscripten::class_<WAtlasImpl>("WAtlasImpl")
      .constructor()
      .function("addMesh", &WAtlasImpl::addMesh)
      .function("addUvMesh", &WAtlasImpl::addUvMesh)
      .function("computeCharts", &WAtlasImpl::computeCharts)
      .function("packCharts", &WAtlasImpl::packCharts)
      .function("generate", &WAtlasImpl::generate)
      .function("getResult", &WAtlasImpl::getResult);
}
