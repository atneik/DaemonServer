cmake_minimum_required(VERSION 2.8)

PROJECT(DaemonServer)
SET(CPACK_CMAKE_GENERATOR "Unix Makefiles")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_definitions(${CMAKE_CURRENT_SOURCE_DIR}/src)

ADD_SUBDIRECTORY( src )