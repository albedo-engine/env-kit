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
  { data::CubemapFace::Y,     "top"   },
  { data::CubemapFace::NEG_Y, "bottom"    },
  { data::CubemapFace::Z,     "front"  },
  { data::CubemapFace::NEG_Z,  "back"    }
};

const std::unordered_map<uint, math::Vector> Cubemap::FACE_TO_VEC =
{
  { 0, math::Vector({1.0f, 0.0f, 0.0f})   },
  { 1, math::Vector({- 1.0f, 0.0f, 0.0f}) },
  { 2, math::Vector({0.0f, 1.0f, 0.0f})   },
  { 3, math::Vector({0.0f, - 1.0f, 0.0f}) },
  { 4, math::Vector({0.0f, 0.0f, 1.0f})   },
  { 5, math::Vector({0.0f, 0.0f, - 1.0f}) },
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
Cubemap::getPixel(uint8_t mipIdx,
                  const math::Vector& dir, float& r, float& g, float& b) const
{

  if (mipIdx < 0 || mipIdx >= mipmaps_.size())
  {
    throw std::invalid_argument("Cubemap: Invalid mipmap index.");
  }

  if (dir.null()) return;

  const float absVec[3] =
  {
    fabsf(dir[0]),
    fabsf(dir[1]),
    fabsf(dir[2]),
  };
  const float max = fmaxf(fmaxf(absVec[0], absVec[1]), absVec[2]);

  /*uint8_t faceIdx = 0;
  if (max == absVec[0])
  {
    faceIdx = (dir[0] >= 0.0f) ? CubemapFace::X : CubemapFace::NEG_X;
  }
  else if (max == absVec[1])
  {
    faceIdx = (dir[1] >= 0.0f) ? CubemapFace::Y : CubemapFace::NEG_Y;
  }
  else if (max == absVec[2])
  {
    faceIdx = (dir[2] >= 0.0f) ? CubemapFace::Z : CubemapFace::NEG_Z;
  }*/

  uint8_t faceIdx = 0;
  math::Vector normalized = dir / max;
  float u = 0.0f;
  float v = 0.0f;
  // +x face
  if (max == absVec[0] && dir[0] >= 0.0f)
  {
    u = normalized[1]; // u -> +y
    v = - normalized[2]; // v -> -z
    faceIdx = 0;
  }
  // -x face
  else if (max == absVec[0] && dir[0] < 0.0f)
  {
    u = - normalized[1]; // u -> -y
    v = - normalized[2]; // v -> -z
    faceIdx = 1;
  }
  // +y face
  else if (max == absVec[1] && dir[1] >= 0.0f)
  {
    u = - normalized[0]; // u -> -x
    v = - normalized[2]; // v -> -z
    faceIdx = 2;
  }
  // -y face
  else if (max == absVec[1] && dir[1] < 0.0f)
  {
    u = normalized[0]; // u -> +x
    v = - normalized[2]; // v -> -z
    faceIdx = 3;
  }
  // +z face
  else if (max == absVec[2] && dir[2] >= 0.0f)
  {
    u = normalized[0]; // u -> +x
    v = - normalized[1]; // v -> -y
    faceIdx = 4;
  }
  // +z face
  else if (max == absVec[2] && dir[2] < 0.0f)
  {
    u = normalized[0]; // u -> +x
    v = normalized[1]; // v -> +y
    faceIdx = 5;
  }

  u = (u + 1.0f) * 0.5f;
  v = (v + 1.0f) * 0.5f;

  getFacePx(mipIdx, faceIdx, u, v, r, g, b);

}

inline void
Cubemap::getFacePx(uint8_t mipIdx, int faceIdx,
                   float u, float v, float& r, float& g, float&b) const
{

  int x = (int)(u * (float)width_);
  int y = (int)(v * (float)width_);

  x = (x >= width_) ? width_ - 1 : x;
  x = (x < 0) ? 0 : x;

  y = (y >= width_) ? width_ - 1 : y;
  y = (y < 0) ? 0 : y;

  const float* data = mipmaps_[mipIdx][faceIdx];

  int idx = (x + y * width_) * 3;
  r = data[idx];
  g = data[idx + 1];
  b = data[idx + 2];
}

} // namespace data

} // namespace tools

} // namespace albedo
