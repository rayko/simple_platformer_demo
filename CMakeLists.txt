cmake_minimum_required(VERSION 3.28)
project(simple_platformer_demo LANGUAGES CXX VERSION 0.0.1)

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
add_executable(simple_platformer_demo)

target_sources(simple_platformer_demo PRIVATE src/main.cpp)
target_sources(simple_platformer_demo PRIVATE src/Core.cpp)
target_sources(simple_platformer_demo PRIVATE src/Animation.cpp)
target_sources(simple_platformer_demo PRIVATE src/Action.cpp)
target_sources(simple_platformer_demo PRIVATE src/Entity.cpp)
target_sources(simple_platformer_demo PRIVATE src/EntityManager.cpp)
target_sources(simple_platformer_demo PRIVATE src/ui/TextPanel.cpp)
target_sources(simple_platformer_demo PRIVATE src/ui/SelectableListPanel.cpp)
target_sources(simple_platformer_demo PRIVATE src/scenes/Base.cpp)
target_sources(simple_platformer_demo PRIVATE src/scenes/Menu.cpp)
target_sources(simple_platformer_demo PRIVATE src/scenes/PlayLevel.cpp)
target_sources(simple_platformer_demo PRIVATE src/scenes/Editor.cpp)
target_sources(simple_platformer_demo PRIVATE src/Physics.cpp)
target_sources(simple_platformer_demo PRIVATE src/GameEngine.cpp)
target_sources(simple_platformer_demo PRIVATE src/Logger.cpp)
target_sources(simple_platformer_demo PRIVATE src/Vec2f.cpp)
target_sources(simple_platformer_demo PRIVATE src/AssetStore.cpp)

set_target_properties(simple_platformer_demo PROPERTIES CXX_SCAN_FOR_MODULES OFF)
target_compile_features(simple_platformer_demo PRIVATE cxx_std_20)
target_link_libraries(simple_platformer_demo PRIVATE SFML::Graphics SFML::Window SFML::System)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)

# This makes GAME_VERSION avaialble inside code
target_compile_definitions(simple_platformer_demo PRIVATE GAME_VERSION="${PROJECT_VERSION}")

install(TARGETS simple_platformer_demo DESTINATION .)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets/" DESTINATION .)

# Copy assets/* to bin/ on build
# if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
#     add_custom_command(
#         TARGET simple_platformer_demo POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_directory
#                 "${CMAKE_CURRENT_SOURCE_DIR}/assets"
#                 "$<TARGET_FILE_DIR:simple_platformer_demo>"
#         COMMENT "Copying assets directly to output binary directory..."
#     )
# endif()

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_NAME "simple_platformer_demo")
set(CPACK_PACKAGE_VENDOR "Rayko")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Simple 2D Platformer Demo built with C++ and SFML")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

if(WIN32)
  get_filename_component(MINGW_BIN_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
  install(FILES
    "${MINGW_BIN_DIR}/libgcc_s_seh-1.dll"
    "${MINGW_BIN_DIR}/libstdc++-6.dll"
    "${MINGW_BIN_DIR}/libwinpthread-1.dll"
    DESTINATION .
    OPTIONAL
  )
  target_link_options(simple_platformer_demo PRIVATE -static-libgcc -static-libstdc++ -static)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/resource.rc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/resource.rc"
    @ONLY
  )

  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/icon32.ico"
    "${CMAKE_CURRENT_BINARY_DIR}/icon32.ico"
    COPYONLY
  )

  target_sources(simple_platformer_demo PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/resource.rc")
endif()

if(WIN32)
  set(CPACK_GENERATOR "ZIP")
else()
  set(CPACK_GENERATOR "TGZ")
endif()

# Linux desktop/icon thingies
if(UNIX AND NOT APPLE)
  install(
    FILES resources/simple_platformer_demo.desktop
    DESTINATION share/applications
  )

  install(
    FILES resources/icon32.png
    DESTINATION share/icons/hicolor/256x256/apps
    RENAME simple_platformer_demo.png
  )
endif()


include(CPack)
