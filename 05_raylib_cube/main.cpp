//
// 05_raylib_cube — куб из примера 03, но матрицы считает raylib.
//
// Сравнивать нужно не длину файла, а то, что исчезло:
//   * glm и три матрицы — их строит BeginMode3D из описания камеры;
//   * буфер глубины включается сам, отсечение задних граней тоже;
//   * uniform-переменные и glGetUniformLocation не упоминаются вовсе.
//
// Исчезла и часть возможностей: цвет у DrawCube один на всю грань, а чтобы
// раскрасить вершины по-своему, придётся идти в rlgl или писать шейдер.
//

#include <raylib.h>
#include <rlgl.h>

#include <cstdio>
#include <cstring>

namespace {

constexpr int kWidth = 800;
constexpr int kHeight = 600;
constexpr Color kBackground{28, 31, 33, 255};

} // namespace

int main(int argc, char **argv) {
  const char *screenshot_path = nullptr;
  for (int i = 1; i < argc; ++i)
    if (std::strcmp(argv[i], "--screenshot") == 0 && i + 1 < argc)
      screenshot_path = argv[++i];

  SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
  InitWindow(kWidth, kHeight, "05 - куб на raylib");

  Camera3D camera{};
  camera.position = Vector3{0.0f, 0.0f, 5.0f};
  camera.target = Vector3{0.0f, 0.0f, 0.0f};
  camera.up = Vector3{0.0f, 1.0f, 0.0f};
  camera.fovy = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  std::printf("Esc — выход, F12 — сохранить кадр, ЛКМ — вращать камеру\n");

  int frame = 0;
  while (!WindowShouldClose()) {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
      UpdateCamera(&camera, CAMERA_THIRD_PERSON);

    const float t = (screenshot_path != nullptr)
                        ? 0.48f
                        : static_cast<float>(GetTime()) * 0.8f;

    BeginDrawing();
    ClearBackground(kBackground);

    BeginMode3D(camera);

    rlPushMatrix();
    rlRotatef(t * 180.0f / PI, 0.4f, 1.0f, 0.2f);
    DrawCube(Vector3{0, 0, 0}, 2.0f, 2.0f, 2.0f, SKYBLUE);
    DrawCubeWires(Vector3{0, 0, 0}, 2.0f, 2.0f, 2.0f, DARKBLUE);
    rlPopMatrix();

    EndMode3D();

    DrawText("BeginMode3D == uProjection * uView from example 03", 12,
             kHeight - 28, 16, LIGHTGRAY);

    EndDrawing();

    if (IsKeyPressed(KEY_F12))
      TakeScreenshot("05_raylib_cube.png");

    if (screenshot_path != nullptr && ++frame >= 3) {
      TakeScreenshot(screenshot_path);
      break;
    }
  }

  CloseWindow();
  return 0;
}
