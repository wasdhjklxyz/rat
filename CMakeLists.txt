cmake_minimum_required(VERSION 3.30)
project(rat LANGUAGES C)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

add_executable(agent)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(agent PRIVATE -g -O0 -Wall -Wextra)
else()
  target_compile_options(agent PRIVATE -O3 -DNDEBUG -flto)
  target_link_options(agent PRIVATE -s)
endif()

set_target_properties(agent PROPERTIES
  OUTPUT_NAME "agent$<$<CONFIG:Debug>:_debug>")

target_sources(agent PRIVATE src/agent.c)
