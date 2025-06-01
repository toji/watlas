import { expect, describe, it, beforeEach, afterEach } from 'vitest';
import { WAtlas, WMesh } from '../../dist/watlas';
import { CubeMeshIndexed, CubeMesh } from './cube-mesh';

function validateMesh(atlas: WAtlas, mesh: WMesh) {
    // Verify that we got an array of indices and that they are all valid
    const indices = new Uint32Array(mesh.indexCount);
    mesh.getIndexArray(indices);
    for (const index of indices) {
        expect(index).toBeLessThan(mesh.vertexCount);
    }

    // Verify that the returned vertices are valid
    for (let i = 0; i < mesh.vertexCount; ++i) {
        const vertex = mesh.getVertex(i);
        expect(vertex).toBeDefined();
        expect(vertex.atlasIndex).toBeLessThanOrEqual(atlas.atlasCount);
        expect(vertex.chartIndex).toBeLessThanOrEqual(atlas.chartCount);
        expect(vertex.uv[0]).toBeLessThanOrEqual(atlas.width);
        expect(vertex.uv[1]).toBeLessThanOrEqual(atlas.height);
        expect(vertex.xref).toBeLessThanOrEqual(mesh.vertexCount);
    }

    // Verify that the returned vertices are valid
    for (let i = 0; i < mesh.chartCount; ++i) {
        const chart = mesh.getChart(i);
        expect(chart).toBeDefined();
        expect(chart.atlasIndex).toBeLessThanOrEqual(atlas.atlasCount);
        expect(chart.faceCount).toBeGreaterThan(0);
        expect(chart.type).toBeLessThan(WAtlas.ChartType.Invalid);

        // Not sure what to validate here. Just make sure that it doesn't throw?
        const faces = new Uint32Array(chart.faceCount);
        chart.getFaceArray(faces);
    }
}

describe("WAtlas", function() {
  describe("initialization", () => {
    it("should fail to create a new WAtlas instance if the library has not been initialized", () => {
      expect(() => new WAtlas()).toThrowError('not initialized');
    });

    it("should succeed in creating a new WAtlas instance after the library has been initialzied", async () => {
      await WAtlas.Initialize();

      const atlas = new WAtlas();
      expect(atlas).toBeInstanceOf(WAtlas);
      atlas.delete();
    });
  });

  describe("usage", () => {
    let atlas: WAtlas;

    beforeEach(async () => {
        await WAtlas.Initialize();
        atlas = new WAtlas();
    });

    afterEach(() => {
        atlas.delete();
    });

    it("will have the expected state after construction", () => {
        expect(atlas.width).toBe(0);
        expect(atlas.height).toBe(0);
        expect(atlas.atlasCount).toBe(0);
        expect(atlas.chartCount).toBe(0);
        expect(atlas.meshCount).toBe(0);
        expect(atlas.texelsPerUnit).toBe(0.0);
    });

    it("can generate with non-indexed meshes", () => {
        atlas.addMesh(CubeMesh);
        atlas.generate();

        expect(atlas.width).toBeGreaterThan(0);
        expect(atlas.height).toBeGreaterThan(0);
        expect(atlas.atlasCount).toBe(1);
        expect(atlas.chartCount).toBeGreaterThan(0);
        expect(atlas.meshCount).toBe(1);
        expect(atlas.texelsPerUnit).toBeGreaterThan(0.0);

        const mesh = atlas.getMesh(0);
        expect(mesh.chartCount).toBeGreaterThan(0);
        expect(mesh.indexCount).toBe(CubeMesh.vertexCount);
        expect(mesh.vertexCount).toBe(CubeMesh.vertexCount);

        validateMesh(atlas, mesh);
    });

    it("can generate with indexed meshes", () => {
        atlas.addMesh(CubeMeshIndexed);
        atlas.generate();

        expect(atlas.width).toBeGreaterThan(0);
        expect(atlas.height).toBeGreaterThan(0);
        expect(atlas.atlasCount).toBe(1);
        expect(atlas.chartCount).toBeGreaterThan(0);
        expect(atlas.meshCount).toBe(1);
        expect(atlas.texelsPerUnit).toBeGreaterThan(0.0);

        const mesh = atlas.getMesh(0);
        expect(mesh.chartCount).toBeGreaterThan(0);
        expect(mesh.indexCount).toBe(CubeMeshIndexed.indexCount);
        expect(mesh.vertexCount).toBeGreaterThanOrEqual(CubeMeshIndexed.vertexCount);

        validateMesh(atlas, mesh);
    });
  });
});