#include <processors/gpu-processor.hpp>

namespace albedo
{

namespace tools
{

namespace process
{

data::Cubemap
GPUProcessor::computeDiffuseIS(const data::Cubemap& cubemap, uint16_t nbSamples)
{
  throw "Not implemented.";
}

void
GPUProcessor::computeSpecularIS()
{
  throw "Not implemented.";
}

void
GPUProcessor::computeBRDFLUT()
{
  throw "Not implemented.";
}

} // process

} // tools

} // albedo
