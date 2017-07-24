#include <data/equirectangular.hpp>
#include <glm/vec3.hpp>

namespace albedo
{

namespace tools
{

namespace data
{

Equirectangular::Equirectangular(float *data, int width,
                                 int height, int nbComponents)
                : Image2D({data}, width, height, nbComponents)
{ }

} // namespace data

} // namespace tools

} // namespace albedo
