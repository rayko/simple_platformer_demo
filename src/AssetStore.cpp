#include "AssetStore.hpp"
#include <fstream>

AssetStore::AssetStore() { m_logOrigin = "AssetStore"; }
AssetStore::AssetStore(std::shared_ptr<Logger> &logger) {
  m_logOrigin = "AssetStore";
  m_logger = logger;
}


void AssetStore::loadConfigs(const std::string &configFile) {
  logInfo("Reading " + configFile);

  std::ifstream fin = openFile(configFile);
  std::string token;
  logDebug("Loading assets... ");
  while (fin >> token) {
    if (token == "Texture") {
      TextureData tex = readTextureCfg(fin);
      loadTexture(tex);
      m_stats.textures++;
      logDebug("Loaded texture " + tex.name + " " + tex.path);
    } else if (token == "Animation" ) {
      AnimationData anim = readAnimationCfg(fin);
      m_stats.animations++;
      logDebug("Loaded animation" + anim.name);
    } else if (token == "Font") {
      FontData font = readFontCfg(fin);
      m_stats.fonts++;
      logDebug("Loaded font" + font.name + " " + font.path);
    }
  }

  logDebug("Loaded " + std::to_string(m_stats.textures) + " textures");
  logDebug("Loaded " + std::to_string(m_stats.fonts) + " fonts");
  logDebug("Loaded " + std::to_string(m_stats.animations) + " animations");
  logDebug("Finished loading!");

  init();
}

void AssetStore::init() {
  logInfo("Initializing...");
  // ??
  logInfo("Done!");
}

// Helpers
const AnimationData AssetStore::readAnimationCfg(std::ifstream &configData) {
  AnimationData item;
  configData >> item.name >> item.textureName >> item.frames >>
      item.frameDuration;
  return item;
}

const TextureData AssetStore::readTextureCfg(std::ifstream &configData) {
  TextureData item;
  configData >> item.name >> item.path;
  return item;
}

const FontData AssetStore::readFontCfg(std::ifstream &configData) {
  FontData item;
  configData >> item.name >> item.path;
  return item;
}

void AssetStore::loadTexture(const TextureData &textureData) {
  // Fail if we already added a texture with the same name
  if (m_textures[textureData.name])
    exitWithError("Already loaded texture with name: " + textureData.name);

  auto tex = std::make_shared<sf::Texture>();

  // Fail if we could not open the texture file
  if (!tex->loadFromFile(textureData.path))
    exitWithError("Could not load texture path " + textureData.path);

  m_textures[textureData.name] = tex;
}

void AssetStore::loadFont(const FontData &fontData) {
  // Fail if we already added a texture with the same name
  if (m_fonts[fontData.name])
    exitWithError("Already loaded font with name: " + fontData.name);

  auto font = std::make_shared<sf::Font>();

  // Fail if we could not open the texture file
  if (!font->openFromFile(fontData.path))
    exitWithError("Could not load font path " + fontData.path);

  m_fonts[fontData.name] = font;
}

void AssetStore::loadAnimation(const AnimationData &animationData) {
  // ??
  // TODO
  // Check if texture exists first
  // Check if not duplicate
  // Create animation
}
