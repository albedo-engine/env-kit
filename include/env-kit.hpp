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

class EnvProcessor
{
  public:
    static inline
    std::shared_ptr<EnvProcessor>
    instance()
    {
      if (instance_ == nullptr) instance_ = std::make_shared<EnvProcessor>();

      return instance_;
    }

  public:
    data::Cubemap
    computeDiffuseIS(const data::Cubemap& cubemap, uint16_t nbSamples);

    void
    computeSpecularIS();

    void
    computeBRDFLUT();

  private:
    static std::shared_ptr<EnvProcessor> instance_;

};

} // tools

} // albedo
