cmake_minimum_required(VERSION 3.28)    # CMake version check
project(MonocromePainter)             # Create project "MonocromePainter"
set(CMAKE_CXX_STANDARD 17)              # Enable c++17 standard

# Add main.cpp file of the project root directory as a source file
set(SOURCE_FILES src/main.cpp src/object.cpp src/stage.cpp src/game.cpp src/npc.cpp src/item.cpp src/monster.cpp src/player.cpp src/assets.cpp src/door.cpp src/menu.cpp src/dynamic_ground.cpp)
include_directories(./include)

find_package(PkgConfig REQUIRED)
pkg_check_modules(OGRE REQUIRED IMPORTED_TARGET jsoncpp)
link_libraries(m dl pthread raylib jsoncpp)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(MonocromePainter ${SOURCE_FILES})
# target_link_libraries(MonocromePainter PUBLIC PkgConfig::OGRE OgreBullet)

# add_custom_command(TARGET MonocromePainter POST_BUILD COMMAND /bin/sh build_stages.sh)
add_custom_command(TARGET MonocromePainter POST_BUILD COMMAND /bin/sh windows-build.sh)