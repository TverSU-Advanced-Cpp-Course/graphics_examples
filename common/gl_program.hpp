#pragma once

#include <filesystem>

namespace ex {

unsigned int make_program(const std::filesystem::path &vertex_path,
                          const std::filesystem::path &fragment_path);

} // namespace ex
