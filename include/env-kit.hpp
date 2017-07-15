#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include <string>
#include <stdexcept>

#include <math/vector.hpp>
#include <data/cubemap.hpp>

namespace albedo
{

namespace tools
{

class EnvProcessor
{
  public:
    void
    computeDiffuseIS(const data::Cubemap& cubemap, std::size_t nbSamples);

    void
    computeSpecularIS();

    void
    computeBRDFLUT();

  private:
    float* data_;
};

} // tools

} // albedo
