#include "GameEngine.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  std::cout << "Begin Program" << std::endl;

  GameEngine engine;
  engine.setDebugMode(true);
  engine.loadConfigs("config.txt");
  engine.loadAssets("assets.txt");

  engine.run();

  std::cout << "End Program" << std::endl;
}
