#include <env-kit.hpp>

namespace albedo
{

namespace tools
{

void
EnvProcessor::computeDiffuseIS()
{
  throw "Not implemented.";
}

void
EnvProcessor::computeSpecularIS()
{
  throw "Not implemented.";
}

void
EnvProcessor::computeBRDFLUT()
{
  throw "Not implemented.";
}

void
EnvProcessor::load(std::string pathToFile)
{
  stbi_set_flip_vertically_on_load(true);
  int width = 0;
  int height = 0;
  int nbComp = 0;

  float* data = stbi_loadf(pathToFile.c_str(), &width, &height, &nbComp, 0);
  if (!data)
  {
    throw std::invalid_argument("EnvProcessor: Fail to load " + pathToFile);
  }

}

} // tools

} // albedo
