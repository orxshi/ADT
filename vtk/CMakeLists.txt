cmake_minimum_required(VERSION 3.3)

project(draw)

find_package(VTK QUIET)

set(CMAKE_CXX_STANDARD 17)

include(${VTK_USE_FILE})
add_executable(draw MACOSX_BUNDLE draw.cpp)
target_link_libraries(draw PRIVATE ${VTK_LIBRARIES} stdc++fs)

