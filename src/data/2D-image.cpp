#include <data/2D-image.hpp>

namespace albedo
{

namespace tools
{

namespace data
{

Image2D::Image2D(float *data, int width, int height, int nbComponents)
        : Image(width, height, nbComponents)
        , mipmaps_{data}
{ }

Image2D::Image2D(int width, int height, int nbComponents)
        : Image(width, height, nbComponents)
{ }

void
Image2D::getPixel(uint8_t mipLvl, float u, float v,
                  float& r, float& g, float& b) const
{
  if (mipLvl < 0 || mipLvl >= mipmaps_.size())
    throw std::invalid_argument("Image2D: invalid mip level");

  if (u < 0.0f || u > 1.0f || v < 0.0f || v > 1.0f)
  {
    std::string error = "Image2D: invalid normalized pixel coordinate (";
    error += std::to_string(u) + std::string(", ") + std::to_string(v) + ")";
    throw std::invalid_argument(error);
  }

  auto x = (int)(u * ((float)width_ + 1));
  auto y = (int)(v * ((float)height_ + 1));

  x = (x >= width_) ? width_ - 1 : x;
  x = (x < 0) ? 0 : x;

  y = (y >= height_) ? height_ - 1 : y;
  y = (y < 0) ? 0 : y;

  getPx(mipLvl, x, y, r, g, b);
}

void
Image2D::getPixel(uint8_t mipLvl, int x, int y,
                  float& r, float& g, float& b) const
{
  if (mipLvl < 0 || mipLvl >= mipmaps_.size())
    throw std::invalid_argument("Image2D: invalid mip level");

  if (x < 0 || x >= width_ || y < 0 || y >= height_)
  {
    std::string error = "Image2D: invalid pixel coordinate (" + x;
    error += ", " + std::to_string(y) + ")";
    throw std::invalid_argument(error);
  }

  getPx(mipLvl, x, y, r, g, b);
}

void
Image2D::getPx(uint8_t mipLvl, int x, int y,
               float& r, float& g, float& b) const
{
  const float* data = mipmaps_[mipLvl];
  int idx = (x + y * width_) * nbComponents_;

  r = data[idx];
  g = data[idx + 1];
  b = data[idx + 2];
}

} // data

} // tools

} // albedo
