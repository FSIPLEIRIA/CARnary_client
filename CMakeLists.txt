cmake_minimum_required(VERSION 3.22)
project(CARnary_lib)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_library(CARnary_lib SHARED
        src/CARnaryClient.cpp
        src/Utils.cpp)

# installation rules
install(TARGETS CARnary_lib LIBRARY DESTINATION /usr/lib/CARnary_lib)
install(DIRECTORY include/carnary DESTINATION /usr/include)
install(FILES CARnary_libConfig.cmake DESTINATION /usr/lib/CARnary_lib)
