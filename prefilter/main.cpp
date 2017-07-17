#include <chrono>
#include <iostream>

#include <data/cubemap.hpp>
#include <processors/abstract-processor.hpp>
#include <processors/cpu-processor.hpp>
#include <processors/gpu-processor.hpp>

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

  // Parses program arguments.
  uint16_t nbSamples = (uint16_t)programData->getArg("samples").intValue;
  std::string inputPath = programData->getArg("src").strValue;
  std::string outputPath = programData->getArg("dst").strValue;
  bool nogpu = programData->getArg("nogpu").boolValue;

  // Creates object in charge of loading
  // and saving textures.
  auto readerWriter = albedo::tools::data::ReaderWriter::instance();

  // Creates processor in charge of
  // computing the texture maps.
  std::shared_ptr<process::AbstractProcessor> processor = nullptr;
  if (nogpu)
    processor = std::make_shared<process::CPUProcessor>();
  else
    processor = std::make_shared<process::GPUProcessor>();


  int width = 0;
  int nbComp = 0;
  std::vector<float*> faces = readerWriter
    ->loadCubemap(inputPath.c_str(), "tga", width, nbComp);

  albedo::tools::data::Cubemap envCubemap(faces, width, nbComp);

  typedef std::chrono::high_resolution_clock Time;
  typedef std::chrono::duration<float> fsec;

  auto tStart = Time::now();
  auto irradianceCubemap = processor->computeDiffuseIS(envCubemap, nbSamples);
  auto tEnd = Time::now();

  fsec fs = tEnd - tStart;
  std::cout << "duration: " << fs.count() << " seconds.";

  readerWriter->save(irradianceCubemap, outputPath.c_str(), "tga");

  return 0;
}
