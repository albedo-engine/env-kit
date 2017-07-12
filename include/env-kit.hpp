#pragma once

#include <string>
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <third-party/stb_image.h>

namespace albedo
{

namespace tools
{

class EnvProcessor
{
  public:
    void
    computeDiffuseIS();

    void
    computeSpecularIS();

    void
    computeBRDFLUT();

  public:
    void
    load(std::string pathToFile);

  private:
    float* data_;
};

} // tools

} // albedo
