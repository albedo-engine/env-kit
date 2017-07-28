#pragma once

#include <iostream>

#if ALBEDO_TOOLS_MODE <= ALBEDO_TBB_GPU_MODE || ALBEDO_TOOLS_MODE ==\
ALBEDO_GPU_ONLY
  #include <GL/glew.h>
  #include <GL/gl.h>
#endif


namespace albedo
{

namespace tools
{

namespace shader
{

class Shader
{
#if ALBEDO_TOOLS_MODE <= ALBEDO_TBB_GPU_MODE
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
#endif
};

} // namespace shader

} // namespace tools

} // namespace albedo
