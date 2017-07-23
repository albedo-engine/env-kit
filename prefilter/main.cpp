#include <algorithm>
#include <cstring>
#include <chrono>
#include <iostream>

#if ALBEDO_TOOLS_MODE <= ALBEDO_TBB_GPU_MODE
  #include <GL/glew.h>
  #include <GLFW/glfw3.h>
#endif

#include <data/cubemap.hpp>
#include <processors/abstract-processor.hpp>
#include <processors/cpu-processor.hpp>
#include <processors/gpu-processor.hpp>

#include "program-parser.hpp"
#include "data/reader-writer.hpp"

#define TEST(x) std::cout << x << std::endl

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
  bool nothread = programData->getArg("onethread").boolValue;

  // Creates object in charge of loading
  // and saving textures.
  auto readerWriter = albedo::tools::data::ReaderWriter::instance();

  // Creates processor in charge of computing the texture maps.
  std::shared_ptr<process::AbstractProcessor> processor = nullptr;

#if ALBEDO_TOOLS_MODE <= ALBEDO_TBB_GPU_MODE
  // Opens a glfw window to run OpenGL in a context.
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

  if (nogpu)
    processor = process::CPUProcessor::instance();
  else
  {
    auto& gpuprocessor = process::GPUProcessor::instance();
    gpuprocessor->setWindow(window);
    processor = gpuprocessor;
  }
#else
  processor = process::CPUProcessor::instance();
#endif

  processor->init();

  // Starts timer computing time taken by
  // the irradiance map convolution.
  auto tStart = Time::now();

  if (type == "eqplanar")
  {
    auto map = readerWriter->loadEquirect(inputPath.c_str(), ext.c_str());
    auto cubemap = processor->toCubemap(map, 512);
    readerWriter->save(cubemap, outputPath.c_str(), "tga");
    auto irradianceCubemap = processor->computeDiffuseIS(cubemap,
                                                         nbSamples,
                                                         requestedWidth);
    readerWriter->save(irradianceCubemap, outputPath.c_str(), "tga");
  }
  /*else if (type == "cubemap_multifaces")
  {
    auto map = readerWriter->loadCubemap(inputPath.c_str(), ext.c_str());
    auto irradianceCubemap = processor->computeDiffuseIS(map, nbSamples, 10);
  }*/

  auto tEnd = Time::now();

  fsec fs = tEnd - tStart;
  std::cout << "duration: " << fs.count() << " seconds.";

  return 0;
}
