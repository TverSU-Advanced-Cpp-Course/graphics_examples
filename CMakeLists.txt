cmake_minimum_required(VERSION 3.28)
project(graphics_examples VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)
find_package(glm REQUIRED)
find_package(raylib REQUIRED)
find_package(stb REQUIRED)

find_package(OpenGL REQUIRED)

add_library(ex_warnings INTERFACE)
if(MSVC)
    target_compile_options(ex_warnings INTERFACE /W4 /permissive- /utf-8)
else()
    target_compile_options(ex_warnings INTERFACE -Wall -Wextra -Wpedantic)
endif()

add_subdirectory(common)

add_subdirectory(01_gl_triangle)
add_subdirectory(02_gl_vbo)
add_subdirectory(03_gl_cube)
add_subdirectory(04_raylib_2d)
add_subdirectory(05_raylib_cube)
