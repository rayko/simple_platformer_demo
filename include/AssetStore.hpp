/*
  AssetStore.hpp
  Main handler of asset files, loading and defining the various
  resources needed. For now it handles texture images and fonts.
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "Logger.hpp"

struct AnimationData {
  std::string name;
  std::string textureName;
  int frames = 0;
  int frameDuration = 0;
};

struct FontData {
  std::string name;
  std::string path;
};

struct TextureData {
  std::string name;
  std::string path;
};

struct AssetStats {
  int textures = 0;
  int fonts = 0;
  int animations = 0;
};

class AssetStore {
  Logger m_logger = Logger("AssetStore");
  bool m_debugMode = true;
  AssetStats m_stats; // Just informational
  std::map<std::string, std::shared_ptr<sf::Texture>> m_textures;
  std::map<std::string, std::shared_ptr<sf::Font>> m_fonts;
  std::map<std::string, AnimationData> m_animations;

  // File reading helpers to extract the info
  const AnimationData readAnimationCfg(std::ifstream &configData);
  const TextureData readTextureCfg(std::ifstream &configData);
  const FontData readFontCfg(std::ifstream &configData);

  // Loading helpers
  void loadTexture(const TextureData &textureData);
  void loadFont(const FontData &fontData);
  void loadAnimation(const AnimationData &animationData);

public:
  AssetStore();
  void setDebugMode(bool value);
  void loadConfigs(const std::string &configFile);
  void init();

  sf::Texture getTexture(const std::string &name) const;
  sf::Font getFont(const std::string &name) const;
  void getAnimation(const std::string &name) const;
};
