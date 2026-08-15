add_executable(gl_triangle main.cpp)
target_link_libraries(gl_triangle PRIVATE glfw OpenGL::GL ex_screenshot ex_warnings)
