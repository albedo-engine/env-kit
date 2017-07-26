#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstring>

#include <functional>
#include <memory>
#include <unordered_map>

#include <string>
#include <stdexcept>

#include <data/cubemap.hpp>
#include <data/latlong.hpp>
#include <data/unicubemap.hpp>

#include <utils/image-utils.hpp>

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

  public:
    data::Cubemap
    toCubemap(const data::Image& map, int size);

    data::Latlong
    toEquirectangular(const data::Image& map);

    data::UniCubemap
    toUniCubemap(const data::Cubemap& map);

  protected:
    virtual data::Cubemap
    toCubemapImpl(const data::Latlong& map, int size) = 0;

    virtual data::Latlong
    toEquirectangularImpl(const data::Cubemap& map) = 0;
};

} // process

} // tools

} // albedo
