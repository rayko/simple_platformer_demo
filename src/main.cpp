#include "GameEngine.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  std::cout << "Begin Program" << std::endl;

  GameEngine engine;
  engine.init("config.txt");
  engine.run();

  std::cout << "End Program" << std::endl;
}
