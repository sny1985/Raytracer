cmake_minimum_required(VERSION 3.0.0)
project(Raytracer VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 11)

include_directories (.)
include_directories (glm)
include_directories (stb)

add_executable(Raytracer main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
