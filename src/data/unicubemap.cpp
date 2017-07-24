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

} // namespace data

} // namespace tools

} // namespace albedo
