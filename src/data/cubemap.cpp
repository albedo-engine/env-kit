#include <data/cubemap.hpp>

namespace albedo
{

namespace tools
{

namespace data
{

Cubemap::Cubemap(std::vector<data::Image> facesData)
        : mipmaps_({facesData})
{ }


void
Cubemap::getPixel(std::size_t mipIdx, const math::Vector &direction)
{
  throw "Not implemented.";
}

} // namespace data

} // namespace tools

} // namespace albedo
