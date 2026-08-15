//
// 02_gl_vbo — тот же треугольник, что и в примере 01, современным способом.
//
// Картинка совпадает пиксель в пиксель, но:
//   * вершины лежат в видеопамяти и в кадре никуда не едут;
//   * что делать с вершинами и пикселями, описано шейдерами, а не зашито в API;
//   * весь треугольник рисуется одной командой вместо восьми вызовов.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gl_program.hpp"
#include "run_options.hpp"
#include "screenshot.hpp"

#include <cstdio>
#include <exception>

namespace {

constexpr int kWidth = 800;
constexpr int kHeight = 600;

struct Vertex {
  float x, y;
  float r, g, b;
};

constexpr Vertex kTriangle[] = {
    {0.0f, 0.5f, 1.0f, 0.0f, 0.0f},
    {-0.5f, -0.5f, 0.0f, 1.0f, 0.0f},
    {0.5f, -0.5f, 0.0f, 0.0f, 1.0f},
};

void error_callback(int code, const char *description) {
  std::fprintf(stderr, "GLFW error %d: %s\n", code, description);
}

bool pressed_once(GLFWwindow *window, int key, bool &was_down) {
  const bool down = glfwGetKey(window, key) == GLFW_PRESS;
  const bool edge = down && !was_down;
  was_down = down;
  return edge;
}

GLuint make_triangle_vao() {
  GLuint vao = 0, vbo = 0;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBufferData(GL_ARRAY_BUFFER, sizeof(kTriangle), kTriangle, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void *>(offsetof(Vertex, x)));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void *>(offsetof(Vertex, r)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  return vao;
}

} // namespace

int main(int argc, char **argv) try {
  const ex::RunOptions opts = ex::parse_options(argc, argv);

  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
    return 1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(kWidth, kHeight, "02 - VBO и шейдеры", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    std::fprintf(stderr, "не удалось загрузить функции OpenGL\n");
    glfwTerminate();
    return 1;
  }
  std::printf("OpenGL: %s\n", glGetString(GL_VERSION));

  const GLuint program = ex::make_program(EX_SHADER_DIR "/triangle.vert",
                                          EX_SHADER_DIR "/triangle.frag");
  const GLuint vao = make_triangle_vao();

  std::printf("Esc — выход, F12 — сохранить кадр\n");

  bool f12_was_down = false;
  int frame = 0;
  while (!glfwWindowShouldClose(window)) {
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glClearColor(0.11f, 0.12f, 0.13f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    if (pressed_once(window, GLFW_KEY_F12, f12_was_down))
      ex::save_screenshot(width, height, "02_gl_vbo.png");

    if (opts.batch() && ++frame >= opts.warmup_frames) {
      ex::save_screenshot(width, height, opts.screenshot_path);
      break;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GLFW_TRUE);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
} catch (const std::exception &e) {
  std::fprintf(stderr, "ошибка: %s\n", e.what());
  glfwTerminate();
  return 1;
}
