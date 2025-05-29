export async function InitializeWAtlas() {
  const WAtlasModule = await Module();
  return WAtlasModule.WAtlas;
}