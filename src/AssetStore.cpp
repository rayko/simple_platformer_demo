#include "AssetStore.hpp"
#include <fstream>

AssetStore::AssetStore() { m_logger.debugMode = m_debugMode; }

void AssetStore::setDebugMode(bool value) {
  if (m_debugMode && value) { return; }
  m_debugMode = value;
  m_logger.debugMode = value;
  if (m_debugMode)
    m_logger.info("ENABLING DEBUG MDOE");
}

void AssetStore::loadConfigs(const std::string &configFile) {
  m_logger.info("Reading " + configFile);

  std::ifstream fin(configFile);
  if (!fin.is_open()) {
    m_logger.error("Could not open " + configFile);
    exit(1);
  }

  std::string token;
  m_logger.debug("Loading assets... ");
  while (fin >> token) {
    if (token == "Texture") {
      TextureData tex = readTextureCfg(fin);
      loadTexture(tex);
      m_stats.textures++;
      m_logger.debug("Loaded texture " + tex.name + " " + tex.path);
    } else if (token == "Animation" ) {
      AnimationData anim = readAnimationCfg(fin);
      m_stats.animations++;
      m_logger.debug("Loaded animation" + anim.name);
    } else if (token == "Font") {
      FontData font = readFontCfg(fin);
      m_stats.fonts++;
      m_logger.debug("Loaded font" + font.name + " " + font.path);
    }
  }

  m_logger.debug("Loaded " + std::to_string(m_stats.textures) + " textures");
  m_logger.debug("Loaded " + std::to_string(m_stats.fonts) + " fonts");
  m_logger.debug("Loaded " + std::to_string(m_stats.animations) + " animations");
  m_logger.debug("Finished loading!");

  init();
}

void AssetStore::init() {
  m_logger.info("Initializing...");
  // ??
  m_logger.info("Done!");
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
  if (m_textures[textureData.name]) {
    m_logger.error("Already loaded texture with name: " + textureData.name);
    exit(1);
  }
  auto tex = std::make_shared<sf::Texture>();

  // Fail if we could not open the texture file
  if (!tex->loadFromFile(textureData.path)) {
    m_logger.error("Could not load texture path " + textureData.path);
    exit(1);
  }
  m_textures[textureData.name] = tex;
}

void AssetStore::loadFont(const FontData &fontData) {
  // Fail if we already added a texture with the same name
  if (m_fonts[fontData.name]) {
    m_logger.error("Already loaded font with name: " + fontData.name);
    exit(1);
  }
  auto font = std::make_shared<sf::Font>();

  // Fail if we could not open the texture file
  if (!font->openFromFile(fontData.path)) {
    m_logger.error("Could not load font path " + fontData.path);
    exit(1);
  }
  m_fonts[fontData.name] = font;
}

void AssetStore::loadAnimation(const AnimationData &animationData) {
  // ??
  // TODO
  // Check if texture exists first
  // Check if not duplicate
  // Create animation
}
