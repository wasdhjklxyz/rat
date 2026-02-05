add_executable(c2)

target_link_libraries(c2 PRIVATE compile_options)

target_compile_definitions(c2 PRIVATE C2_PORT=${C2_PORT})

set_target_properties(c2 PROPERTIES
  OUTPUT_NAME "c2$<$<CONFIG:Debug>:_debug>")

target_sources(c2 PRIVATE main.c net.c)
