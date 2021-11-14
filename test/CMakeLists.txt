cmake_minimum_required(VERSION 3.18.0)

project(2dpoint CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wreturn-type -Wempty-body -Wno-deprecated")

include_directories(../)
link_directories(../)
add_executable(2dpoint 2dpoint.cpp)
add_executable(2dquad 2dquad.cpp)
target_link_libraries(2dpoint kdt)
target_link_libraries(2dquad kdt)
