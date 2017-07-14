#include <data/cubemap.hpp>
#include <iostream>

namespace albedo
{

namespace tools
{

namespace data
{

Cubemap::Cubemap(std::vector<data::Image> facesData)
        : mipmaps_({facesData})
{ }

Cubemap::~Cubemap()
{ }

void
Cubemap::getPixel(std::size_t mipIdx, const math::Vector &direction) const
{

  if (mipIdx < 0 || mipIdx >= mipmaps_.size())
  {
    throw std::invalid_argument("Cubemap: Invalid mipmap index.");
  }

  const auto& maps = mipmaps_[mipIdx];

  std::size_t max = 0;
  float maxVal = 0.0f;
  // Gets sample face
  for (std::size_t i = 0; i < 3; ++i)
  {
    if (direction[i] > maxVal)
    {
      max = i;
      maxVal = std::abs(direction[i]);
    }
  }
  math::Vector normalized = direction / maxVal;

  std::size_t faceIdx = max * 2 + ((direction[max] < 0.0) ? 1 : 0);

  normalized = (normalized * 0.5f) + 0.5f;

  // TODO: Gets the two remaining components serving
  // TODO: to fetch in the target texture.

  auto l = 0.5;

}

} // namespace data

} // namespace tools

} // namespace albedo
