cmake_minimum_required(VERSION 3.30)
project(rat LANGUAGES C)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

add_executable(implant)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(implant PRIVATE -g -O0 -Wall -Wextra)
else()
  target_compile_options(implant PRIVATE -O3 -DNDEBUG -flto)
  target_link_options(implant PRIVATE -s)
endif()

set_target_properties(implant PROPERTIES
  OUTPUT_NAME "implant$<$<CONFIG:Debug>:_debug>")

target_sources(implant PRIVATE src/test.c)
