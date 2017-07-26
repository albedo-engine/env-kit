#include <processors/cpu-processor.hpp>

namespace albedo
{

namespace tools
{

namespace process
{

const glm::vec2 CPUProcessor::INV_ATAN = glm::vec2(0.1591, 0.3183);

CPUProcessor::CPUProcessor()
            : multithread_{true}
{ }

void
CPUProcessor::init()
{ }

AbstractProcessor::CubemapPtr
CPUProcessor::computeDiffuseIS(const AbstractProcessor::CubemapPtr& cubemap,
                               uint16_t nbSamples, int writeSize)
{
  //int size = cubemap->getWidth();
  int size = writeSize;
  int halfSize = size / 2;
  int nbComp = cubemap->getNbComp();

  std::vector<float*> faces;
  for (uint8_t i = 0; i < 6; ++i)
    faces.push_back(utils::createImage(size, size, nbComp));

  static const float TWO_PI = (2.0f * (float)M_PI);
  static const float PI_2 = (0.5f * (float)M_PI);

  float step = TWO_PI / (float)nbSamples;

  for (uint8_t f = 0; f < 6; ++f)
  {
    for (int u = -halfSize; u < halfSize; ++u)
    {
      for (int v = -halfSize; v < halfSize; ++v)
      {
        auto normal = this->faceIDXtoVector(f, halfSize, u, v);
        normal = glm::normalize(normal);

        auto up = glm::vec3(0.0f, 1.0f, 0.0f);
        auto right = glm::cross(up, normal);
        up = glm::cross(normal, right);

        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;

        // Integrates over the the y axis
        float nrSamples = 0.0f;
        for (float phi = 0.0f; phi < TWO_PI; phi += step)
        {
          // Integrates over the the normal axis
          for (float theta = 0.0f; theta < PI_2; theta += step)
          {
            auto tangent = glm::vec3(std::sin(theta) * std::cos(phi),
                                     std::sin(theta) * std::sin(phi),
                                     std::cos(theta));
            auto fetch = (tangent.x * right) + (tangent.y * up)
                         + tangent.z * normal;

            float cR = 0.0f;
            float cG = 0.0f;
            float cB = 0.0f;
            int fetchX = 0;
            int fetchY = 0;
            cubemap->getPixel(0, fetch, cR, cG, cB, fetchX, fetchY);

            r += cR * std::cos(theta) * std::sin(theta);
            g += cG * std::cos(theta) * std::sin(theta);
            b += cB * std::cos(theta) * std::sin(theta);

            nrSamples++;
          }
        }
        r = (float)M_PI * r * (1.0f / nrSamples);
        g = (float)M_PI * g * (1.0f / nrSamples);
        b = (float)M_PI * b * (1.0f / nrSamples);

        int px = 0;
        int py = 0;
        cubemap->getFetchCoord(f, normal, size, px, py);

        int idx = (px + py * size) * 3;
        faces[f][idx] = r;
        faces[f][idx + 1] = g;
        faces[f][idx + 2] = b;
      }
    }
  }

  return std::make_shared<data::Cubemap>(faces, size, nbComp);
}

void
CPUProcessor::computeSpecularIS()
{
  throw "Not implemented.";
}

void
CPUProcessor::computeBRDFLUT()
{
  throw "Not implemented.";
}

// Implementation of conversion to Cubemap
AbstractProcessor::CubemapPtr
CPUProcessor::toCubemapImpl(const AbstractProcessor::LatlongPtr& map,
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
  {
    for (int u = -halfSize; u < halfSize; ++u)
    {
      for (int v = -halfSize; v < halfSize; ++v)
      {
        auto normal = faceIDXtoVector(f, halfSize, u, v);
        normal = glm::normalize(normal);

        // TODO: Apply bilinear interpolation

        const float phi = atan2f(normal[0], normal[2]);
        const float theta = acosf(normal[1]);

#define CMFT_RPI      0.31830988618379067153f

        float uu = (M_PI + phi) * (0.5f / M_PI);
        float vv = theta * CMFT_RPI;

        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        map->getPixel(0, uu, vv, r, g, b);

        int px = 0;
        int py = 0;
        data::Cubemap::getFetchCoord(f, normal, w, px, py);

        int idx = (px + py * w) * nbComp;
        faces[f][idx] = r;
        faces[f][idx + 1] = g;
        faces[f][idx + 2] = b;
      }
    }
  }

  return std::make_shared<data::Cubemap>(faces, w, nbComp);
}

// Implementation of conversion to Latlong
AbstractProcessor::LatlongPtr
CPUProcessor::toLatlongImpl(const AbstractProcessor::CubemapPtr& map,
                            int w, int h)
{
  throw "Not implemented.";
}

AbstractProcessor::LatlongPtr
CPUProcessor::toLatlongImpl(const AbstractProcessor::CubecrossPtr& map,
                            int w, int h)
{
  throw "Not implemented.";
}

glm::vec3
CPUProcessor::faceIDXtoVector(uint8_t faceIDX,
                              int distToCenter, int u, int v) const
{
  if (faceIDX == 0 || faceIDX == 1)
    return glm::vec3((faceIDX == 0) ? distToCenter : - distToCenter, u, v);

  if (faceIDX == 2 || faceIDX == 3)
    return glm::vec3(u, (faceIDX == 2) ? distToCenter : - distToCenter, v);

  return glm::vec3(u, v, (faceIDX == 4) ? distToCenter : - distToCenter);
}

} // process

} // tools

} // albedo
