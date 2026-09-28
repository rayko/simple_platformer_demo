#include "scenes/PlayLevel.hpp"
#include "GameEngine.hpp"
#include "scenes/Menu.hpp"
#include <fstream>
#include <cmath>

namespace Scenes {
  ///// Public

  PlayLevel::PlayLevel(GameEngine *engine, const std::string &lvlConfigFile)
    : m_levelPath(lvlConfigFile), Base(engine) {

    m_logOrigin = "Scenes::PlayLevel (" + m_levelPath + ")";
    setLogger(engine->getLogger());
    m_physics = Physics();
    init();
  }

  void PlayLevel::update() {
    m_entityManager.update();

    if (!m_paused) {
      sMovement();
      sCollisions();
      sLifespan();
      sEventTimers();
      sInteractions();
      sAnimation();
    }

    sRender();
  }

  void PlayLevel::doAction(const Action &action) { sDoAction(action); }


  ///// Private

  void PlayLevel::init() {
    logInfo("Loading scene");
    m_gridTextFont = m_engine->assetStore().getFont("SimpleFont");

    m_debugPanel = std::make_shared<UI::TextPanel>(m_gridTextFont);

    logDebug("Mapping actions");
    registerKeyboardAction(sf::Keyboard::Scancode::G, Action::Name::ToggleGrid);
    registerKeyboardAction(sf::Keyboard::Scancode::T, Action::Name::ToggleTextures);
    registerKeyboardAction(sf::Keyboard::Scancode::C, Action::Name::ToggleColliders);
    registerKeyboardAction(sf::Keyboard::Scancode::Escape, Action::Name::Escape);
    registerKeyboardAction(sf::Keyboard::Scancode::P, Action::Name::Pause);
    registerKeyboardAction(sf::Keyboard::Scancode::Z, Action::Name::ToggleInfo);

    registerKeyboardAction(sf::Keyboard::Scancode::W, Action::Name::Jump);
    registerKeyboardAction(sf::Keyboard::Scancode::S, Action::Name::Down);
    registerKeyboardAction(sf::Keyboard::Scancode::A, Action::Name::Left);
    registerKeyboardAction(sf::Keyboard::Scancode::D, Action::Name::Right);
    registerKeyboardAction(sf::Keyboard::Scancode::Up, Action::Name::Jump);
    registerKeyboardAction(sf::Keyboard::Scancode::Down, Action::Name::Down);
    registerKeyboardAction(sf::Keyboard::Scancode::Left, Action::Name::Left);
    registerKeyboardAction(sf::Keyboard::Scancode::Right, Action::Name::Right);
    registerKeyboardAction(sf::Keyboard::Scancode::Space, Action::Name::Shoot);
    registerKeyboardAction(sf::Keyboard::Scancode::E, Action::Name::Interact);

    m_width = m_engine->window().getSize().x;
    m_height = m_engine->window().getSize().y;
    m_view = sf::View(sf::FloatRect({0, 0}, {(float)m_width, (float)m_height}));
    m_engine->window().setView(m_view);

    // Set the reference to world origin point. Since we are going to be
    // using a custom grid where the bottom-left corner is (0,0), we set
    // our world origin to x=0, and y=window.y which is the total y size.
    m_worldOrigin = {0, (float)m_height};

    m_entityManager = EntityManager();
    loadLevel(m_levelPath);
    // countRequiredCoins();
    spawnPlayer(Vec2f(m_playerAttrs.x, m_playerAttrs.y));
  }

  void PlayLevel::loadLevel(const std::string &filename) {
    logInfo("Loading config " + filename);
    std::ifstream fin = openFile(filename);
    logDebug("Loading level...");
    std::string token;
    Vec2f gridPos;
    Vec2f worldPos;
    std::shared_ptr<Animation> anim;
    std::shared_ptr<Entity> entity;
    std::string animName;
    int value;
    while (fin >> token) {
      // Ignore comments
      if (token.starts_with("#")) {
        fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        continue;
      }

      if (token == "Player"){
        // Load player attrs
        fin >> m_playerAttrs.cx >> m_playerAttrs.cy; // Collider size (x,y)
        fin >> m_playerAttrs.speed;                  // X speed (run/move)
        fin >> m_playerAttrs.jumpVel;                // Jump speed (Y)
        fin >> m_playerAttrs.maxSpeed;               // max speed (x or y)
        fin >> m_playerAttrs.gravity;                // Duh!
        fin >> m_playerAttrs.weaponName;             // Texture for bullet
      } else if (token == "Tile") {
        // Create all tiles
        fin >> animName;
        fin >> gridPos.x >> gridPos.y;

        entity = m_entityManager.addEntity("Tile");
        entity->addComponent<CAnimation>(m_engine->assetStore().getAnimation(animName));
        anim = entity->getComponent<CAnimation>().animation;
        worldPos = initialSpritePosition(gridPos, anim->getSize());
        entity->addComponent<CTransform>(worldPos);
        entity->addComponent<CBoxCollider>(Vec2f(64,64));
        if (anim->getName() == "WoodBox1")
          entity->addComponent<CGravity>(m_playerAttrs.gravity);

        if (anim->getName() == "Coin") {
          entity->addComponent<CCoinReward>();
          m_levelRequiredCoins++;
        }
        
        if (anim->getName() == "ActiveTile1") {
          entity->addComponent<CCoinReward>();
          m_levelRequiredCoins++;
        }

      } else if (token == "FrontDec") {
        // Create all decorations
        fin >> animName;
        fin >> gridPos.x >> gridPos.y;

        entity = m_entityManager.addEntity("FrontDec");
        entity->addComponent<CAnimation>(m_engine->assetStore().getAnimation(animName));
        anim = entity->getComponent<CAnimation>().animation;
        worldPos = initialSpritePosition(gridPos, anim->getSize());
        entity->addComponent<CTransform>(worldPos);
      } else if (token == "BackDec") {
        // Create all decorations
        fin >> animName;
        fin >> gridPos.x >> gridPos.y;

        entity = m_entityManager.addEntity("BackDec");
        entity->addComponent<CAnimation>(m_engine->assetStore().getAnimation(animName));
        anim = entity->getComponent<CAnimation>().animation;
        worldPos = initialSpritePosition(gridPos, anim->getSize());
        entity->addComponent<CTransform>(worldPos);
      } else if (token == "BackgroundColor") {
        fin >> value;
        m_bgColor.r = value;
        fin >> value;
        m_bgColor.g = value;
        fin >> value;
        m_bgColor.b = value;
      } else if (token == "PlayerSpawn") {
        // Setup player spawn location
        fin >> m_playerAttrs.x >> m_playerAttrs.y;
      } else if (token == "ExitSwitch") {
        // Setup exit switch location
        // TODO Hardcoded
        fin >> gridPos.x >> gridPos.y;
        entity = m_entityManager.addEntity("ExitSwitch");
        entity->addComponent<CAnimation>(m_engine->assetStore().getAnimation("ExitMachineIdle"));
        anim = entity->getComponent<CAnimation>().animation;
        worldPos = initialSpritePosition(gridPos, anim->getSize());
        entity->addComponent<CTransform>(worldPos);
        entity->addComponent<CBoxCollider>(Vec2f(64,64));
        entity->addComponent<CInteractible>("OpenExit");
        m_levelExitSwitch = entity;
      } else if (token == "ExitDoor") {
        // Setup exit door location
        fin >> gridPos.x >> gridPos.y;
        entity = m_entityManager.addEntity("Tile");
        entity->addComponent<CAnimation>(m_engine->assetStore().getAnimation("ExitDoorIdle"));
        anim = entity->getComponent<CAnimation>().animation;
        worldPos = initialSpritePosition(gridPos, anim->getSize());
        entity->addComponent<CTransform>(worldPos);
        entity->addComponent<CBoxCollider>(Vec2f(64,128));
        m_levelExitDoor = entity;
      } else {
        logWarn("Unrecognized keyword: " + token);
      }
    }
  }

  void PlayLevel::onEnd() {
    // TODO
    m_finished = true;
  }


  //// Gameplay Mechanics and helpers ////

  void PlayLevel::playerHitsTile(std::shared_ptr<Entity> &tile) {
    auto anim = tile->getComponent<CAnimation>().animation;
    if (anim->getName() == "ActiveTile1") {
      tile->addComponent<CAnimation>(m_engine->assetStore().getAnimation("InnactiveTile1"));
      spawnCoin(tile);
    } else if (anim->getName() == "WoodBox1") {
      destroyTile(tile);
    }
  }

  void PlayLevel::expireBullet(std::shared_ptr<Entity> &bullet) {
    std::shared_ptr<Entity> fx = m_entityManager.addEntity("FrontDec");
    fx->addComponent<CTransform>(bullet->getComponent<CTransform>().pos);
    fx->addComponent<CAnimation>(m_engine->assetStore().getAnimation("BulletExpl1"));
    bullet->destroy();
  }

  void PlayLevel::spawnPlayer(const Vec2f &gridBlock) {
    m_player = m_entityManager.addEntity("Player");
    m_player->addComponent<CAnimation>(m_engine->assetStore().getAnimation("AlexAir"));
    auto anim = m_player->getComponent<CAnimation>().animation;
    m_player->addComponent<CTransform>(initialSpritePosition(gridBlock, anim->getSize()));
    m_player->addComponent<CBoxCollider>(Vec2f(m_playerAttrs.cx, m_playerAttrs.cy));
    m_player->addComponent<CInput>();
    m_player->addComponent<CState>(m_playerStates.airborne);
    m_player->addComponent<CGravity>(m_playerAttrs.gravity);
  }

  void PlayLevel::spawnBullet(std::shared_ptr<Entity> &player) {
    const auto &pTransform = player->getComponent<CTransform>();
    int direction = 1;
    const float speed = m_playerAttrs.maxSpeed / 2;
    if (pTransform.scale.x > 0) { direction = 1; }
    else { direction = -1; }

    std::shared_ptr<Entity> bullet = m_entityManager.addEntity("Bullet");
    bullet->addComponent<CTransform>(pTransform.pos);
    bullet->getComponent<CTransform>().pos.x += 10 * direction;
    bullet->getComponent<CTransform>().scale.x = direction;
    bullet->getComponent<CTransform>().vel = Vec2f(speed * direction, 0);
    bullet->addComponent<CAnimation>(m_engine->assetStore().getAnimation("Bullet1"));
    bullet->addComponent<CBoxCollider>(Vec2f(36,10));
    bullet->addComponent<CLifespan>(50);
  }

  void PlayLevel::spawnCoin(std::shared_ptr<Entity> &originTile) {
    const auto tTransform = originTile->getComponent<CTransform>();
    Vec2f coinLocation = Vec2f(tTransform.pos.x, tTransform.pos.y - m_gridSize.y);
    auto fx = m_entityManager.addEntity("FrontDec");
    fx->addComponent<CTransform>(coinLocation);
    fx->addComponent<CAnimation>(m_engine->assetStore().getAnimation("Smoke"));
    Vec2f gridPos = gridBlockFromPixel(coinLocation);
    spawnCoinAt(gridPos);
  }

  void PlayLevel::spawnCoinAt(Vec2f &gridPos) {
    auto coin = m_entityManager.addEntity("Tile");
    auto anim = m_engine->assetStore().getAnimation("Coin");
    coin->addComponent<CAnimation>(anim);
    coin->addComponent<CTransform>(initialSpritePosition(gridPos, anim->getSize()));
    coin->addComponent<CBoxCollider>(Vec2f(32, 32));
    coin->addComponent<CCoinReward>();
  }

  void PlayLevel::destroyBullet(std::shared_ptr<Entity> &bullet, int direction) {
    std::shared_ptr<Entity> fx = m_entityManager.addEntity("FrontDec");
    fx->addComponent<CTransform>(bullet->getComponent<CTransform>().pos);
    fx->addComponent<CAnimation>(m_engine->assetStore().getAnimation("BulletHit1"));
    fx->getComponent<CTransform>().scale.x = direction;
    bullet->destroy();
  }

  void PlayLevel::destroyTile(std::shared_ptr<Entity> &tile) {
    std::shared_ptr<Entity> fx = m_entityManager.addEntity("FrontDec");
    fx->addComponent<CTransform>(tile->getComponent<CTransform>().pos);
    fx->addComponent<CAnimation>(m_engine->assetStore().getAnimation("Explosion"));
    tile->destroy();
  }

  void PlayLevel::checkBulletCollisions(){
    Vec2f overlap = {-1, -1};
    Vec2f prevOverlap = {-1, -1};
    for (auto bullet : m_entityManager.entities("Bullet")){
      for (auto tile : m_entityManager.entities("Tile")) {
        if (!tile->hasComponent<CBoxCollider>()) { continue; }
        overlap = m_physics.getOverlap(bullet, tile);
        if (overlap.x <= 0 || overlap.y <= 0) { continue; }
        prevOverlap = m_physics.getPreviousOverlap(bullet, tile);
        if (overlap.x > 0 && prevOverlap.y > 0) {
          // Horizontal
          if (bullet->getComponent<CTransform>().vel.x > 0) {
            // From left
            destroyBullet(bullet, 1);
          } else if (bullet->getComponent<CTransform>().vel.x < 0) {
            // From right
            destroyBullet(bullet, -1);
          }
          if (tile->getComponent<CAnimation>().animation->getName() == "WoodBox1")
            destroyTile(tile);
        }
      }
    }
  }

  void PlayLevel::pickupCoin() {
    logDebug("Picked up a coin");
    m_playerCoins++;
  }

  void PlayLevel::handleTimedEvent(const std::string &name) {
    std::shared_ptr<Entity> entity;
    logDebug("Triggering timed event: " + name);

    logWarn("Unhandled timed event name: " + name);
  }

  void PlayLevel::handleInteraction(std::shared_ptr<Entity> &entity) {
    const std::string triggerName = entity->getComponent<CInteractible>().triggerName;
    logDebug("Interacting with entity with trigger " + triggerName);

    if (triggerName == "OpenExit") {
      if (m_playerCoins >= m_levelRequiredCoins) {
        m_levelExitSwitch->addComponent<CAnimation>(m_engine->assetStore().getAnimation("ExitMachineOpen"));
        m_levelExitSwitch->getComponent<CAnimation>().destroyWhenFinished = false;
        m_levelExitDoor->addComponent<CAnimation>(m_engine->assetStore().getAnimation("ExitDoorOpen"));
        m_levelExitDoor->getComponent<CAnimation>().destroyWhenFinished = false;
        m_levelExitDoor->removeComponent<CBoxCollider>();
        // Events to finish

      } else {
        // TODO
        logDebug("Not enough coins");
      }
      return;
    }

    logWarn("Unhandled interaction trigger: " + triggerName);
  }
  
  ///// System Functions /////

  void PlayLevel::sInteractions() {
    Vec2f overlap;
    for (auto entity : m_entityManager.entities()) {
      if (!entity->hasComponent<CInteractible>()) { continue; }
      if (!entity->hasComponent<CBoxCollider>()) { continue; }
      overlap = m_physics.getOverlap(m_player, entity);
      if (overlap.x <= 0 || overlap.y <= 0) { continue; }
      // If we reach here, we are nerby an interactible entity (colliding)
      // IDEA Show something on screen to inform it's possible to interact with entity
      if (m_player->getComponent<CInput>().canInteract && m_player->getComponent<CInput>().interact) {
        m_player->getComponent<CInput>().canInteract = false;
        handleInteraction(entity);
      }
    }
  }

  void PlayLevel::sLifespan() {
    for (auto entity : m_entityManager.entities()) {
      if (entity->hasComponent<CLifespan>()) {
        entity->getComponent<CLifespan>().remaining--;

        if (entity->getComponent<CLifespan>().remaining <= 0) {
          if (entity->tag() == "Bullet")
            expireBullet(entity);
          else
            entity->destroy();
        }
      }
    }
  }

  void PlayLevel::sEventTimers() {
    for (auto entity : m_entityManager.entities()) {
      if (entity->hasComponent<CEventTimer>()) {
        entity->getComponent<CEventTimer>().remaining--;
        if (entity->getComponent<CEventTimer>().remaining <= 0){
          handleTimedEvent(entity->getComponent<CEventTimer>().name);
          entity->destroy();
        }
      }
    }
  }

  void PlayLevel::sCollisions() {
    Vec2f overlap = {-1, -1};
    Vec2f prevOverlap = {-1, -1};
    m_player->getComponent<CBoxCollider>().colliding = false;

    checkBulletCollisions();

    // gravity tiles
    for (auto tile : m_entityManager.entities("Tile")) {
      if (!tile->hasComponent<CBoxCollider>()) { continue; }
      if (!tile->hasComponent<CGravity>()) { continue; }
      if (tile->getComponent<CAnimation>().animation->getName() == "WoodBox1") { 
        for (auto otherTile : m_entityManager.entities("Tile")) {
          if (tile->id() == otherTile->id()) { continue; }
          if (!otherTile->hasComponent<CBoxCollider>()) { continue; }
          overlap = m_physics.getOverlap(tile, otherTile);
          if (overlap.x <= 0 || overlap.y <= 0) { continue; }
          prevOverlap = m_physics.getPreviousOverlap(tile, otherTile);
          if (overlap.y > 0 && prevOverlap.x > 0) {
            // Vertical
            if (tile->getComponent<CTransform>().vel.y > 0) {
              // from top
              tile->getComponent<CTransform>().vel.y = 0;
              tile->getComponent<CTransform>().pos.y -= overlap.y;
              tile->getComponent<CTransform>().prevPos.y -= overlap.y;
            }
          }
        }
      }
    }

    // Player vs tiles
    if (m_player->hasComponent<CBoxCollider>())
      for (auto tile : m_entityManager.entities("Tile")) {
        if (!tile->hasComponent<CBoxCollider>()) { continue; }
        tile->getComponent<CBoxCollider>().colliding = false;
        overlap = m_physics.getOverlap(m_player, tile);
        if (overlap.x <= 0 || overlap.y <= 0) { continue; }
        if (tile->getComponent<CAnimation>().animation->getName() == "Coin") {
          m_playerCoins += tile->getComponent<CCoinReward>().amount;
          tile->destroy();
          continue;
        }

        tile->getComponent<CBoxCollider>().colliding = true;
        m_player->getComponent<CBoxCollider>().colliding = true;
        prevOverlap = m_physics.getPreviousOverlap(m_player, tile);
        if (overlap.x > 0 && prevOverlap.y > 0) {
          // horizontal
          if (overlap.y <= 5) {
            m_player->getComponent<CTransform>().pos.y -= overlap.y; // Uppies
          } else {
            if (m_player->getComponent<CTransform>().vel.x > 0) {
              // from left
              m_player->getComponent<CTransform>().pos.x -= overlap.x;
              m_player->getComponent<CTransform>().prevPos.x -= overlap.x;
            } else if (m_player->getComponent<CTransform>().vel.x < 0){
              // from right
              m_player->getComponent<CTransform>().pos.x += overlap.x;
              m_player->getComponent<CTransform>().prevPos.x += overlap.x;
            }
          }
        }

        if (overlap.y > 0 && prevOverlap.x > 0) {
          // Vertical
          if (m_player->getComponent<CTransform>().vel.y > 0) {
            // from top
            m_player->getComponent<CTransform>().vel.y = 0;
            m_player->getComponent<CTransform>().pos.y -= overlap.y;
            m_player->getComponent<CTransform>().prevPos.y -= overlap.y;
            m_playerOnFloor = true;
          } else if (m_player->getComponent<CTransform>().vel.y < 0){
            // from bottom
            playerHitsTile(tile);
            m_player->getComponent<CTransform>().vel.y = 0;
            m_player->getComponent<CTransform>().pos.y += overlap.y;
            m_player->getComponent<CTransform>().prevPos.y += overlap.y;
          }
        }
      }
  }

  void PlayLevel::sDoAction(const Action &action) {
    if (action.starting()) {
      switch (action.name()) {
      case (Action::Name::Shoot):
        if (m_player) {
          if (m_player->getComponent<CInput>().canShoot) {
            m_player->getComponent<CInput>().shoot = true;
          }
        }
        break;
      case (Action::Name::Pause):
        m_paused = !m_paused;
        break;
      case (Action::Name::ToggleInfo):
        m_drawDebugPanel = !m_drawDebugPanel;
        break;
      case (Action::Name::ToggleGrid):
        m_drawGrid = !m_drawGrid;
        break;
      case (Action::Name::ToggleTextures):
        m_drawTextures = !m_drawTextures;
        break;
      case (Action::Name::ToggleColliders):
        m_drawColliders = !m_drawColliders;
        break;
      case (Action::Name::Escape):
        m_engine->changeScene("MainMenu", std::make_shared<Menu>(m_engine));
        break;
      case (Action::Name::Up):
        if (m_player)
          m_player->getComponent<CInput>().up = true;
        break;
      case (Action::Name::Down):
        if (m_player)
          m_player->getComponent<CInput>().down = true;
        break;
      case (Action::Name::Left):
        if (m_player)
          m_player->getComponent<CInput>().left = true;
        break;
      case (Action::Name::Right):
        if (m_player)
          m_player->getComponent<CInput>().right = true;
        break;
      case (Action::Name::Jump):
        if (m_player)
          m_player->getComponent<CInput>().jump = true;
        break;
      case (Action::Name::Interact):
        if (m_player)
          m_player->getComponent<CInput>().interact = true;
        break;
      default: break;
      }
    }

    if (action.ending()) {
      switch (action.name()) {
      case (Action::Name::Shoot):
        if (m_player) {
          m_player->getComponent<CInput>().shoot = false;
        }
        break;
      case (Action::Name::Up):
        if (m_player)
          m_player->getComponent<CInput>().up = false;
        break;
      case (Action::Name::Down):
        if (m_player)
          m_player->getComponent<CInput>().down = false;
        break;
      case (Action::Name::Left):
        if (m_player)
          m_player->getComponent<CInput>().left = false;
        break;
      case (Action::Name::Right):
        if (m_player)
          m_player->getComponent<CInput>().right = false;
        break;
      case (Action::Name::Jump):
        if (m_player)
          m_player->getComponent<CInput>().jump = false;
        break;
      case (Action::Name::Interact):
        if (m_player) {
          m_player->getComponent<CInput>().interact = false;
          m_player->getComponent<CInput>().canInteract = true;
        }
        break;
      default: break;
      }
    }
  }

  void PlayLevel::sAnimation() {
    // Set player animation based on state
    if (m_player){
      if (m_player->hasComponent<CState>()) {
        auto animName = m_player->getComponent<CAnimation>().animation->getName();
        if (m_player->getComponent<CState>().state == m_playerStates.run && animName != "AlexRun")
          m_player->addComponent<CAnimation>(m_engine->assetStore().getAnimation("AlexRun"));
        if (m_player->getComponent<CState>().state == m_playerStates.stand && animName != "AlexStand")
          m_player->addComponent<CAnimation>(m_engine->assetStore().getAnimation("AlexStand"));
        if (m_player->getComponent<CState>().state == m_playerStates.airborne && animName != "AlexAir")
          m_player->addComponent<CAnimation>(m_engine->assetStore().getAnimation("AlexAir"));
      }
    }

    for (auto entity : m_entityManager.entities()) {
      if (entity->hasComponent<CAnimation>()) {
        auto anim = entity->getComponent<CAnimation>().animation;
        if (anim->finished() && entity->getComponent<CAnimation>().destroyWhenFinished) {
          entity->destroy();
        } else {
          anim->getSprite().setPosition(entity->getComponent<CTransform>().pos.toVector2f());
          anim->getSprite().setScale(entity->getComponent<CTransform>().scale.toVector2f());
          anim->update();
        }
      }
    }
  }

  void PlayLevel::sMovement() {
    // Player logic
    if (m_player) {
      CTransform &pTransform = m_player->getComponent<CTransform>();
      CInput &pInput = m_player->getComponent<CInput>();
      CState &pState = m_player->getComponent<CState>();

      if (pTransform.pos.y > m_engine->window().getSize().y + 800) {
        // Teleport player to beginning cause it's the simplest route
        pTransform.pos = gridBlockOrigin(Vec2f(m_playerAttrs.x, m_playerAttrs.y));
      }

      // Left/Right movement
      if (pInput.left){
        pTransform.vel.x = -m_playerAttrs.speed;
        pTransform.scale.x = -1;
      } else if (pInput.right) {
        pTransform.vel.x = m_playerAttrs.speed;
        pTransform.scale.x = 1;
      } else {
        pTransform.vel.x = 0;
      }

      // Jumping
      if (!m_playerJumping && m_playerOnFloor) {
        if (!pInput.prevJump && pInput.jump) {
          // Activate jump
          m_playerJumping = true;
          m_playerOnFloor = false;
          pTransform.vel.y = -m_playerAttrs.jumpVel;
        }
      } else {
        if (pInput.prevJump && !pInput.jump){
          // Cancel jump (only if we are going up)
          m_playerJumping = false;
          if (pTransform.vel.y < 0)
            pTransform.vel.y = 0;
        }
      }

      // Shooting
      if (pInput.canShoot && pInput.shoot) {
        pInput.canShoot = false;
        spawnBullet(m_player);
      } else if (!pInput.canShoot && !pInput.shoot) {
        pInput.canShoot = true;
      }

      pInput.prevJump = pInput.jump;

      // Force floor unstick if we are moving substantially on Y
      // This way, gravity can trigger airborne state (ie: dropping from ledge)
      if (std::abs(pTransform.vel.y) > 3) {
        m_playerOnFloor = false;
      }

      // Rough set of states for animations based on velocity (not the best way)
      if (pTransform.vel.x == 0 && m_playerOnFloor){
        pState.state = m_playerStates.stand;
      } else if (m_playerOnFloor && pTransform.vel.x != 0) {
        pState.state = m_playerStates.run;
      } else if (!m_playerOnFloor){
        pState.state = m_playerStates.airborne;
      } else if (pTransform.vel.y > 0) {
        pState.state = m_playerStates.airborne;
      }
    }

    for (auto entity : m_entityManager.entities()) {
      if (entity->hasComponent<CTransform>()) {
        entity->getComponent<CTransform>().prevPos = entity->getComponent<CTransform>().pos;
        entity->getComponent<CTransform>().pos += entity->getComponent<CTransform>().vel;
        if (entity->hasComponent<CGravity>()){
          entity->getComponent<CTransform>().vel.y += m_playerAttrs.gravity;
        }
        entity->getComponent<CTransform>().vel.cap(m_playerAttrs.maxSpeed);
      }
    }
  }

  void PlayLevel::sRender() {
    sf::RenderWindow &window = m_engine->window();

    if (m_player) {
      auto &pTransform = m_player->getComponent<CTransform>();
      float viewX = std::max(m_width / 2.0f, pTransform.pos.x);
      float viewY = std::min(m_height / 2.0f, pTransform.pos.y);
      m_view.setCenter(sf::Vector2f(viewX, viewY));
    }

    m_engine->window().setView(m_view);
    m_viewCenter = m_view.getCenter();
    window.clear(m_bgColor);

    if (m_drawTextures) {
      // Background stuff
      for (auto entity : m_entityManager.entities("BackDec")) {
        if (entity->hasComponent<CAnimation>()) {
          window.draw(entity->getComponent<CAnimation>().animation->getSprite());
        }
      }

      // Active stuff (ground, blocks, etc)
      for (auto entity : m_entityManager.entities("Tile")) {
        if (entity->hasComponent<CAnimation>()) {
          window.draw(entity->getComponent<CAnimation>().animation->getSprite());
        }
      }

      for (auto entity : m_entityManager.entities("Bullet")) {
        if (entity->hasComponent<CAnimation>()) {
          window.draw(entity->getComponent<CAnimation>().animation->getSprite());
        }
      }

      for (auto entity : m_entityManager.entities("ExitSwitch")) {
        if (entity->hasComponent<CAnimation>()) {
          window.draw(entity->getComponent<CAnimation>().animation->getSprite());
        }
      }

      for (auto entity : m_entityManager.entities("ExitDoor")) {
        if (entity->hasComponent<CAnimation>()) {
          window.draw(entity->getComponent<CAnimation>().animation->getSprite());
        }
      }

      // Draw player here, before foreground
      window.draw(m_player->getComponent<CAnimation>().animation->getSprite());

      // Foreground stuff
      for (auto entity : m_entityManager.entities("FrontDec")) {
        if (entity->hasComponent<CAnimation>()) {
          window.draw(entity->getComponent<CAnimation>().animation->getSprite());
        }
      }
    }

    sf::Text coinCounter(*m_gridTextFont, "", 30);
    coinCounter.setString(std::format("{} / {}", m_playerCoins, m_levelRequiredCoins));
    sf::Vector2f counterPos = m_view.getCenter();
    counterPos.x -= (float)window.getSize().x / 2;
    counterPos.y -= (float)window.getSize().y / 2;
    counterPos.x += 10;
    counterPos.y += 10;
    coinCounter.setPosition(counterPos);
    window.draw(coinCounter);

    if (m_drawGrid)
      drawGrid();

    if (m_drawColliders)
      drawColliders();

    if (m_drawDebugPanel)
      drawDebugPanel();

    if (m_paused) {
      sf::Text pausedText(*m_gridTextFont, "PAUSED", 30);
      sf::Vector2f ctr = m_view.getCenter();
      ctr.y -= 100;
      ctr.x -= pausedText.getLocalBounds().size.x / 2;
      pausedText.setPosition(ctr);
      window.draw(pausedText);
    }

    window.display();
  }


  //// Helper functions ////

  void PlayLevel::drawDebugPanel() {
    m_debugPanel->clear();

    auto transform = m_player->getComponent<CTransform>();
    auto anim = m_player->getComponent<CAnimation>().animation;
    auto collider = m_player->getComponent<CBoxCollider>();

    m_debugPanel->addTextLine(std::format("Entities: {}", m_entityManager.entities().size()));
    m_debugPanel->addTextLine("Player");
    m_debugPanel->addTextLine("  Position: " + transform.pos.str());
    m_debugPanel->addTextLine("  Previous: " + transform.prevPos.str());
    m_debugPanel->addTextLine("  Velocity: " + transform.vel.str());
    m_debugPanel->addTextLine("  Center: " + Vec2f(anim->getSprite().getOrigin().x, anim->getSprite().getOrigin().y).str());
    m_debugPanel->addTextLine("  Collider Size: " + collider.size.str());
    m_debugPanel->addTextLine("  Collider Size / 2: " + collider.halfSize.str());
    m_debugPanel->addTextLine("  Collider Offset: " + collider.offset.str());
    sf::Vector2f pos = { 0, 0 };
    pos.x = m_view.getCenter().x - (m_width / 2) + 20;
    pos.y = m_view.getCenter().y - (m_height / 2) + 20;
    m_debugPanel->setPosition(pos);
    m_debugPanel->draw(m_engine->window());
  }

  void PlayLevel::drawColliders() {
    sf::RenderWindow &window = m_engine->window();
    const sf::Color lineColor = {255, 0, 0, 64};
    const sf::Color innactiveFillColor = {0, 255, 0, 32};
    const sf::Color activeFillColor = {255, 0, 0, 32};
    Vec2f colPos;
    sf::RectangleShape rect;
    rect.setOutlineColor(lineColor);
    rect.setOutlineThickness(-1);
    for (auto entity : m_entityManager.entities()){
      if (entity->hasComponent<CBoxCollider>() && entity->hasComponent<CTransform>()) {
        if (entity->getComponent<CBoxCollider>().colliding) {
          rect.setFillColor(activeFillColor);
        } else {
          rect.setFillColor(innactiveFillColor);
        }
        colPos = entity->getComponent<CTransform>().pos - entity->getComponent<CBoxCollider>().offset;
        rect.setPosition(colPos.toVector2f());
        rect.setSize(entity->getComponent<CBoxCollider>().size.toVector2f());
        window.draw(rect);
      }
    }
  }

  void PlayLevel::drawGrid() {
    sf::RenderWindow &window = m_engine->window();
    sf::Vector2f viewCenter = m_view.getCenter();
    Vec2f viewOrigin;
    Vec2f pos;
    sf::RectangleShape rect;
    rect.setSize(m_gridSize.toVector2f());
    rect.setOutlineColor(sf::Color(255, 255, 255, 128));
    rect.setOutlineThickness(-1);
    rect.setFillColor(sf::Color::Transparent);
    sf::Text blockName(*m_gridTextFont, "", m_gridFontSize);

    viewOrigin.x = viewCenter.x - ((float) m_width / 2);
    viewOrigin.y = viewCenter.y + ((float) m_height / 2);

    Vec2f gridBlock = gridBlockFromPixel(viewOrigin);
    Vec2f currentGridBlock;

    // TODO dynamically place grid based on view size instead of hardcoding
    for (int y = 0; y <= 12; y++)
      for (int x = 0; x <= 20; x++) {
        currentGridBlock.x = gridBlock.x + x;
        currentGridBlock.y = gridBlock.y + y;
        pos = gridBlockOrigin(currentGridBlock);
        rect.setPosition(pos.toVector2f());
        blockName.setString(std::format("{}, {}", (int) currentGridBlock.x, (int) currentGridBlock.y));
        blockName.setPosition(sf::Vector2f(pos.x + 5, pos.y + 50));

        m_engine->window().draw(rect);
        m_engine->window().draw(blockName);
      }
  }

  Vec2f PlayLevel::gridBlockOrigin(const Vec2f &gridCords) const {
    float y = m_worldOrigin.y;
    Vec2f origin = {0, y};
    origin.x += (m_gridSize.x * gridCords.x);
    origin.y -= (m_gridSize.y * gridCords.y);
    origin.y -= m_gridSize.y;
    return origin;
  }

  Vec2f PlayLevel::gridBlockFromPixel(Vec2f &pos) {
    int x = std::floor(pos.x);
    x -= (x % (int)std::floor(m_gridSize.x));

    int y = std::floor(pos.y) - m_engine->window().getSize().y;
    y -= (y % (int)std::floor(m_gridSize.y));

    return Vec2f(x / m_gridSize.x, -(y / m_gridSize.y));
  }

  // Derives a proper location from a grid coordinate onto
  // a world coordinate to place a sprite at the desired
  // grid position, ensuring the bottom of the sprite aligns
  // with the bottom line of the corresponding grid position.
  // Sprites bigger than 64 pixels need extra adjustment.
  // We assume all sprites set their origin at their center here.
  Vec2f PlayLevel::initialSpritePosition(const Vec2f &gridPos, const Vec2f &spriteSize) {
    // Grid block origin references top left corner of the block
    Vec2f worldPos = gridBlockOrigin(gridPos);

    worldPos.x += spriteSize.x / 2; // Offset to the right

    // Sprites are drawn from a point towards bottom-right.
    // Sprites that are 64 pixels tall need only half-size
    // on the Y offset. Taller sprites require more logic.
    if (spriteSize.y > m_gridSize.y) {

    } else {
      worldPos.y += spriteSize.y / 2; // Offset down
    }
    return worldPos;
  }

  // This is sketchy. Make it better.
  void PlayLevel::countRequiredCoins() {
    for (auto entity : m_entityManager.entities()) {
      if (!entity->hasComponent<CCoinReward>()) { continue; }
      m_levelRequiredCoins += entity->getComponent<CCoinReward>().amount;
    }
  }
}
