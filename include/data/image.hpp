#pragma once

#include <vector>

namespace albedo
{

namespace tools
{

namespace data
{

class Cubemap;

class Image
{
  public:
    Image(std::vector<std::vector<float*>> mipmaps,
          int width, int height, int nbComponents);

  public:
    inline const std::vector<float*>&
    getMip(std::size_t i) const { return mipmaps_[i]; }

    inline const std::vector<std::vector<float*>>
    getMipmaps() const { return mipmaps_; }

    inline int
    getNbComp() const { return nbComponents_; }

    inline int
    getNbMipmaps() const { return mipmaps_.size(); }

    inline int
    getWidth() const { return width_; }

    inline int
    getHeight() const { return height_; }

  protected:
    std::vector<std::vector<float*>>  mipmaps_;
    int                               width_;
    int                               height_;
    int                               nbComponents_;
};

} // data

} // tools

} // albedo
