add_executable(gl_vbo main.cpp)
target_link_libraries(gl_vbo PRIVATE glfw glad::glad ex_gl_program ex_screenshot ex_warnings)

target_compile_definitions(gl_vbo PRIVATE EX_SHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shaders")
