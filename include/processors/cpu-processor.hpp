#pragma once

#include <data/cubemap.hpp>
#include <math/vector.hpp>
#include <processors/abstract-processor.hpp>

namespace albedo
{

namespace tools
{

namespace process
{

class CPUProcessor : public AbstractProcessor
{

  public:
    data::Cubemap
    computeDiffuseIS(const data::Cubemap& cubemap, uint16_t nbSamples) override;

    data::Cubemap
    computeDiffuseIS(const data::Equirectangular& map,
                     uint16_t nbSamples) override;

    void
    computeSpecularIS() override;

    void
    computeBRDFLUT() override;

    data::Cubemap
    toCubemap(const data::Equirectangular& map) override;

    data::Equirectangular
    toEquirectangular(const data::Cubemap& map) override;

};

} // process

} // tools

} // albedo
