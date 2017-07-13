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

  private:
    float* data_;
};

} // tools

} // albedo
