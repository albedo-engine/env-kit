#pragma once

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
};

} // namespace data

} // namespace tools

} // namespace albedo
