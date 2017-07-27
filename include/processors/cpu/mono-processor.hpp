#include <processors/cpu-processor.hpp>

namespace albedo
{

namespace tools
{

namespace process
{

namespace cpu
{

class MonoProcessor : public CPUProcessor
                    , public Singleton<MonoProcessor>
{
  public:
    friend class Singleton<MonoProcessor>;

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
    MonoProcessor() = default;
};

} // namespace cpu

} // namespace process

} // namespace tools

} // namespace albedo
