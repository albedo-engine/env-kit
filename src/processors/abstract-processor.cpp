#include <processors/abstract-processor.hpp>

namespace albedo
{

namespace tools
{

namespace process
{

data::Cubemap
AbstractProcessor::toCubemap(const data::Image& map, int size)
{
  if (map.getType() == "latlong")
    return toCubemapImpl(dynamic_cast<const data::Latlong&>(map), size);

  throw "Not implemented.";
}

data::Latlong
AbstractProcessor::toEquirectangular(const data::Image& map)
{
  if (map.getType() == "cubemap")
    return toEquirectangularImpl(dynamic_cast<const data::Cubemap&>(map));

  throw "Not implemented.";
}

///       ________________________
///      |      |     |           |
///      |      | TOP |           |
///      |______|_____|_____ _____|
///      |      |     |     |     |
///      | LFT  | FRT | RGT | BCK |
///      |_____ |_____|_____|_____|
///      |      |     |           |
///      |      | BTM |           |
///      |______|_____|___________|
///
data::UniCubemap
AbstractProcessor::toUniCubemap(const data::Cubemap &map)
{
  int faceSize = map.getWidth();
  int height = 3 * faceSize;
  int width = 4 * faceSize;
  int nbComp = map.getNbComp();

  const auto& mipmaps = map.getMipmaps();

  // TODO: Handles all the mip levels
  float* data = utils::createImage(width, height, nbComp);

  const static uint8_t VERTICAL_IDX[3] =
  {
    data::CubemapFace::Y,     // Top
    data::CubemapFace::Z,     // Front
    data::CubemapFace::NEG_Y  // Back
  };
  const static uint8_t HORIZONTAL_IDX[2] =
  {
    data::CubemapFace::X,     // Top
    data::CubemapFace::NEG_Z, // Front
  };

  // Vertical copy: top, front, and bottom faces.
  for (auto i = 0; i < 3; ++i)
  {
    int mipIdx = VERTICAL_IDX[i];
    utils::copyData(std::make_pair(0, 0), std::make_pair(faceSize, faceSize * i),
                    faceSize, faceSize, width, nbComp, mipmaps[0][mipIdx], data);
  }
  // Horizontal copy: right, up faces.
  for (auto i = 0; i < 2; ++i)
  {
    int mipIdx = HORIZONTAL_IDX[i];
    int dstX = 2 * faceSize + i * faceSize;
    utils::copyData(std::make_pair(0, 0), std::make_pair(dstX, faceSize),
                    faceSize, faceSize, width, nbComp, mipmaps[0][mipIdx], data);
  }
  // Horizontal copy: left faces.
  const float* leftFaceData = mipmaps[0][data::CubemapFace::NEG_X];
  utils::copyData(std::make_pair(0, 0), std::make_pair(0, faceSize),
                  faceSize, faceSize, width, nbComp, leftFaceData, data);


  return data::UniCubemap(data, width, height, nbComp);
}

} // process

} // tools

} // albedo
