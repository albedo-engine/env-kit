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
  for (auto& img : images_) stbi_image_free(img); //delete img;
}

std::vector<float*>
ReaderWriter::loadCubemap(char const* path, char const* ext,
                          int& width, int& nbComponents)
{

  stbi_set_flip_vertically_on_load(true);

  // Contains the the single mipmaps layer of the Cubemap.
  std::vector<float*> faces;

  std::string file;
  for (size_t i = 0; i < 6; ++i)
  {
    file = std::string(path)
           + "-"
           + std::string(data::Cubemap::TYPE_TO_STRING.at(i)) + "." + ext;
    float* data = stbi_loadf(file.c_str(), &width, &width, &nbComponents, 0);

    if (!data)
    {
      std::string exception = "EnvProcessor: Fail to load " + std::string(file);
      throw std::invalid_argument(exception);
    }

    faces.push_back(data);
    images_.push_back(data);
  }

  return faces;
}

void
ReaderWriter::save(const data::Cubemap &cubemap,
                   const char* path, const char* ext) const
{
  int size = cubemap.getSize();
  int nbComp = cubemap.getNbComp();

  for (const auto& mip : cubemap.getMipmaps())
  {
    for (std::size_t i = 0; i < mip.size(); ++i)
    {
      std::string file = std::string(path) + "-";
      file += data::Cubemap::TYPE_TO_STRING.at(i) + "." + ext;
      int ret = stbi_write_tga(file.c_str(), size, size, nbComp, mip[i]);
      if (!ret)
      {
        std::string error = "ReaderWriter: An error occured while saving'";
        error += path + std::string("'");
        throw std::runtime_error(error);
      }
    }
  }

}

} // namespace data

} // namespace tools

} // namespace albedo
