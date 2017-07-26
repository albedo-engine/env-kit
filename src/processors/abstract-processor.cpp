#include <processors/abstract-processor.hpp>

namespace albedo
{

namespace tools
{

namespace process
{

std::shared_ptr<data::Image>
AbstractProcessor::to(const std::shared_ptr<data::Image> map,
                      std::string toType, int w, int h)
{
  std::string fromType = map->getType();
  if (toType == fromType) return map;

  std::string error = "Processor: missing convertion implementation ";
  error += "'" + fromType + "' ";

  if (toType == "cubemap")
  {
    if (fromType == "latlong")
    {
      const auto& mapCast = std::static_pointer_cast<data::Latlong>(map);
      return this->toCubemapImpl(mapCast, w, h);
    }
    error += "to 'cubemap'";
    throw std::invalid_argument(error);
  }

  if (toType == "latlong")
  {
    if (fromType == "cubemap")
    {
      const auto& mapCast = std::static_pointer_cast<data::Cubemap>(map);
      return this->toLatlongImpl(mapCast, w, h);
    }
    error += "to 'latlong'";
    throw std::invalid_argument(error);
  }

  if (toType == "cubecross")
  {
    if (fromType == "cubemap")
    {
      const auto& mapCast = std::static_pointer_cast<data::Cubemap>(map);
      return this->toCubecrossImpl(mapCast, w, h);
    }
    if (fromType == "latlong")
    {
      const auto& mapCast = std::static_pointer_cast<data::Latlong>(map);
      auto cubemap = this->toCubemapImpl(mapCast, w, h);
      return this->toCubecrossImpl(cubemap, w, h);
    }
    error += "to 'cubecross'";
    throw std::invalid_argument(error);
  }

  throw std::invalid_argument("Processor: type '" + toType + "' not supported");
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
AbstractProcessor::CubecrossPtr
AbstractProcessor::toCubecrossImpl(const AbstractProcessor::CubemapPtr &map,
                                   int w, int h)
{
  int faceSize = map->getWidth();
  int height = 3 * faceSize;
  int width = 4 * faceSize;
  int nbComp = map->getNbComp();

  const auto& mipmaps = map->getMipmaps();

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


  return std::make_shared<data::Cubecross>(data, width, height, nbComp);
}

AbstractProcessor::CubemapPtr
AbstractProcessor::toCubemapImpl(const AbstractProcessor::CubecrossPtr& map,
                                 int w, int h)
{
  throw "Not implemented.";
}

} // process

} // tools

} // albedo
