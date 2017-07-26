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

#if ALBEDO_TOOLS_MODE <= ALBEDO_TBB_GPU_MODE
#include <processors/gpu-processor.hpp>
#endif

#include <utils/logger.hpp>

#include "program-parser.hpp"
#include "data/reader-writer.hpp"

using namespace albedo::tools;

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

  // Prints on stdout information about
  // the processing that is going to occur
  programData->printInfo();

  // Parses program arguments.
  auto inputPath = programData->getArg("src");
  auto outputPath = programData->getArg("dst");

  auto ext = programData->getArg("ext");
  std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

  auto inputType = programData->getArg("inputtype");
  std::transform(inputType.begin(), inputType.end(),
                 inputType.begin(), ::tolower);

  auto outputType = programData->getArg("outputtype");
  std::transform(inputType.begin(), inputType.end(),
                 inputType.begin(), ::tolower);

  bool nogpu = programData->hasFlag("nogpu");
  bool nothread = programData->hasFlag("onethread");
  bool computeIrradiance = programData->hasFlag("irradiance");
  bool computeSpecular = programData->hasFlag("specular");
  bool convert = programData->hasFlag("convert");

  // Creates object in charge of loading and saving textures.
  auto readerWriter = albedo::tools::data::ReaderWriter::instance();

  // Creates processor in charge of computing the texture maps.
  std::shared_ptr<process::AbstractProcessor> processor = nullptr;
#if ALBEDO_TOOLS_MODE <= ALBEDO_TBB_GPU_MODE
  if (!nogpu)
  {
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

    auto& gpuprocessor = process::GPUProcessor::instance();
    gpuprocessor->setWindow(window);
    processor = gpuprocessor;
  }
  else
    processor = process::CPUProcessor::instance();
#else
  auto& cpuprocessor = process::CPUProcessor::instance();
  cpuprocessor->setMultithreading(!nothread);
  processor = cpuprocessor;
#endif

  processor->init();

  // Loads src image.
  utils::Logger::instance()->start("Image loading...");
  auto entryMap = readerWriter->load(inputPath.c_str(), ext.c_str(), inputType);
  utils::Logger::instance()->stop("Image successfully loaded in ");

  if (computeIrradiance)
  {
    int outWidth = 0;
    int outHeight = 0;
    programData->extractSize("irradiance", outWidth, outHeight);
    uint16_t nbSamples = std::atoi(programData->getArg("samples").c_str());

    utils::Logger::instance()->start("Converting input map to Cubemap...");
    auto cubemapImage = processor->to(entryMap, "cubemap", 512, 512);
    auto cubemap = std::static_pointer_cast<data::Cubemap>(cubemapImage);
    utils::Logger::instance()->stop("Input map sucessfully converted in ");

    utils::Logger::instance()->start("Computing irradiance Cubemap...");
    auto irradianceCubemap = processor->computeDiffuseIS(cubemap,
                                                         nbSamples,
                                                         outWidth);
    utils::Logger::instance()->stop("Irradiance Cubemap created in ");

    utils::Logger::instance()->start("Irradiance map to output type...");
    auto mapTosave = processor->to(irradianceCubemap, outputType,
                                   outWidth, outHeight);
    utils::Logger::instance()->stop("Map successfully converted in ");

    utils::Logger::instance()->start("Saving Irradiance map to output type...");
    readerWriter->save(*mapTosave, outputPath + "-irradiance", "tga");
    utils::Logger::instance()->stop("Irradiance Map successfully saved in ");
  }

  if (convert)
  {
    int outWidth = 0;
    int outHeight = 0;
    programData->extractSize("convert", outWidth, outHeight);

    utils::Logger::instance()->start("Converting input map...");
    auto out = processor->to(entryMap, outputType, outWidth, outHeight);
    utils::Logger::instance()->stop("Map successfully converted in ");

    utils::Logger::instance()->start("Saving input map to output type...");
    readerWriter->save(*out, outputPath + "-converted", "tga");
    utils::Logger::instance()->stop("Input Map successfully saved in ");
  }

  return 0;
}
