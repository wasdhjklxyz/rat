add_executable(agent)

target_link_libraries(agent PRIVATE compile_options)

target_compile_definitions(agent PRIVATE
  C2_PORT=${C2_PORT}
  C2_HOST=\"${C2_HOST}\")

set_target_properties(agent PROPERTIES
  OUTPUT_NAME "agent$<$<CONFIG:Debug>:_debug>")

target_sources(agent PRIVATE main.c socket.c)
