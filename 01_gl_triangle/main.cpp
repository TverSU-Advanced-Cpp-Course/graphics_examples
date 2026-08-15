//
// 01_gl_triangle — минимальная графическая программа: окно, контекст, цикл, своп.
//
// Загрузчик расширений здесь не подключён.
// Всё, что вызывается ниже, входит в OpenGL 1.1, а её opengl32.dll экспортирует
// напрямую. Для более новых вызовов понадобится glad (смотри пример 02).
//

#include <GLFW/glfw3.h>

#include "run_options.hpp"
#include "screenshot.hpp"

#include <cstdio>

namespace {

constexpr int kWidth = 800;
constexpr int kHeight = 600;

struct Vertex {
  float x, y;
  float r, g, b;
};

// Координаты сразу в NDC: видимая область экрана — это куб от -1 до 1.
// Никаких матриц пока нет, вершины кладутся туда, где они окажутся на экране.
constexpr Vertex kTriangle[] = {
    {0.0f, 0.5f, 1.0f, 0.0f, 0.0f},
    {-0.5f, -0.5f, 0.0f, 1.0f, 0.0f},
    {0.5f, -0.5f, 0.0f, 0.0f, 1.0f},
};

void error_callback(int code, const char *description) {
  std::fprintf(stderr, "GLFW error %d: %s\n", code, description);
}

void draw() {
  glClearColor(0.11f, 0.12f, 0.13f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glBegin(GL_TRIANGLES);
  for (const Vertex &v : kTriangle) {
    glColor3f(v.r, v.g, v.b);
    glVertex2f(v.x, v.y);
  }
  glEnd();
}

bool pressed_once(GLFWwindow *window, int key, bool &was_down) {
  const bool down = glfwGetKey(window, key) == GLFW_PRESS;
  const bool edge = down && !was_down;
  was_down = down;
  return edge;
}

} // namespace

int main(int argc, char **argv) {
  const ex::RunOptions opts = ex::parse_options(argc, argv);

  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
    return 1;

  GLFWwindow *window =
      glfwCreateWindow(kWidth, kHeight, "01 - immediate mode", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  std::printf("Esc — выход, F12 — сохранить кадр в 01_gl_triangle.png\n");
  std::printf("OpenGL: %s\n", glGetString(GL_VERSION));

  bool f12_was_down = false;
  int frame = 0;
  while (!glfwWindowShouldClose(window)) {
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    draw();

    if (pressed_once(window, GLFW_KEY_F12, f12_was_down))
      ex::save_screenshot(width, height, "01_gl_triangle.png");

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
}
