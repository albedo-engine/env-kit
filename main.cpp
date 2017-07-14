#include <chrono>

#include <iostream>
#include <unordered_map>

#include <env-kit.hpp>
#include <data/image.hpp>
#include <data/cubemap.hpp>

using namespace albedo::tools;

data::Cubemap
load(const char* path, const char* ext)
{
  static std::unordered_map<uint, std::string> TYPE_TABLE =
  {
    { data::CubemapFace::X, "right" },
    { data::CubemapFace::NEG_X, "left" },
    { data::CubemapFace::Y, "top" },
    { data::CubemapFace::NEG_Y, "bottom" },
    { data::CubemapFace::Z, "front" },
    { data::CubemapFace::NEG_Z, "back" }
  };

  // Contains the the single mipmaps layer
  // of the Cubemap.
  std::vector<data::Image> faces;

  stbi_set_flip_vertically_on_load(true);

  std::string file;
  for (size_t i = 0; i < 6; ++i)
  {
    file = std::string(path) + "-" + std::string(TYPE_TABLE.at(i)) + "." + ext;

    data::Image image;
    image.data = stbi_loadf(file.c_str(),
                            &image.width,
                            &image.height,
                            &image.nbComp, 0);
    if (!image.data)
    {
      std::string exception = "EnvProcessor: Fail to load " + std::string(file);
      throw std::invalid_argument(exception);
    }

    faces.push_back(image);
  }

  return data::Cubemap(faces);
}

int main(int argc, const char** argv)
{
  typedef std::chrono::high_resolution_clock Time;
  typedef std::chrono::duration<float> fsec;

  data::Cubemap envCubemap = load("../assets/small/desert", "png");
  albedo::tools::EnvProcessor processor;

  auto tStart = Time::now();
  processor.computeDiffuseIS(envCubemap, 10);
  auto tEnd = Time::now();

  fsec fs = tEnd - tStart;

  std::cout << "duration: " << fs.count() << " seconds.";

  return 0;
}
