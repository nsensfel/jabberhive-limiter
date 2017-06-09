add_subdirectory(error)
add_subdirectory(core)
add_subdirectory(filter)
add_subdirectory(server)
add_subdirectory(parameters)

set(
   SRC_FILES ${SRC_FILES}
   ${CMAKE_CURRENT_SOURCE_DIR}/main.c
)

set(SRC_FILES ${SRC_FILES} PARENT_SCOPE)
