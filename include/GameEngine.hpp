/*
  GameEngine.hpp
  Core class to orchestrate the high level functionality
  of the game. We make use of the rest of things here, and
  provide the main run loop.
*/

#include <string>

class GameEngine {

public:
  GameEngine();
  void init(const std::string & configFile);
  void run();
};
