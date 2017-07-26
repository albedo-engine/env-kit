#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include <data/image.hpp>

namespace albedo
{

namespace tools
{

namespace data
{

class Image2D : public Image
{
  public:
    Image2D(float *data, int width, int height, int nbComponents);

    Image2D(int width, int height, int nbComponents);

  public:
    void
    getPixel(uint8_t mipLvl, float u, float v,
             float& r, float& g, float& b) const;
    void
    getPixel(uint8_t mipLvl, int x, int y,
             float& r, float& g, float& b) const;

    inline const std::vector<float*>&
    getMipmaps() const { return mipmaps_; }

  protected:
    virtual std::string
    getType() const = 0;

  private:
    void
    getPx(uint8_t mipLvl, int x, int y,
          float& r, float& g, float& b) const;

  protected:
    std::vector<float*> mipmaps_;
};

} // namespace data

} // namespace tools

} // namespace albedo
