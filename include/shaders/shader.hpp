#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GL/gl.h>

namespace albedo
{

namespace tools
{

namespace shader
{

class Shader
{
  public:
    Shader(const GLchar* vertexShader, const GLchar* fragmentShader);

  public:
    bool
    compile();

    void
    use();

    inline GLuint
    id() { return program_; }

    void
    printError();

  private:
    bool
    hasCompiled(GLuint shader);

  private:
    GLuint program_;
    GLchar compileInfo_[512];

    const char*   vertexShader_;
    const char*   fragmentShader_;
};

} // namespace shader

} // namespace tools

} // namespace albedo
