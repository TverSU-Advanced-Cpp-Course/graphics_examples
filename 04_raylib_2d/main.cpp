//
// 04_raylib_2d — тот же треугольник в третий раз, теперь на raylib.
//
// Сравните объём с примерами 01 и 02. Окно, контекст, загрузчик расширений,
// шейдеры, VAO, VBO — всё это никуда не делось, просто теперь оно внутри
// InitWindow.
//
// Рисование идёт через rlgl — слой внутри raylib, который изображает старый
// immediate mode поверх OpenGL 3.3. Код ниже выглядит почти как пример 01,
// но ведёт себя как пример 02: вершины копятся в буфере и уезжают на
// видеокарту одной командой.
//

#include <raylib.h>
#include <rlgl.h>

#include <cstdio>
#include <cstring>

namespace {

constexpr int kWidth = 800;
constexpr int kHeight = 600;

constexpr Color kBackground{28, 31, 33, 255};

void draw_triangle() {
  rlBegin(RL_TRIANGLES);
  rlColor4ub(255, 0, 0, 255);
  rlVertex2f(400.0f, 150.0f);
  rlColor4ub(0, 255, 0, 255);
  rlVertex2f(200.0f, 450.0f);
  rlColor4ub(0, 0, 255, 255);
  rlVertex2f(600.0f, 450.0f);
  rlEnd();
}

} // namespace

int main(int argc, char **argv) {
  const char *screenshot_path = nullptr;
  for (int i = 1; i < argc; ++i)
    if (std::strcmp(argv[i], "--screenshot") == 0 && i + 1 < argc)
      screenshot_path = argv[++i];

  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(kWidth, kHeight, "04 - тот же треугольник на raylib");

  std::printf("Esc — выход, F12 — сохранить кадр\n");

  int frame = 0;
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(kBackground);

    draw_triangle();

    EndDrawing();

    if (IsKeyPressed(KEY_F12))
      TakeScreenshot("04_raylib_2d.png");

    if (screenshot_path != nullptr && ++frame >= 3) {
      TakeScreenshot(screenshot_path);
      break;
    }
  }

  CloseWindow();
  return 0;
}
