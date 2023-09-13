cmake_minimum_required(VERSION 3.26)
project(CARnary_lib)

set(CMAKE_CXX_STANDARD 17)

add_library(CARnary_lib SHARED src/library.cpp)
