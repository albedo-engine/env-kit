#include <tbb/parallel_for.h>

#include <processors/cpu-processor.hpp>

namespace albedo
{

namespace tools
{

namespace process
{

namespace cpu
{

class MultiProcessor : public CPUProcessor
                     , public Singleton<MultiProcessor>
{
  public:
    friend class Singleton<MultiProcessor>;

  public:
    void
    init() override;

    AbstractProcessor::CubemapPtr
    computeDiffuseIS(const AbstractProcessor::CubemapPtr& cubemap,
                     uint16_t nbSamples, int writeSize) override;

    void
    computeSpecularIS() override;

    void
    computeBRDFLUT() override;

  protected:
    // Implementation of conversion to Cubemap
    AbstractProcessor::CubemapPtr
    toCubemapImpl(const AbstractProcessor::LatlongPtr& map, int w, int h);

    // Implementation of conversion to Latlong
    AbstractProcessor::LatlongPtr
    toLatlongImpl(const AbstractProcessor::CubemapPtr& map, int w, int h);

    AbstractProcessor::LatlongPtr
    toLatlongImpl(const AbstractProcessor::CubecrossPtr& map, int w, int h);

  private:
    MultiProcessor() = default;
};

} // namespace cpu

} // namespace process

} // namespace tools

} // namespace albedo
