#include <data/cubemap.hpp>
#include <iostream>

namespace albedo
{

namespace tools
{

namespace data
{

const std::unordered_map<uint, std::string> Cubemap::TYPE_TO_STRING =
{
  { data::CubemapFace::X,     "right"   },
  { data::CubemapFace::NEG_X, "left"    },
  { data::CubemapFace::Y,     "front"   },
  { data::CubemapFace::NEG_Y, "back"    },
  { data::CubemapFace::Z,     "bottom"  },
  { data::CubemapFace::NEG_Z,  "top"    }
};

///
///    --> U    _____
///   |        |     |
///   v        | +Y  |
///   V   _____|_____|_____ _____
///      |     |     |     |     |
///      | -X  | +Z  | +X  | -Z  |
///      |_____|_____|_____|_____|
///            |     |
///            | -Y  |
///            |_____|
///
const float Cubemap::FACE_UV_VEC[6][3][3] =
{
  { // +x face
    {  0.0f,  0.0f, -1.0f }, // u -> -z
    {  0.0f, -1.0f,  0.0f }, // v -> -y
    {  1.0f,  0.0f,  0.0f }, // +x face
  },
  { // -x face
    {  0.0f,  0.0f,  1.0f }, // u -> +z
    {  0.0f, -1.0f,  0.0f }, // v -> -y
    { -1.0f,  0.0f,  0.0f }, // -x face
  },
  { // +y face
    {  1.0f,  0.0f,  0.0f }, // u -> +x
    {  0.0f,  0.0f,  1.0f }, // v -> +z
    {  0.0f,  1.0f,  0.0f }, // +y face
  },
  { // -y face
    {  1.0f,  0.0f,  0.0f }, // u -> +x
    {  0.0f,  0.0f, -1.0f }, // v -> -z
    {  0.0f, -1.0f,  0.0f }, // -y face
  },
  { // +z face
    {  1.0f,  0.0f,  0.0f }, // u -> +x
    {  0.0f, -1.0f,  0.0f }, // v -> -y
    {  0.0f,  0.0f,  1.0f }, // +z face
  },
  { // -z face
    { -1.0f,  0.0f,  0.0f }, // u -> -x
    {  0.0f, -1.0f,  0.0f }, // v -> -y
    {  0.0f,  0.0f, -1.0f }, // -z face
  }
};


Cubemap::Cubemap(std::vector<float*> facesData,
                 int width, int nbComponents)
        : mipmaps_({facesData})
        , width_{width}
        , nbComponents_{nbComponents}
{ }

uint
Cubemap::getFaceIndex(const math::Vector& direction)
{

}

void
Cubemap::getPixel(std::size_t mipIdx,
                  const math::Vector& direction,
                  float& r, float& g, float& b) const
{

  if (mipIdx < 0 || mipIdx >= mipmaps_.size())
  {
    throw std::invalid_argument("Cubemap: Invalid mipmap index.");
  }

  if (direction.null()) return;

  const auto& maps = mipmaps_[mipIdx];

  std::size_t max = 0;
  float maxVal = 0.0f;
  // Gets sample face
  for (std::size_t i = 0; i < 3; ++i)
  {
    float val = std::abs(direction[i]);
    if (val > maxVal)
    {
      max = i;
      maxVal = val;
    }
  }
  math::Vector normalized = direction / maxVal;

  std::size_t faceIdx = max * 2 + ((direction[max] < 0.0) ? 1 : 0);

  normalized = (normalized * 0.5f) + 0.5f;

  // TODO: Gets the two remaining components serving
  // TODO: to fetch in the target texture.

  float u = 0;
  float v = 0;
  if (max == 0)
  {
    u = normalized.y();
    v = normalized.z();
  }
  else if (max == 1)
  {
    u = normalized.x();
    v = normalized.z();
  }
  else
  {
    u = normalized.y();
    v = normalized.z();
  }

  getFacePx(
    0,
    faceIdx,
    (int)(u * (float)(this->getSize() / 2)),
    (int)(v * (float)(this->getSize() / 2)), r, g, b
  );

}

inline void
Cubemap::getFacePx(int mipLvl, int faceIdx,
                   int x, int y, float& r, float& g, float&b) const
{
  if (x < 0 || x >= width_ || y < 0 || y >= width_)
  {
    std::string error = "Image: pixel (" + x;
    error += ", " + y;
    error += ") is not in the image";
    throw std::invalid_argument(error);
  }

  const float* data = mipmaps_[mipLvl][faceIdx];

  int idx = x + y * width_;
  r = data[idx];
  g = data[idx + 1];
  b = data[idx + 2];
}

} // namespace data

} // namespace tools

} // namespace albedo
