add_executable(GGJ2024
  main.cpp
  cooking.cpp
  drawable.cpp
  fish.cpp
)
target_link_options(GGJ2024 PRIVATE ${SUBSYSTEM_LINKER_OPTIONS})
target_link_libraries(GGJ2024 GraphicsEnv)
set(exec-name GGJ2024)
include("${CMAKE_CURRENT_SOURCE_DIR}/../Graphics-Environment/cmake/copy-resources.cmake")
add_custom_command(TARGET GGJ2024 POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${CMAKE_CURRENT_SOURCE_DIR}/../resources"
  $<TARGET_FILE_DIR:GGJ2024>)
