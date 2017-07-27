#pragma once

#include <glm/glm.hpp>

#include <data/cubemap.hpp>
#include <math/vector.hpp>
#include <processors/abstract-processor.hpp>

#include <utils/image-utils.hpp>
#include <utils/singleton.hpp>

namespace albedo
{

namespace tools
{

namespace process
{

class CPUProcessor : public AbstractProcessor,
                     public Singleton<CPUProcessor>
{
  public:
    friend class Singleton<CPUProcessor>;

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

  public:
    inline void
    setMultithreading(bool multithread) { multithread_ = multithread; }

  protected:
    // Implementation of conversion to Cubemap
    AbstractProcessor::CubemapPtr
    toCubemapImpl(const AbstractProcessor::LatlongPtr& map,
                  int w, int h) override;

    // Implementation of conversion to Latlong
    AbstractProcessor::LatlongPtr
    toLatlongImpl(const AbstractProcessor::CubemapPtr& map,
                  int w, int h) override;

    AbstractProcessor::LatlongPtr
    toLatlongImpl(const AbstractProcessor::CubecrossPtr& map,
                  int w, int h) override;

  protected:
    void
    computePixelIrradiance(const AbstractProcessor::CubemapPtr& cubemap,
                           uint8_t fID, int u, int v, int outSize, float step,
                           float* toData);

    void
    computePixelFromLatlong(const AbstractProcessor::LatlongPtr& map,
                            uint8_t fID, int u, int v, int outSize,
                            float* toData);

  private:
    glm::vec3
    faceIDXtoVector(uint8_t faceIDX, int distToCenter, int u, int v) const;

  private:
    CPUProcessor();

  private:
    static const glm::vec2  INV_ATAN;
    static const float      TWO_PI;
    static const float      PI_2;

  private:
    bool multithread_;
};

} // process

} // tools

} // albedo
