#pragma once

#include <cstddef>

#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

#include <data/image.hpp>

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
    static const glm::fvec3                             FACE_UV_VEC[6][2];
    static const std::unordered_map<uint, glm::vec3>    FACE_TO_VEC;
    static const std::unordered_map<uint, std::string>  TYPE_TO_STRING;

  public:
    Cubemap(std::vector<float*> facesData, int width, int nbComponents);

  public:
    static void
    getFetchCoord(uint8_t faceIdx, const glm::vec3& direction,
                  int width, int& x, int& y);

  public:
    void
    getPixel(uint8_t mipIdx, const glm::vec3& direction,
             float& r, float& g, float& b, int& x, int& y) const;

    inline const std::vector<std::vector<float*>>&
    getMipmaps() const { return mipmaps_; }

  private:
  std::vector<std::vector<float*>> mipmaps_;

};

} // namespace data

} // namespace tools

} // namespace albedo
