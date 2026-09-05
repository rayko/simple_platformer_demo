#include "AssetStore.hpp"
#include "Vec2f.hpp"
#include <fstream>
#include <string>

AssetStore::AssetStore() { m_logOrigin = "AssetStore"; }
AssetStore::AssetStore(std::shared_ptr<Logger> &logger) {
  m_logOrigin = "AssetStore";
  m_logger = logger;
}

std::shared_ptr<sf::Font> AssetStore::getFont(const std::string &name) const {
  int value = m_fonts.size();
  if (!m_fonts.contains(name))
    exitWithError("Queried missing font " + name);

  return m_fonts.at(name);
}

std::shared_ptr<sf::Texture> AssetStore::getTexture(const std::string &name) const {
  if (!m_textures.contains(name))
    exitWithError("Queried missing texture " + name);

  return m_textures.at(name);
}

std::shared_ptr<Animation> AssetStore::getAnimation(const std::string &name) const {
  if (!m_animations.contains(name))
    exitWithError("Queried missing animation " + name);

  return m_animations.at(name);
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
      loadAnimation(anim);
      m_stats.animations++;
      logDebug("Loaded animation " + anim.name);
    } else if (token == "Font") {
      FontData font = readFontCfg(fin);
      loadFont(font);
      m_stats.fonts++;
      logDebug("Loaded font " + font.name + " " + font.path);
    } else {
      // logWarn("Unrecognized keyword: " + token);
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
  configData >> item.name >> item.textureName;
  configData >> item.frames >> item.frameDuration;
  configData >> item.width >> item.height;

  int modeValue;
  // Read value for PlayMode as int
  configData >> modeValue;
  switch (modeValue) {
  case (0):
    item.playMode = Animation::PlayMode::Static;
    break;
  case (1):
    item.playMode = Animation::PlayMode::Once;
    break;
  case (2):
    item.playMode = Animation::PlayMode::Loop;
    break;
  default:
    exitWithError("Animnation " + item.name + ": Invalid PlayMode value " + std::to_string(modeValue));
    break;
  }

  // Read value for LoopMode as int
  configData >> modeValue;
  switch (modeValue) {
  case (0):
    item.loopMode = Animation::LoopMode::Forward;
    break;
  case (1):
    item.loopMode = Animation::LoopMode::Backward;
    break;
  case (2):
    item.loopMode = Animation::LoopMode::Bounce;
    break;
  default:
    exitWithError("Animnation " + item.name + ": Invalid LoopMode value " + std::to_string(modeValue));
    break;
  }

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

void AssetStore::loadTexture(const TextureData &data) {
  // Fail if we already added a texture with the same name
  if (m_textures.contains(data.name))
    exitWithError("Already loaded texture with name: " + data.name);

  m_textures[data.name] = std::make_shared<sf::Texture>();
  // Fail if we could not open the texture file
  if (!m_textures[data.name]->loadFromFile(data.path))
    exitWithError("Could not load texture path " + data.path);
}

void AssetStore::loadFont(const FontData &data) {
  // Fail if we already added a texture with the same name
  if (m_fonts.contains(data.name))
    exitWithError("Already loaded font with name: " + data.name);

  m_fonts[data.name] = std::make_shared<sf::Font>();
  // Fail if we could not open the texture file
  if (!m_fonts[data.name]->openFromFile(data.path))
    exitWithError("Could not load font path " + data.path);
}

void AssetStore::loadAnimation(const AnimationData &data) {
  if (m_animations.contains(data.name))
    exitWithError("Already loaded animation with name: " + data.name);

  if (!m_textures.contains(data.textureName))
    exitWithError("Animation " + data.name + " points to missing texture " + data.textureName);

  std::shared_ptr<sf::Texture> tex = m_textures[data.textureName];
  const Vec2f size(data.width, data.height);
  std::string name = data.name;
  m_animations[name] = std::make_shared<Animation>(name, *tex, size);
  if (data.frames > 1) {
    m_animations[name]->setFrameCount(data.frames);
    m_animations[name]->setFrameDuration(data.frameDuration);
    m_animations[name]->setPlayMode(data.playMode);
    m_animations[name]->setLoopMode(data.loopMode);
  }
  // logDebug(m_animations[name]->str());
}
