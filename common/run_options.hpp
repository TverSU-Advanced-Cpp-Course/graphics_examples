#pragma once

#include <string>
#include <string_view>

namespace ex {

struct RunOptions {
  std::string screenshot_path;
  int warmup_frames = 3;

  bool batch() const { return !screenshot_path.empty(); }
};

inline RunOptions parse_options(int argc, char **argv) {
  RunOptions opts;
  for (int i = 1; i < argc; ++i) {
    const std::string_view arg = argv[i];
    if (arg == "--screenshot" && i + 1 < argc)
      opts.screenshot_path = argv[++i];
  }
  return opts;
}

} // namespace ex
