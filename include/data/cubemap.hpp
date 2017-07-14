#pragma once

#include <cstddef>

#include <unordered_map>
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
  X = 0,
  NEG_X,
  Y,
  NEG_Y,
  Z,
  NEG_Z
};

class Cubemap
{
  public:
    Cubemap(std::vector<data::Image> facesData);

    ~Cubemap();

  public:
    void
    getPixel(std::size_t mipIdx, const math::Vector& direction) const;

    inline int
    getSize() const
    {
      return mipmaps_[0][0].width;
    }

  private:
    std::vector<std::vector<data::Image>> mipmaps_;

};

} // namespace data

} // namespace tools

} // namespace albedo
