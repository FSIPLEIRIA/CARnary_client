cmake_minimum_required(VERSION 3.26)
project(CARnary_lib)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_library(CARnary_lib SHARED
        src/CARnaryClient.cpp
        src/Utils.cpp)

# TODO: create installation rules
