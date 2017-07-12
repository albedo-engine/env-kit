#include <iostream>

#include <env-kit.hpp>

int main()
{
  albedo::tools::EnvProcessor processor;
  processor.load("../assets/desert.hdr");

  return 0;
}
