#pragma once

#include <cstddef>

#include <unordered_map>
#include <vector>

#include <data/image.hpp>
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

class Cubemap : public Image
{
  public:
    static const math::Vector                           FACE_UV_VEC[6][2];
    static const std::unordered_map<uint, math::Vector> FACE_TO_VEC;
    static const std::unordered_map<uint, std::string>  TYPE_TO_STRING;

  public:
    Cubemap(std::vector<float*> facesData, int width, int nbComponents);

  public:
    void
    getPixel(uint8_t mipIdx, const math::Vector& direction,
             float& r, float& g, float& b, int& x, int& y) const;

    void
    getFetchCoord(uint8_t faceIdx,
                  const math::Vector& direction, int& x, int& y) const;

    inline int
    getSize() const { return width_; }

};

} // namespace data

} // namespace tools

} // namespace albedo
