#include <chrono>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <data/cubemap.hpp>
#include <processors/abstract-processor.hpp>
#include <processors/cpu-processor.hpp>
#include <processors/gpu-processor.hpp>
#include <algorithm>
#include <cstring>

#include "program-parser.hpp"
#include "data/reader-writer.hpp"

#define TBB_GPU_MODE  0
#define GPU_ONLY_MODE 1
#define TBB_ONLY_MODE 2

using namespace albedo::tools;

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::duration<float> fsec;

int main(int argc, char** argv)
{
  auto programData = albedo::tools::ProgramParser::instance();

  if (!programData->parseArgs(argc, argv))
    return 1;

  if (programData->helpRequested())
  {
    programData->printHelp();
    return 0;
  }

#if D_ALBEDO_TOOLS_MODE <= GPU_ONLY_MODE
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(600, 400, "Test", NULL, NULL);
  glfwMakeContextCurrent(window);
  if (window == NULL)
  {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return 1;
  }
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    std::cerr << "Failed to initialize GLEW" << std::endl;
    return 1;
  }
#endif

  // Parses program arguments.
  uint16_t nbSamples = (uint16_t)programData->getArg("samples").intValue;
  auto inputPath = programData->getArg("src").strValue;
  auto outputPath = programData->getArg("dst").strValue;

  auto ext = programData->getArg("ext").strValue;
  std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
  auto type = programData->getArg("type").strValue;
  std::transform(type.begin(), type.end(), type.begin(), ::tolower);

  auto outputSizeStr = programData->getArg("outsize").strValue;
  auto splitIdx = outputSizeStr.find('x');
  if (splitIdx == std::string::npos)
  {
    std::cerr << "Invalid value: '" << outputSizeStr << "'"
              << " should be of the type INTxINT."
              << std::endl;
    return 1;
  }

  int requestedWidth = std::stoi(outputSizeStr.substr(0, splitIdx));
  int requestedHeight = std::stoi(outputSizeStr.substr(splitIdx + 1));

  bool nogpu = programData->getArg("nogpu").boolValue;

  // Creates object in charge of loading
  // and saving textures.
  auto readerWriter = albedo::tools::data::ReaderWriter::instance();

  // Creates processor in charge of
  // computing the texture maps.
#if D_ALBEDO_TOOLS_MODE <= GPU_ONLY_MODE
  std::shared_ptr<process::AbstractProcessor> processor = nullptr;
  if (nogpu)
    processor = std::make_shared<process::CPUProcessor>();
  else
    processor = std::make_shared<process::GPUProcessor>(window);
#else
  processor = std::make_shared<process::CPUProcessor>();
#endif

  // Starts timer computing time taken by
  // the irradiance map convolution.
  auto tStart = Time::now();

  if (type == "eqplanar")
  {
    auto map = readerWriter->loadEquirect(inputPath.c_str(), ext.c_str());
    auto cubemap = processor->toCubemap(map, 512);
    auto irradianceCubemap = processor->computeDiffuseIS(cubemap,
                                                         nbSamples,
                                                         requestedWidth);
    readerWriter->save(irradianceCubemap, outputPath.c_str(), "tga");
  }
  else if (type == "cubemap_multifaces")
  {
    auto map = readerWriter->loadCubemap(inputPath.c_str(), ext.c_str());
    auto irradianceCubemap = processor->computeDiffuseIS(map, nbSamples, 10);
  }

  auto tEnd = Time::now();

  fsec fs = tEnd - tStart;
  std::cout << "duration: " << fs.count() << " seconds.";

  return 0;
}
