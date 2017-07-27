#include <processors/cpu-processor.hpp>

namespace albedo
{

namespace tools
{

namespace process
{

const glm::vec2 CPUProcessor::INV_ATAN = glm::vec2(0.1591, 0.3183);
const float CPUProcessor::TWO_PI = (2.0f * (float)M_PI);
const float CPUProcessor::PI_2 = (0.5f * (float)M_PI);

void
CPUProcessor::computePixelIrradiance(const AbstractProcessor::CubemapPtr&cubemap,
                                     uint8_t fID, int u, int v, int outSize,
                                     float step, float* toData)
{
  int halfSize = outSize / 2;
  auto normal = this->faceIDXtoVector(fID, halfSize, u, v);
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
  cubemap->getFetchCoord(fID, normal, outSize, px, py);

  int idx = (px + py * outSize) * 3;
  toData[idx] = r;
  toData[idx + 1] = g;
  toData[idx + 2] = b;
}

void
CPUProcessor::computePixelFromLatlong(const AbstractProcessor::LatlongPtr& map,
                                      uint8_t fID, int u, int v, int outSize,
                                      float* toData)
{
  int halfSize = outSize / 2;
  int nbComp = map->getNbComp();

  auto normal = faceIDXtoVector(fID, halfSize, u, v);
  normal = glm::normalize(normal);

  // TODO: Apply bilinear interpolation

  const float phi = atan2f(normal[0], normal[2]);
  const float theta = acosf(normal[1]);

  float uu = (M_PI + phi) * (0.5f / M_PI);
  float vv = theta * INV_ATAN.y;

  float r = 0.0f;
  float g = 0.0f;
  float b = 0.0f;
  map->getPixel(0, uu, vv, r, g, b);

  int px = 0;
  int py = 0;
  data::Cubemap::getFetchCoord(fID, normal, outSize, px, py);

  int idx = (px + py * outSize) * nbComp;
  toData[idx] = r;
  toData[idx + 1] = g;
  toData[idx + 2] = b;
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
