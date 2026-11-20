# CMakeLists.txt -- build instructions for the Space Invaders project.
# This is the same file as in the environment check, with more source files.
cmake_minimum_required(VERSION 3.28)
project(SpaceInvaders LANGUAGES CXX)

# Use the same C++ standard as the course.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This game only needs graphics and a window.
set(SFML_BUILD_AUDIO OFF CACHE BOOL "Build SFML audio" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "Build SFML networking" FORCE)

# Download and build the course's SFML version (first build only).
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(SFML)

# Your files (paste your Elice solutions here) and the provided files.
add_executable(space_invaders
    PA1.cpp
    PA2.cpp
    visual/Draw.cpp
    visual/Game.cpp
    visual/Input.cpp
    visual/PixelFont.cpp
    visual/main.cpp
)
target_include_directories(space_invaders PRIVATE headers visual)
target_link_libraries(space_invaders PRIVATE SFML::Graphics)
