#include <chrono>
#include <iostream>

#include <env-kit.hpp>
#include <data/cubemap.hpp>

#include "program-parser.hpp"
#include "data/reader-writer.hpp"

int main(int argc, char** argv)
{
  auto programData = albedo::tools::ProgramParser::instance();
  if (!programData->parseArgs(argc, argv)) return 1;

  // Parses program arguments.
  int nbSamples = programData->getArg("samples").intValue;
  std::string inputPath = programData->getArg("src").strValue;
  std::string outputPath = programData->getArg("dst").strValue;

  // Creates object in charge of loading
  // and saving textures.
  auto readerWriter = albedo::tools::data::ReaderWriter::instance();

  // Creates processor in charge of
  // computing the texture maps.
  auto processor = albedo::tools::EnvProcessor::instance();

  int width = 0;
  int nbComp = 0;
  std::vector<float*> faces = readerWriter
    ->loadCubemap(inputPath.c_str(), "tga", width, nbComp);

  albedo::tools::data::Cubemap envCubemap(faces, width, nbComp);

  typedef std::chrono::high_resolution_clock Time;
  typedef std::chrono::duration<float> fsec;

  auto tStart = Time::now();
  auto irradianceCubemap = processor->computeDiffuseIS(envCubemap, 10);
  auto tEnd = Time::now();

  fsec fs = tEnd - tStart;
  std::cout << "duration: " << fs.count() << " seconds.";

  readerWriter->save(irradianceCubemap, "toto", "tga");

  return 0;
}
