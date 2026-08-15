#include "gl_program.hpp"

#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

std::string read_file(const std::filesystem::path &path) {
  std::ifstream in(path, std::ios::binary);
  if (!in)
    throw std::runtime_error("не открывается файл шейдера: " + path.string());

  std::ostringstream buf;
  buf << in.rdbuf();
  return buf.str();
}

std::string shader_log(GLuint shader) {
  GLint length = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
  std::vector<char> log(static_cast<std::size_t>(length) + 1, '\0');
  glGetShaderInfoLog(shader, length, nullptr, log.data());
  return log.data();
}

std::string program_log(GLuint program) {
  GLint length = 0;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
  std::vector<char> log(static_cast<std::size_t>(length) + 1, '\0');
  glGetProgramInfoLog(program, length, nullptr, log.data());
  return log.data();
}

GLuint compile(GLenum stage, const std::filesystem::path &path) {
  const std::string source = read_file(path);
  const char *ptr = source.c_str();

  GLuint shader = glCreateShader(stage);
  glShaderSource(shader, 1, &ptr, nullptr);
  glCompileShader(shader);

  GLint ok = GL_FALSE;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
  if (!ok) {
    const std::string log = shader_log(shader);
    glDeleteShader(shader);
    throw std::runtime_error(path.string() + ": " + log);
  }
  return shader;
}

} // namespace

namespace ex {

unsigned int make_program(const std::filesystem::path &vertex_path,
                          const std::filesystem::path &fragment_path) {
  const GLuint vertex = compile(GL_VERTEX_SHADER, vertex_path);
  const GLuint fragment = compile(GL_FRAGMENT_SHADER, fragment_path);

  const GLuint program = glCreateProgram();
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);

  glDeleteShader(vertex);
  glDeleteShader(fragment);

  GLint ok = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &ok);
  if (!ok) {
    const std::string log = program_log(program);
    glDeleteProgram(program);
    throw std::runtime_error("линковка программы: " + log);
  }
  return program;
}

} // namespace ex
