cmake_minimum_required(VERSION 3.14)

include("Graphics-Environment/cmake/project-setup.cmake")

set(ProjectName "GGJ2024")

if(BUILD_STATIC)
  set(GRAPHICS_BUILD_STATIC ON)
  if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  endif()
endif()

project(GGJ2024-Project VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(Graphics-Environment)
add_subdirectory(src)
