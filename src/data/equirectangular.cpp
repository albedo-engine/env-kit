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
                : Image({{data}}, width, height, nbComponents)
{ }

void
Equirectangular::getPixel(uint8_t mipIdx, float u, float v,
                          float &r, float &g, float &b) const
{
  int x = (int)(u * ((float)width_ + 1));
  int y = (int)(v * ((float)height_ + 1));

  x = (x >= width_) ? width_ - 1 : x;
  x = (x < 0) ? 0 : x;

  y = (y >= height_) ? height_ - 1 : y;
  y = (y < 0) ? 0 : y;

  int idx = (x + y * width_) * nbComponents_;

  const float* data = mipmaps_[mipIdx][0];
  r = data[idx];
  g = data[idx + 1];
  b = data[idx + 2];
}

} // namespace data

} // namespace tools

} // namespace albedo
