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

data::Cubemap
ReaderWriter::loadCubemap(char const* path, char const* ext)
{
  int width = 0;
  int height = 0;
  int nbComponents = 0;

  std::vector<float*> faces;
  std::string file;
  for (size_t i = 0; i < 6; ++i)
  {
    file = std::string(path)
           + "-"
           + std::string(data::Cubemap::TYPE_TO_STRING.at(i)) + "." + ext;

    float* data = this->loadFromExt(file.c_str(), ext,
                                    width, height, nbComponents);

    faces.push_back(data);
    images_.push_back(data);
  }
  return data::Cubemap(faces, width, nbComponents);
}

data::Equirectangular
ReaderWriter::loadEquirect(char const *path, char const *ext)
{
  int width = 0;
  int height = 0;
  int nbComponents = 0;
  std::string file = path + std::string(".") + ext;
  float* data = this->loadFromExt(file.c_str(), ext,
                                  width, height, nbComponents);

  return data::Equirectangular(data, width, height, nbComponents);
}

void
ReaderWriter::save(const data::Cubemap &cubemap,
                   const char* path, const char* ext) const
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
                   const char* path, const char* ext) const
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
ReaderWriter::loadFromExt(char const* fullPath, char const* ext,
                          int& width, int& height, int& nbComponents)
{
  float* data = nullptr;
  if (strcmp(ext, "hdr") != 0)
  {
    unsigned char* raw = this->loadUnsigned(fullPath, width,
                                            height, nbComponents);
    data = this->toFloat(raw, width * width, nbComponents);
    // Free previously loaded image that
    // is now stored in the data pointer as float.
    stbi_image_free(raw);
  }
  else
    data = this->loadFloat(fullPath, width, height, nbComponents);

  return data;
}

unsigned char*
ReaderWriter::loadUnsigned(char const* path, int& width,
                           int& height, int& nbComponents)
{
  auto* raw = stbi_load(path, &width, &height, &nbComponents, STBI_rgb);
  if (!raw)
  {
    std::string exception = "EnvProcessor: fail to load " + std::string(path);
    throw std::invalid_argument(exception);
  }

  return raw;
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

float*
ReaderWriter::toFloat(const unsigned char *data,
                      int size, int nbComp) const noexcept
{
  int nbValues = size * nbComp;

  auto* result = new float[nbValues];
  for (std::size_t i = 0; i < nbValues; ++i)
  {
    result[i] = ((float)data[i]) / 255.0f;
  }
  return result;
}

unsigned char*
ReaderWriter::toChar(const float *data,
                     int size, int nbComp) const noexcept
{
  int nbValues = size * nbComp;

  auto* result = new unsigned char[nbValues];
  for (std::size_t i = 0; i < nbValues; ++i)
  {
    result[i] = (unsigned char)(data[i] * 255.0f);
  }
  return result;
}

} // namespace data

} // namespace tools

} // namespace albedo
