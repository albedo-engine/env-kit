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

class CPUProcessor : public AbstractProcessor
{
  public:
    virtual
    void
    init() = 0;

    virtual
    AbstractProcessor::CubemapPtr
    computeDiffuseIS(const AbstractProcessor::CubemapPtr& cubemap,
                     uint16_t nbSamples, int writeSize) = 0;

    virtual
    void
    computeSpecularIS() = 0;

    virtual
    void
    computeBRDFLUT() = 0;

  protected:
    // Implementation of conversion to Cubemap
    virtual AbstractProcessor::CubemapPtr
    toCubemapImpl(const AbstractProcessor::LatlongPtr& map, int w, int h) = 0;

    // Implementation of conversion to Latlong
    virtual AbstractProcessor::LatlongPtr
    toLatlongImpl(const AbstractProcessor::CubemapPtr& map, int w, int h) = 0;

    virtual AbstractProcessor::LatlongPtr
    toLatlongImpl(const AbstractProcessor::CubecrossPtr& map, int w, int h) = 0;

  protected:
    glm::vec3
    faceIDXtoVector(uint8_t faceIDX, int distToCenter, int u, int v) const;

    void
    computePixelIrradiance(const AbstractProcessor::CubemapPtr& cubemap,
                           uint8_t fID, int outSize, float step, float* toData);

    void
    computePixelFromLatlong(const AbstractProcessor::LatlongPtr& map,
                            uint8_t fID, int outSize, float* toData);

  protected:
    static const glm::vec2  INV_ATAN;
    static const float      TWO_PI;
    static const float      PI_2;
};

} // process

} // tools

} // albedo
