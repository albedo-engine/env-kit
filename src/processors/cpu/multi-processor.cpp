#include <processors/cpu/multi-processor.hpp>

namespace albedo
{

namespace tools
{

namespace process
{

namespace cpu
{

void
MultiProcessor::init()
{ }

AbstractProcessor::CubemapPtr
MultiProcessor::computeDiffuseIS(const AbstractProcessor::CubemapPtr& cubemap,
                                 uint16_t nbSamples, int writeSize)
{
  int size = writeSize;
  int halfSize = size / 2;
  int nbComp = cubemap->getNbComp();

  std::vector<float*> faces;
  for (uint8_t i = 0; i < 6; ++i)
    faces.push_back(utils::createImage(size, size, nbComp));

  float step = CPUProcessor::TWO_PI / (float)nbSamples;

  tbb::parallel_for(size_t(0), size_t(6), [&](size_t f)
  {
    auto fID = (uint8_t)f;
    for (int u = -halfSize; u < halfSize; ++u)
    {
      for (int v = -halfSize; v < halfSize; ++v)
        this->computePixelIrradiance(cubemap, fID, u, v, size, step, faces[f]);
    }
  });
  return std::make_shared<data::Cubemap>(faces, size, nbComp);
}

void
MultiProcessor::computeSpecularIS()
{
  throw "Not implemented.";
}

void
MultiProcessor::computeBRDFLUT()
{
  throw "Not implemented.";
}

AbstractProcessor::CubemapPtr
MultiProcessor::toCubemapImpl(const AbstractProcessor::LatlongPtr& map,
                              int w, int h)
{
  if (w != h)
  {
    std::string error = "CPUProcessor: conversion to cubemap can not be ";
    error += "done with different dimensions.";
    throw std::invalid_argument(error);
  }

  int nbComp = map->getNbComp();
  int halfSize = w / 2;

  std::vector<float*> faces;
  for (uint8_t i = 0; i < 6; ++i)
    faces.push_back(utils::createImage(w, h, nbComp));

  tbb::parallel_for(size_t(0), size_t(6), [&](size_t f)
  {
    auto fID = (uint8_t)f;
    for (int u = -halfSize; u < halfSize; ++u)
    {
      for (int v = -halfSize; v < halfSize; ++v)
        this->computePixelFromLatlong(map, fID, u, v, w, faces[f]);
    }
  });
  return std::make_shared<data::Cubemap>(faces, w, nbComp);
}

// Implementation of conversion to Latlong
AbstractProcessor::LatlongPtr
MultiProcessor::toLatlongImpl(const AbstractProcessor::CubemapPtr& map,
                              int w, int h)
{
  throw "Not implemented.";
}

AbstractProcessor::LatlongPtr
MultiProcessor::toLatlongImpl(const AbstractProcessor::CubecrossPtr& map,
                              int w, int h)
{
  throw "Not implemented.";
}

} // namespace cpu

} // namespace process

} // namespace tools

} // namespace albedo
