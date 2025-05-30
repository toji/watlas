export async function InitializeWAtlas() {
  const WAtlasModule = await Module();
  return WAtlasModule.WAtlas;
}

export class WAtlas {
  static #modulePromise;
  static #module;
  static async Initialize() {
    if (!WAtlas.#modulePromise) {
      WAtlas.#modulePromise = Module();
      WAtlas.#module = await WAtlas.#modulePromise;
    } else {
      await WAtlas.#modulePromise;
    }
  }

  static AddMeshError = {
    Success: 0,
    Error: 1,
    IndexOutOfRange: 2,
    InvalidFaceVertexCount: 3,
    InvalidIndexCount: 4,
  }

  static IndexFormat = {
    UInt16: 0,
    UInt32: 1,
  }

  static ChartType = {
    Planar: 0,
    Ortho: 1,
    LSCM: 2,
    Piecewise: 3,
    Invalid: 4,
  }

  #impl;
  constructor() {
    if (!WAtlas.#module) {
      throw new Error('WAtlas not initialized! Call await WAtlas.Initialize() before constructing an WAtlas instance.')
    }
    this.#impl = new WAtlas.#module.WAtlasImpl();
  }

  delete() {
    this.#impl.delete();
    this.#impl = null;
  }

  addMesh(meshDecl) {
    const meshDeclImpl = {...meshDecl};
    if (meshDecl.indexData) {
      if (meshDecl.indexData instanceof Uint16Array) {
        meshDeclImpl.indexFormat = 0; ; // Uint16
      } else if (meshDecl.indexData instanceof Uint32Array) {
        meshDeclImpl.indexFormat = 1; // Uint32
      } else {
        throw new Error('Unsupported indexData format. Must be Uint16Array or Uint32Array');
      }
    }
    return this.#impl.addMesh(meshDecl);
  }

  addUvMesh(meshDecl) {
    const meshDeclImpl = {...meshDecl};
    if (meshDecl.indexData) {
      if (meshDecl.indexData instanceof Uint16Array) {
        meshDeclImpl.indexFormat = 0; ; // Uint16
      } else if (meshDecl.indexData instanceof Uint32Array) {
        meshDeclImpl.indexFormat = 1; // Uint32
      } else {
        throw new Error('Unsupported indexData format. Must be Uint16Array or Uint32Array');
      }
    }
    return this.#impl.addUvMesh(meshDecl);
  }

  computeCharts(options) {
    this.#impl.computeCharts(options);
  }

  packCharts(options) {
    this.#impl.packCharts(options);
  }

  generate(chartOptions, packOptions) {
    this.#impl.generate(chartOptions, packOptions);
  }

  getResult() {
    const resultImpl = this.#impl.getResult();

    const result = {
      meshes: [],
      utilization: new Float32Array(resultImpl.atlasCount),
      width: resultImpl.width,
      height: resultImpl.height,
      atlasCount: resultImpl.atlasCount,
      chartCount: resultImpl.chartCount,
      texelsPerUnit: resultImpl.texelsPerUnit,
    };

    for (let i = 0; i < resultImpl.meshCount; ++i) {
      const meshImpl = resultImpl.meshes.get(0);

      const mesh = {
        chartArray: [],
        indexArray: new Uint32Array(meshImpl.indexCount),
        vertexArray: [],
      };

      for (let j = 0; j < meshImpl.chartCount; ++j) {
        const chartImpl = meshImpl.chartArray.get(j);

        const chart = {
          faceArray: new Uint32Array(chartImpl.faceCount),
          atlasIndex: chartImpl.atlasIndex,
          type: chartImpl.type, // TODO: Translate Enum
          material: chartImpl.material,
        };

        chart.faceArray.set(chartImpl.faceArray);

        mesh.chartArray.push(chart);
      }

      mesh.indexArray.set(meshImpl.indexArray);

      for (let j = 0; j < meshImpl.vertexCount; ++j) {
        const vertexImpl = meshImpl.vertexArray.get(j);
        mesh.vertexArray.push(vertexImpl);
      }

      result.meshes.push(mesh);
    }

    result.utilization.set(resultImpl.utilization);

    return result;
  }
}