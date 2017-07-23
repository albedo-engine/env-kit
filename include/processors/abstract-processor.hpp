#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include <memory>

#include <string>
#include <stdexcept>

#include <math/vector.hpp>
#include <data/cubemap.hpp>
#include <data/equirectangular.hpp>

namespace albedo
{

namespace tools
{

namespace process
{

class AbstractProcessor
{
  public:
    virtual void
    init() = 0;

    virtual data::Cubemap
    computeDiffuseIS(const data::Cubemap& cubemap,
                     uint16_t nbSamples, int size) = 0;

    virtual void
    computeSpecularIS() = 0;

    virtual void
    computeBRDFLUT() = 0;

    virtual data::Cubemap
    toCubemap(const data::Equirectangular& map, int size) = 0;

    virtual data::Equirectangular
    toEquirectangular(const data::Cubemap& map) = 0;
};

} // process

} // tools

} // albedo
