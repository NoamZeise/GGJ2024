add_executable(CatFlat
  main.cpp
  cooking.cpp
  drawable.cpp
  fish.cpp
  menu.cpp
  house.cpp
)
target_link_options(CatFlat PRIVATE ${SUBSYSTEM_LINKER_OPTIONS})
target_link_libraries(CatFlat GraphicsEnv)
set(exec-name CatFlat)
include("${CMAKE_CURRENT_SOURCE_DIR}/../Graphics-Environment/cmake/copy-resources.cmake")
add_custom_command(TARGET CatFlat POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${CMAKE_CURRENT_SOURCE_DIR}/../resources"
  $<TARGET_FILE_DIR:CatFlat>)
