cmake_minimum_required(VERSION 3.3)

project(test)

find_package(VTK QUIET)

include(${VTK_USE_FILE})
add_executable(draw_2dpoint MACOSX_BUNDLE draw_2dpoint.cpp)
target_link_libraries(draw_2dpoint PRIVATE ${VTK_LIBRARIES})
