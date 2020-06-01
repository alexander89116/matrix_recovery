cmake_minimum_required(VERSION 3.12)
project(matrix)

set(CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=undefined")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=undefined")

add_executable(matrix main.cpp)

add_subdirectory(googletest)

include_directories(googletest/googletest/include)
include_directories(googletest/googlemock/include)

target_link_libraries(matrix gtest gtest_main)