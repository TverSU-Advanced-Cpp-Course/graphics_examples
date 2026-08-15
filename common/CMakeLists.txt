add_library(ex_screenshot STATIC screenshot.cpp)
target_include_directories(ex_screenshot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex_screenshot PRIVATE stb::stb OpenGL::GL ex_warnings)

add_library(ex_gl_program STATIC gl_program.cpp)
target_include_directories(ex_gl_program PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex_gl_program PUBLIC glad::glad)
target_link_libraries(ex_gl_program PRIVATE ex_warnings)
