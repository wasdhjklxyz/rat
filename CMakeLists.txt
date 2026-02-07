cmake_minimum_required(VERSION 3.30)
project(rat LANGUAGES C)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(C2_HOST "" CACHE STRING "C2 host")
set(C2_PORT "" CACHE STRING "C2 port")

add_library(compile_options INTERFACE)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(compile_options INTERFACE -g -O0 -Wall -Wextra)
else()
  target_compile_options(compile_options INTERFACE -O3 -DNDEBUG -flto)
  target_link_options(compile_options INTERFACE -s)
endif()

add_subdirectory(src)
