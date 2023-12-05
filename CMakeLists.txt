cmake_minimum_required(VERSION 3.2)
project(Breakout)

set(CMAKE_CXX_STANDARD 11)
# include_directories(${CMAKE_SOURCE_DIR}/include/PortAudio)
include_directories(${CMAKE_SOURCE_DIR}/include/yse)
# include_directories(${CMAKE_SOURCE_DIR}/include/sndfile)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(src)