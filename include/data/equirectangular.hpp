#pragma once

#include <cstdint>

#include <glm/glm.hpp>

#include <data/image.hpp>

namespace albedo
{

namespace tools
{

namespace data
{

class Equirectangular : public Image
{
  public:
    Equirectangular(float* data, int width, int height, int nbComponents);

  public:
    void
    getPixel(uint8_t mipIdx, float u, float v,
             float& r, float& g, float& b) const;
};

} // namespace data

} // namespace tools

} // namespace albedo
