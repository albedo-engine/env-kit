#pragma once

#include <cstddef>

#include <unordered_map>
#include <vector>

#include <math/vector.hpp>

namespace albedo
{

namespace tools
{

namespace data
{

enum CubemapFace
{
  X = 0,
  NEG_X,
  Y,
  NEG_Y,
  Z,
  NEG_Z
};

class Cubemap
{
  public:
    static std::unordered_map<uint, std::string> TYPE_TO_STRING;

  public:
    Cubemap(std::vector<float*> facesData, int width, int nbComponents);

  public:
    void
    getPixel(std::size_t mipIdx,
             const math::Vector& direction,
             float& r, float& g, float& b) const;

    inline int
    getSize() const { return width_; }

    inline int
    getNbComp() const { return nbComponents_; }

    inline int
    getNbMipmaps() const { return mipmaps_.size(); }

    inline std::vector<std::vector<float*>>
    getMipmaps() const { return mipmaps_; }

    inline std::vector<float*>&
    getMip(std::size_t i) { return mipmaps_[i]; }

  private:
    void
    getFacePx(int mipLvl, int faceIdx,
              int x, int y, float& r, float& g, float& b) const;

  private:
    std::vector<std::vector<float*>>  mipmaps_;
    int                               width_;
    int                               nbComponents_;

};

} // namespace data

} // namespace tools

} // namespace albedo
