cmake_minimum_required(VERSION 2.8)

INCLUDE(FindPkgConfig)

project(cdgplay)

file(GLOB SOURCES
    libCDG/src/*.cpp
)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

PKG_SEARCH_MODULE(SDL2_MIXER REQUIRED SDL2_mixer)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} "main.cpp" ${SOURCES})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_MIXER_LIBRARIES})
