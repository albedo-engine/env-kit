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
  { data::CubemapFace::X,     "right"  },
  { data::CubemapFace::NEG_X, "left"   },
  { data::CubemapFace::Y,     "top"    },
  { data::CubemapFace::NEG_Y, "bottom" },
  { data::CubemapFace::Z,     "front"  },
  { data::CubemapFace::NEG_Z, "back"   }
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

const glm::vec3 Cubemap::FACE_UV_VEC[6][2] =
{
  { // +x face
    glm::vec3({  0.0f,  0.0f, -1.0f }), // u -> -z
    glm::vec3({  0.0f, -1.0f,  0.0f }), // v -> -y
  },
  { // -x face
    glm::vec3({  0.0f,  0.0f,  1.0f }), // u -> +z
    glm::vec3({  0.0f, -1.0f,  0.0f }), // v -> -y
  },
  { // +y face
    glm::vec3({  1.0f,  0.0f,  0.0f }), // u -> +x
    glm::vec3({  0.0f,  0.0f,  1.0f }), // v -> +z
  },
  { // -y face
    glm::vec3({  1.0f,  0.0f,  0.0f }), // u -> +x
    glm::vec3({  0.0f,  0.0f, -1.0f }), // v -> -z
  },
  { // +z face
    glm::vec3({  1.0f,  0.0f,  0.0f }), // u -> +x
    glm::vec3({  0.0f, -1.0f,  0.0f }), // v -> -y
  },
  { // -z face
    glm::vec3({ -1.0f,  0.0f,  0.0f }), // u -> -x
    glm::vec3({  0.0f, -1.0f,  0.0f }) // v -> -y
  }
};

Cubemap::Cubemap(std::vector<float*> facesData,
                 int width, int nbComponents)
        : Image(width, width, nbComponents)
        , mipmaps_{{facesData}}
{ }

void
Cubemap::getFetchCoord(uint8_t faceIdx, const glm::vec3& dir,
                       int width, int& x, int& y)
{
  const float absVec[3] =
    {
      fabsf(dir[0]),
      fabsf(dir[1]),
      fabsf(dir[2]),
    };
  const float max = fmaxf(fmaxf(absVec[0], absVec[1]), absVec[2]);
  glm::fvec3 normalized = dir / max;

  float u = glm::dot(FACE_UV_VEC[faceIdx][0], normalized);
  float v = glm::dot(FACE_UV_VEC[faceIdx][1], normalized);
  u = (u + 1.0f) * 0.5f;
  v = (v + 1.0f) * 0.5f;

  x = (int)(u * ((float)width + 1));
  y = (int)(v * ((float)width + 1));

  x = (x >= width) ? width - 1 : x;
  x = (x < 0) ? 0 : x;

  y = (y >= width) ? width - 1 : y;
  y = (y < 0) ? 0 : y;
}

void
Cubemap::getPixel(uint8_t mipIdx, const glm::vec3& dir,
                  float& r, float& g, float& b, int& x, int& y) const
{
  if (mipIdx < 0 || mipIdx >= mipmaps_.size())
  {
    throw std::invalid_argument("Cubemap: Invalid mipmap index.");
  }

  if (dir == glm::vec3(0.0f, 0.0f, 0.0f)) return;

  const float absVec[3] =
  {
    fabsf(dir[0]),
    fabsf(dir[1]),
    fabsf(dir[2]),
  };
  const float max = fmaxf(fmaxf(absVec[0], absVec[1]), absVec[2]);

  uint8_t faceIdx = 0;
  if (max == absVec[0])
    faceIdx = (dir[0] >= 0.0f) ? CubemapFace::X : CubemapFace::NEG_X;
  else if (max == absVec[1])
    faceIdx = (dir[1] >= 0.0f) ? CubemapFace::Y : CubemapFace::NEG_Y;
  else if (max == absVec[2])
    faceIdx = (dir[2] >= 0.0f) ? CubemapFace::Z : CubemapFace::NEG_Z;

  auto normalized = dir / max;
  float u = glm::dot(FACE_UV_VEC[faceIdx][0], normalized);
  float v = glm::dot(FACE_UV_VEC[faceIdx][1], normalized);

  u = (u + 1.0f) * 0.5f;
  v = (v + 1.0f) * 0.5f;

  x = (int)(u * ((float)width_ + 1));
  y = (int)(v * ((float)width_ + 1));

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
