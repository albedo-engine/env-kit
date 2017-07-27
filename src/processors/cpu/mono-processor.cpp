#include <processors/cpu/mono-processor.hpp>

namespace albedo
{

namespace tools
{

namespace process
{

namespace cpu
{

void
MonoProcessor::init()
{ }

AbstractProcessor::CubemapPtr
MonoProcessor::computeDiffuseIS(const AbstractProcessor::CubemapPtr &cubemap,
                                uint16_t nbSamples, int writeSize)
{
  int size = writeSize;
  int nbComp = cubemap->getNbComp();

  std::vector<float*> faces;
  for (uint8_t i = 0; i < 6; ++i)
    faces.push_back(utils::createImage(size, size, nbComp));

  float step = CPUProcessor::TWO_PI / (float)nbSamples;

  for (uint8_t f = 0; f < 6; ++f)
    this->computePixelIrradiance(cubemap, f, size, step, faces[f]);

  return std::make_shared<data::Cubemap>(faces, size, nbComp);
}

void
MonoProcessor::computeSpecularIS()
{
  throw "Not implemented.";
}

void
MonoProcessor::computeBRDFLUT()
{
  throw "Not implemented.";
}

AbstractProcessor::CubemapPtr
MonoProcessor::toCubemapImpl(const AbstractProcessor::LatlongPtr& map,
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

  for (uint8_t f = 0; f < 6; ++f)
    this->computePixelFromLatlong(map, f, w, faces[f]);

  return std::make_shared<data::Cubemap>(faces, w, nbComp);
}

// Implementation of conversion to Latlong
AbstractProcessor::LatlongPtr
MonoProcessor::toLatlongImpl(const AbstractProcessor::CubemapPtr& map,
                             int w, int h)
{
  throw "Not implemented.";
}

AbstractProcessor::LatlongPtr
MonoProcessor::toLatlongImpl(const AbstractProcessor::CubecrossPtr& map,
              int w, int h)
{
  throw "Not implemented.";
}

} // namespace cpu

} // namespace process

} // namespace tools

} // namespace albedo
