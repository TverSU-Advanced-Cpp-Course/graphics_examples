add_executable(gl_cube main.cpp)
target_link_libraries(gl_cube
    PRIVATE glfw glad::glad glm::glm-header-only ex_gl_program ex_screenshot ex_warnings)
target_compile_definitions(gl_cube PRIVATE EX_SHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shaders")
