#include <chrono>
#include <iostream>

#include <env-kit.hpp>
#include <data/cubemap.hpp>

#include "data/reader-writer.hpp"

int main(int argc, const char** argv)
{
  typedef std::chrono::high_resolution_clock Time;
  typedef std::chrono::duration<float> fsec;

  albedo::tools::EnvProcessor processor;
  auto readerWriter = albedo::tools::data::ReaderWriter::instance();

  int width = 0;
  int nbComp = 0;
  std::vector<float*> faces = readerWriter
    ->loadCubemap("../assets/small/desert", "tga", width, nbComp);

  albedo::tools::data::Cubemap envCubemap(faces, width, nbComp);

  auto tStart = Time::now();
  processor.computeDiffuseIS(envCubemap, 10);
  auto tEnd = Time::now();

  fsec fs = tEnd - tStart;
  std::cout << "duration: " << fs.count() << " seconds.";

  readerWriter->save(envCubemap, "toto", "tga");

  return 0;
}
