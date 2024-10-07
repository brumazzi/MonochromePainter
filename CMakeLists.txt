cmake_minimum_required(VERSION 3.28)    # CMake version check
project(MonochromePainter)             # Create project "MonochromePainter"
set(CMAKE_CXX_STANDARD 17)              # Enable c++17 standard

# Add main.cpp file of the project root directory as a source file
set(SOURCE_FILES src/main.cpp src/object.cpp src/stage.cpp src/game.cpp src/npc.cpp src/item.cpp src/monster.cpp src/player.cpp src/assets.cpp src/door.cpp src/menu.cpp src/dynamic_ground.cpp src/steam.cpp)
include_directories(./include)

find_package(PkgConfig REQUIRED)
pkg_check_modules(OGRE REQUIRED IMPORTED_TARGET jsoncpp)
link_libraries(m dl pthread raylib jsoncpp steam_api)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wl,-rpath=./lib64")

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(MonochromePainter ${SOURCE_FILES})
# target_link_libraries(MonochromePainter PUBLIC PkgConfig::OGRE OgreBullet)

add_custom_command(TARGET MonochromePainter POST_BUILD COMMAND /bin/sh build_stages.sh)
# add_custom_command(TARGET MonochromePainter POST_BUILD COMMAND wine build.bat)
# add_custom_command(TARGET MonochromePainter POST_BUILD COMMAND /bin/sh create_zip.sh)
