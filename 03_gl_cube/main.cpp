//
// 03_gl_cube — переход от плоскости к объёму.
//
// К примеру 02 добавляются ровно четыре вещи, и каждая из них обязательна:
//   * матрицы модели, вида и проекции — иначе объём некуда девать;
//   * индексный буфер — 8 вершин вместо 36;
//   * буфер глубины — иначе дальние грани перекрывают ближние;
//   * отсечение задних граней — половина треугольников не растеризуется зря.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gl_program.hpp"
#include "run_options.hpp"
#include "screenshot.hpp"

#include <cstdio>
#include <exception>

namespace {

constexpr int kWidth = 800;
constexpr int kHeight = 600;

struct Vertex {
  float x, y, z;
  float r, g, b;
};

constexpr Vertex kVertices[] = {
    {-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f}, // 0
    {1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f},  // 1
    {1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f},   // 2
    {-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f},  // 3
    {-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f},  // 4
    {1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f},   // 5
    {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},    // 6
    {-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f},   // 7
};

constexpr unsigned char kIndices[] = {
    4, 5, 6, 4, 6, 7, // передняя,  z = +1
    1, 0, 3, 1, 3, 2, // задняя,    z = -1
    0, 4, 7, 0, 7, 3, // левая,     x = -1
    5, 1, 2, 5, 2, 6, // правая,    x = +1
    0, 1, 5, 0, 5, 4, // нижняя,    y = -1
    3, 7, 6, 3, 6, 2, // верхняя,   y = +1
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

GLuint make_cube_vao() {
  GLuint vao = 0, vbo = 0, ebo = 0;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(kVertices), kVertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(kIndices), kIndices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
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

  GLFWwindow *window = glfwCreateWindow(kWidth, kHeight, "03 - куб и матрицы",
                                        nullptr, nullptr);
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

  const GLuint program = ex::make_program(EX_SHADER_DIR "/cube.vert",
                                          EX_SHADER_DIR "/cube.frag");
  const GLuint vao = make_cube_vao();

  const GLint loc_model = glGetUniformLocation(program, "uModel");
  const GLint loc_view = glGetUniformLocation(program, "uView");
  const GLint loc_projection = glGetUniformLocation(program, "uProjection");

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  std::printf("Esc — выход, F12 — сохранить кадр\n");

  bool f12_was_down = false;
  int frame = 0;
  while (!glfwWindowShouldClose(window)) {
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glClearColor(0.11f, 0.12f, 0.13f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const float t = opts.batch() ? 0.6f : static_cast<float>(glfwGetTime());

    const glm::mat4 model =
        glm::rotate(glm::mat4(1.0f), t * 0.8f, glm::normalize(glm::vec3(0.4f, 1.0f, 0.2f)));

    const glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f),
                                       glm::vec3(0.0f, 0.0f, 0.0f),
                                       glm::vec3(0.0f, 1.0f, 0.0f));

    const float aspect =
        static_cast<float>(width) / static_cast<float>(height > 0 ? height : 1);
    const glm::mat4 projection =
        glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

    glUseProgram(program);
    glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(loc_view, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(loc_projection, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, nullptr);

    if (pressed_once(window, GLFW_KEY_F12, f12_was_down))
      ex::save_screenshot(width, height, "03_gl_cube.png");

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
