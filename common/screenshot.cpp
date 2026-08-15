#include "screenshot.hpp"

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <vector>

namespace ex {

bool save_screenshot(int width, int height, const std::string &path) {
  if (width <= 0 || height <= 0)
    return false;

  std::vector<unsigned char> pixels(static_cast<std::size_t>(width) * height * 4);

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

  stbi_flip_vertically_on_write(1);
  return stbi_write_png(path.c_str(), width, height, 4, pixels.data(),
                        width * 4) != 0;
}

} // namespace ex
