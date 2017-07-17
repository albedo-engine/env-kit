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

class GPUProcessor : public AbstractProcessor
{

  public:
    data::Cubemap
    computeDiffuseIS(const data::Cubemap& cubemap, uint16_t nbSamples) override;

    void
    computeSpecularIS() override;

    void
    computeBRDFLUT() override;

};

} // process

} // tools

} // albedo
