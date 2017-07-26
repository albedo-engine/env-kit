#include <data/cubecross.hpp>

namespace albedo
{

namespace tools
{

namespace data
{

Cubecross::Cubecross(float* data, int width, int height, int nbComponents)
          : Image2D(data, width, height, nbComponents)
{ }

std::string
Cubecross::getType() const
{
  return "cubecross";
}

} // namespace data

} // namespace tools

} // namespace albedo
