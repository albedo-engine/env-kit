#include "reader-writer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace albedo
{

namespace tools
{

namespace data
{

std::shared_ptr<ReaderWriter> ReaderWriter::instance_ = nullptr;

ReaderWriter::~ReaderWriter()
{
  //for (auto& img : images_) delete[] img;
}

std::shared_ptr<data::Image>
ReaderWriter::load(char const *path, char const *ext, std::string type)
{
  int width = 0;
  int height = 0;
  int nbComp = 0;

  if (type == "latlong")
  {
    float *data = this->load2D(path, ext, width, height, nbComp);
    return std::make_shared<data::Latlong>(data, width, height, nbComp);
  }
  if (type == "cubecross")
  {
    float *data = this->load2D(path, ext, width, height, nbComp);
    return std::make_shared<data::Cubecross>(data, width, height, nbComp);
  }
  if (type == "cubemap")
  {
    std::vector<float*> data = this->loadCubemap(path, ext, width, nbComp);
    return std::make_shared<data::Cubemap>(data, width, nbComp);
  }

  std::string error = "ReaderWriter: no input type '" + type + "' found";
  throw std::invalid_argument(error);
}

void ReaderWriter::save(const data::Image& map,
                        std::string path, std::string ext) const
{
  std::string type = map.getType();
  if (type == "latlong" || type == "cubecross")
  {
    const auto& image2D = static_cast<const data::Image2D&>(map);
    this->save(image2D, path, ext);
    return;
  }
  if (type == "cubemap")
  {
    const auto& cubemap = static_cast<const data::Cubemap&>(map);
    this->save(cubemap, path, ext);
    return;
  }

  std::string error = "ReaderWriter: no output type '" + type + "' found";
  throw std::invalid_argument(error);
}

void
ReaderWriter::save(const data::Cubemap &cubemap,
                   std::string path, std::string ext) const
{
  int size = cubemap.getWidth();
  int nbComp = cubemap.getNbComp();

  const auto& mips = cubemap.getMipmaps();
  auto& mip = mips[0];

  for (unsigned int i = 0; i < mip.size(); ++i)
  {
    // Creates the full location where to store the image.
    std::string file = std::string(path) + "-";
    //file += data::Cubemap::TYPE_TO_STRING.at(i) + "." + ext;
    file += data::Cubemap::TYPE_TO_STRING.at(i) + ".hdr";

    // Converts back float data to unsigned char
    //unsigned char* img = this->toChar(mip[i], size * size, cubemap.getNbComp
      //());
    float* img = mip[i];

    //int ret = stbi_write_tga(file.c_str(), size, size, nbComp, img);
    int ret = stbi_write_hdr(file.c_str(), size, size, nbComp, img);
    if (!ret)
    {
      std::string error = "ReaderWriter: an error occured while saving'";
      error += path + std::string("'");
      throw std::runtime_error(error);
    }
    //delete[] img;
  }

}

void
ReaderWriter::save(const data::Image2D& map,
                   std::string path, std::string ext) const
{
  std::string file = std::string(path) + ".hdr";
  const float* data = map.getMipmaps()[0];
  int ret = stbi_write_hdr(file.c_str(), map.getWidth(),
                           map.getHeight(), map.getNbComp(), data);
  if (!ret)
  {
    std::string error = "ReaderWriter: an error occured while saving'";
    error += path + std::string("'");
    throw std::runtime_error(error);
  }
}

float*
ReaderWriter::load2D(char const *path, char const *ext,
                     int& width, int& height, int& nbComp)
{
  std::string file = path + std::string(".") + ext;
  return this->loadFloat(file.c_str(), width, height, nbComp);
}

std::vector<float*>
ReaderWriter::loadCubemap(char const* path, char const* ext,
                          int& width, int& nbComp)
{
  int height = 0;
  int nbComponents = 0;

  std::vector<float*> faces;
  std::string file;
  for (size_t i = 0; i < 6; ++i)
  {
    file = std::string(path) + "-"
           + std::string(data::Cubemap::TYPE_TO_STRING.at(i)) + "." + ext;

    float* data = this->loadFloat(file.c_str(), width, height, nbComponents);
    if (width != height)
    {
      std::string error = "ReaderWriter: invalid cubemap face '" + file + "'. ";
      error += "Faces should be squared";
      throw std::runtime_error(error);
    }

    faces.push_back(data);
    images_.push_back(data);
  }
  return faces;
}

float*
ReaderWriter::loadFloat(char const* path, int& width,
                        int& height, int& nbComponents)
{
  auto* raw = stbi_loadf(path, &width, &height, &nbComponents, 0);
  if (!raw)
  {
    std::string exception = "EnvProcessor: fail to load " + std::string(path);
    throw std::invalid_argument(exception);
  }

  return raw;
}

} // namespace data

} // namespace tools

} // namespace albedo
