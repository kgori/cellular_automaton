cmake_minimum_required(VERSION 3.19)
project(cellular_automaton)

find_package(SFML COMPONENTS system window graphics REQUIRED)

set(CMAKE_CXX_STANDARD 23)

add_executable(cellular_automaton main.cpp include/grid.h include/sfml_renderer.h)
target_include_directories(cellular_automaton PUBLIC include)
target_link_libraries(cellular_automaton PRIVATE FLAC sfml-system sfml-network sfml-graphics sfml-audio sfml-window)
