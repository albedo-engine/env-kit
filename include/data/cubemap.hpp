#pragma once

#include <cstddef>

#include <vector>

#include <math/vector.hpp>
#include <data/image.hpp>

namespace albedo
{

namespace tools
{

namespace data
{

enum CubemapFace
{
  LEFT = 0,
  RIGHT,
  FRONT,
  BACK,
  TOP,
  BOTTOM
};

class Cubemap
{
  public:
    Cubemap(std::vector<data::Image> facesData);

  public:
    void
    getPixel(std::size_t mipIdx, const math::Vector& direction);

  private:
    std::vector<std::vector<data::Image>> mipmaps_;

};

} // namespace data

} // namespace tools

} // namespace albedo
