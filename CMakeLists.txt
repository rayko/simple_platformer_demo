cmake_minimum_required(VERSION 3.28)
project(SimplePlatformerDemo LANGUAGES CXX VERSION 0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPERIMENTAL_CXX_MODULES OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

include_directories(include)
add_executable(simple_platformer_demo src/main.cpp
  src/Core.cpp
  src/Animation.cpp
  src/Action.cpp
  src/Entity.cpp
  src/EntityManager.cpp
  src/Scene.cpp
  src/GameEngine.cpp
  src/Logger.cpp
  src/Vec2f.cpp
  src/AssetStore.cpp)
set_target_properties(simple_platformer_demo PROPERTIES CXX_SCAN_FOR_MODULES OFF)
target_compile_features(simple_platformer_demo PRIVATE cxx_std_20)
target_link_libraries(simple_platformer_demo PRIVATE SFML::Graphics SFML::Window SFML::System)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)
