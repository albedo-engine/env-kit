#include <iostream>
#include <unordered_map>

#include <env-kit.hpp>
#include <data/image.hpp>
#include <data/cubemap.hpp>

using namespace albedo::tools;

data::Cubemap
load(const char* path, int& width, int& height, int& nbComponents)
{
  static std::unordered_map<uint, std::string> TYPE_TABLE =
  {
    { data::CubemapFace::LEFT, "left" },
    { data::CubemapFace::RIGHT, "right" },
    { data::CubemapFace::FRONT, "front" },
    { data::CubemapFace::BACK, "back" },
    { data::CubemapFace::TOP, "top" },
    { data::CubemapFace::BOTTOM, "bottom" }
  };

  // Contains the the single mipmaps layer
  // of the Cubemap.
  std::vector<data::Image> faces;

  stbi_set_flip_vertically_on_load(true);

  std::string file;
  for (size_t i = 0; i < 6; ++i)
  {
    file = std::string(path) + "-" + std::string(TYPE_TABLE.at(i));

    data::Image image;
    image.data = stbi_loadf(file.c_str(),
                            &image.width,
                            &image.height,
                            &nbComponents, 0);
    if (!image.data)
    {
      std::string exception = "EnvProcessor: Fail to load " + std::string(path);
      throw std::invalid_argument(exception);
    }

    faces.push_back(image);
  }

  return data::Cubemap(faces);
}

int main(int argc, const char** argv)
{
  albedo::tools::EnvProcessor processor;

  return 0;
}
