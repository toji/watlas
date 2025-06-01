import { expect, describe, it, beforeEach } from 'vitest';
import { WAtlas } from "../../dist/watlas"

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
    beforeEach(async () => {
        await WAtlas.Initialize();
    });

    it("will have the expected state after construction", () => {
        const atlas = new WAtlas();

        expect(atlas.width).toBe(0);
        expect(atlas.height).toBe(0);
        expect(atlas.atlasCount).toBe(0);
        expect(atlas.chartCount).toBe(0);
        expect(atlas.meshCount).toBe(0);
        expect(atlas.texelsPerUnit).toBe(0.0);

        atlas.delete();
    });
  });
});