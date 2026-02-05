cmake_minimum_required(VERSION 3.30)
project(rat LANGUAGES C)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

set(C2_HOST "" CACHE STRING "C2 host")
set(C2_PORT "" CACHE STRING "C2 port")

add_executable(agent)
add_executable(c2)
add_library(compile_options INTERFACE)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(compile_options INTERFACE -g -O0 -Wall -Wextra)
else()
  target_compile_options(compile_options INTERFACE -O3 -DNDEBUG -flto)
  target_link_options(compile_options INTERFACE -s)
endif()

set_target_properties(agent PROPERTIES
  OUTPUT_NAME "agent$<$<CONFIG:Debug>:_debug>")
set_target_properties(c2 PROPERTIES
  OUTPUT_NAME "c2$<$<CONFIG:Debug>:_debug>")

target_compile_definitions(agent PRIVATE
  C2_PORT=${C2_PORT}
  C2_HOST=\"${C2_HOST}\")
target_compile_definitions(c2 PRIVATE C2_PORT=${C2_PORT})

target_link_libraries(agent PRIVATE compile_options)
target_link_libraries(c2 PRIVATE compile_options)

target_sources(agent PRIVATE src/agent/main.c)
target_sources(c2 PRIVATE src/c2/main.c)
