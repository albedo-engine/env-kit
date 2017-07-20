#include <shaders/shader.hpp>

namespace albedo
{

namespace tools
{

namespace shader
{

Shader::Shader(const GLchar* vertexShader, const GLchar* fragmentShader)
      : vertexShader_{vertexShader}
      , fragmentShader_{fragmentShader}
{ }

bool
Shader::compile()
{
  GLuint vertex;
  GLuint fragment;

  // Vertex Shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertexShader_, NULL);
  glCompileShader(vertex);
  if (!hasCompiled(vertex))
    return false;

  // Fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragmentShader_, NULL);
  glCompileShader(fragment);
  if (!hasCompiled(fragment))
    return false;

  // Link shaders
  program_ = glCreateProgram();
  glAttachShader(program_, vertex);
  glAttachShader(program_, fragment);
  glLinkProgram(program_);

  // Check for linking errors
  GLint success;
  glGetProgramiv(program_, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(program_, 512, NULL, compileInfo_);
    return false;
  };

  // Delete shaders
  glDeleteShader(vertex);
  glDeleteShader(fragment);

  return true;
}

void Shader::use()
{
  glUseProgram(program_);
}

bool
Shader::hasCompiled(GLuint shader)
{
  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(shader, 512, NULL, compileInfo_);
    return false;
  }
  return true;
}

void
Shader::printError()
{
  std::cerr << compileInfo_ << std::endl;
}

}

}

}
