#include "scenes/PlayLevel.hpp"
#include "GameEngine.hpp"

namespace Scenes {
  ///// Public

  PlayLevel::PlayLevel(GameEngine *engine, std::string &lvlConfigFile) : Base(engine), m_levelPath(lvlConfigFile) {
    m_logOrigin = "Scenes::PlayLevel (" + m_levelPath + ")";
    init();
  }

  void PlayLevel::update() {
    // TODO
  }

  void PlayLevel::doAction(const Action &action) { sDoAction(action); }

  
  ///// Private

  void PlayLevel::init() {
    // TODO
    logInfo("Loading scene");
    m_gridTextFont = m_engine->assetStore().getFont("SimpleFont");
  }

  void PlayLevel::onEnd() {
    // TODO
    m_finished = true;
  }

  void PlayLevel::sDoAction(const Action &action) {
    // TODO
  }

  void PlayLevel::sRender() {
    // TODO
  }
  
}
