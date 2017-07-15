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
  for (auto& img : images_) delete[] img;
}

std::vector<float*>
ReaderWriter::loadCubemap(char const* path, char const* ext,
                          int& width, int& nbComponents)
{
  // Contains the the single mipmaps layer of the Cubemap.
  std::vector<float*> faces;

  std::string file;
  for (size_t i = 0; i < 6; ++i)
  {
    file = std::string(path)
           + "-"
           + std::string(data::Cubemap::TYPE_TO_STRING.at(i)) + "." + ext;

    unsigned char* raw = stbi_load(file.c_str(),
                                   &width, &width, &nbComponents, STBI_rgb);

    if (!raw)
    {
      std::string exception = "EnvProcessor: Fail to load " + std::string(file);
      throw std::invalid_argument(exception);
    }

    float* data = this->toFloat(raw, width * width, nbComponents);
    faces.push_back(data);
    images_.push_back(data);

    stbi_image_free(raw);
  }

  return faces;
}

void
ReaderWriter::save(const data::Cubemap &cubemap,
                   const char* path, const char* ext) const
{
  int size = cubemap.getSize();
  int nbComp = cubemap.getNbComp();

  const auto& mips = cubemap.getMipmaps();
  auto& mip = mips[0];

  for (unsigned int i = 0; i < mip.size(); ++i)
  {
    // Creates the full location where to store the image.
    std::string file = std::string(path) + "-";
    file += data::Cubemap::TYPE_TO_STRING.at(i) + "." + ext;

    // Converts back float data to unsigned char
    unsigned char* img = this->toChar(mip[i], size * size, cubemap.getNbComp());

    int ret = stbi_write_tga(file.c_str(), size, size, nbComp, img);
    if (!ret)
    {
      std::string error = "ReaderWriter: An error occured while saving'";
      error += path + std::string("'");
      throw std::runtime_error(error);
    }
    delete[] img;
  }

}

float*
ReaderWriter::toFloat(const unsigned char *data, int size, int nbComp) const
{
  int nbValues = size * nbComp;

  float* result = new float[nbValues];
  for (std::size_t i = 0; i < nbValues; ++i)
  {
    result[i] = ((float)data[i]) / 255.0f;
  }
  return result;
}

unsigned char*
ReaderWriter::toChar(const float *data, int size, int nbComp) const
{
  int nbValues = size * nbComp;

  unsigned char* result = new unsigned char[nbValues];
  for (std::size_t i = 0; i < nbValues; ++i)
  {
    result[i] = (unsigned char)(data[i] * 255.0f);
  }
  return result;
}

} // namespace data

} // namespace tools

} // namespace albedo
