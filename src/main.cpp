#include "GameEngine.hpp"
#include "Logger.hpp"

int main(int argc, char *argv[]) {
  std::shared_ptr<Logger> logger = std::make_shared<Logger>();
  logger->setLevel(Logger::Level::Debug);

  GameEngine engine(logger);
  engine.loadConfigs("config.txt");
  engine.loadAssets("assets.txt");

  engine.run();

  logger->debug("End Program");
}
