cmake_minimum_required(VERSION 3.3)

project(draw)

find_package(VTK QUIET)

include_directories(../)
add_executable(draw MACOSX_BUNDLE draw.cpp)
target_link_libraries(draw PRIVATE ${VTK_LIBRARIES})
