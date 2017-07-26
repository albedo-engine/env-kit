#include <data/latlong.hpp>
#include <glm/vec3.hpp>

namespace albedo
{

namespace tools
{

namespace data
{

Latlong::Latlong(float *data, int width, int height, int nbComponents)
                : Image2D({data}, width, height, nbComponents)
{ }

std::string
Latlong::getType() const
{
  return "latlong";
}

} // namespace data

} // namespace tools

} // namespace albedo
