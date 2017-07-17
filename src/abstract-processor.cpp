#include <processors/abstract-processor.hpp>

namespace albedo
{

namespace tools
{

data::Cubemap
EnvProcessor::computeDiffuseIS(const data::Cubemap& cubemap,
                               std::uint16_t nbSamples)


}

void
EnvProcessor::computeSpecularIS()
{
  throw "Not implemented.";
}

void
EnvProcessor::computeBRDFLUT()
{
  throw "Not implemented.";
}

} // tools

} // albedo
