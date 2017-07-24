#pragma once

#include <cstdint>

#include <glm/glm.hpp>

#include <data/2D-image.hpp>

namespace albedo
{

namespace tools
{

namespace data
{

class Equirectangular : public Image2D
{
  public:
    Equirectangular(float* data, int width, int height, int nbComponents);
};

} // namespace data

} // namespace tools

} // namespace albedo
