#include <data/unicubemap.hpp>

namespace albedo
{

namespace tools
{

namespace data
{

UniCubemap::UniCubemap(float* data, int width, int height, int nbComponents)
          : Image2D(data, width, height, nbComponents)
{ }

std::string
UniCubemap::getType() const
{
  return "cubecross";
}

} // namespace data

} // namespace tools

} // namespace albedo
