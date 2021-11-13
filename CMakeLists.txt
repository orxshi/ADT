cmake_minimum_required(VERSION 3.18.0)

project(kdt CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wreturn-type -Wempty-body -Wno-deprecated")

file(GLOB SOURCES "*.cpp")
#add_executable(kdt ${SOURCES})

add_executable(2dpoint 2dpoint.cpp)
#add_executable(someotherexe relatedfile)
add_library(kdt node.cpp kdt.cpp object.cpp)
target_link_libraries(2dpoint kdt)
#target_link_libraries(someotherexe kdt)
