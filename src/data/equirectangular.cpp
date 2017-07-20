#include <data/equirectangular.hpp>

namespace albedo
{

namespace tools
{

namespace data
{

Equirectangular::Equirectangular(float *data, int width,
                                 int height, int nbComponents)
                : Image({{data}}, width, height, nbComponents)
{ }

} // namespace data

} // namespace tools

} // namespace albedo
