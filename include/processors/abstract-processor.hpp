#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include <memory>

#include <string>
#include <stdexcept>

#include <math/vector.hpp>
#include <data/cubemap.hpp>

namespace albedo
{

namespace tools
{

namespace process
{

class AbstractProcessor
{
  public:
    virtual
    data::Cubemap
    computeDiffuseIS(const data::Cubemap& cubemap, uint16_t nbSamples) = 0;

    virtual
    void
    computeSpecularIS() = 0;

    virtual
    void
    computeBRDFLUT() = 0;

};

} // process

} // tools

} // albedo
