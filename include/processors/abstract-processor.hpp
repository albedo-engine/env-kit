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
#include <data/cubecross.hpp>

#include <utils/image-utils.hpp>

namespace albedo
{

namespace tools
{

namespace process
{

class AbstractProcessor
{
  protected:
    typedef std::shared_ptr<data::Image>      ImagePtr;
    typedef std::shared_ptr<data::Cubemap>    CubemapPtr;
    typedef std::shared_ptr<data::Latlong>    LatlongPtr;
    typedef std::shared_ptr<data::Cubecross>  CubecrossPtr;

  public:
    virtual void
    init() = 0;

    virtual CubemapPtr
    computeDiffuseIS(const CubemapPtr& cubemap,
                     uint16_t nbSamples, int size) = 0;

    virtual void
    computeSpecularIS() = 0;

    virtual void
    computeBRDFLUT() = 0;

  public:
    std::shared_ptr<data::Image>
    to(const std::shared_ptr<data::Image> map,
       std::string type, int width, int height);

  protected:
    // Implementation of conversion to Cubemap
    virtual CubemapPtr
    toCubemapImpl(const LatlongPtr& map, int w, int h) = 0;

    // Implementation of conversion to Latlong
    virtual LatlongPtr
    toLatlongImpl(const CubemapPtr& map, int w, int h) = 0;

    virtual LatlongPtr
    toLatlongImpl(const CubecrossPtr& map, int w, int h) = 0;

  private:
    // Implementation of conversion to Cubemap
    CubemapPtr
    toCubemapImpl(const CubecrossPtr& map, int w, int h);

    // Implementation of conversion to Cubecross
    CubecrossPtr
    toCubecrossImpl(const CubemapPtr& map, int w, int h);

};

} // process

} // tools

} // albedo
