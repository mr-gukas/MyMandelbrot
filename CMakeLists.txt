cmake_minimum_required(VERSION 3.16)
project(frac)

set(CMAKE_CXX_STANDARD 14)

find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

add_compile_options(-Ofast -mavx2)

add_executable(frac main.cpp mandelbrot.cpp)
target_link_libraries(frac sfml-graphics sfml-window sfml-system sfml-audio)

