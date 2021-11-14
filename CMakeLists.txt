cmake_minimum_required(VERSION 3.18.0)

project(kdt CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wreturn-type -Wempty-body -Wno-deprecated")

file(GLOB SOURCES "*.cpp")

add_library(kdt SHARED ${SOURCES})
