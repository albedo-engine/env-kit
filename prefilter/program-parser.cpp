#include <cstring>
#include "program-parser.hpp"

namespace albedo
{

namespace tools
{

std::shared_ptr<ProgramParser> ProgramParser::instance_ = nullptr;

const std::unordered_map<std::string, std::string> ProgramParser::OPT_TO_KEY
{
  { "-s", "src" },
  { "--src", "src" },
  { "-d", "dst" },
  { "--dst", "dst" },
  { "-n", "samples" },
  { "--nb-samples", "samples" },
  { "--no-gpu", "nogpu" }
};

const std::unordered_map<std::string, std::string> ProgramParser::OPT_TO_DESC
{
  { "-s", "-s, --src <path>{precise the path of the input texture" },
  { "-d", "-d, --dst <path>{precise the path of the outputs texture" },
  { "-n", "-n, --nb-samples <N>{precise the number of samples to compute "
          "the irradiance map. Default: 125" }
};

const std::unordered_map<std::string, bool> ProgramParser::NUMERICAL_OPT
{
  { "samples", true }
};

const std::unordered_map<std::string, bool> ProgramParser::FLAG_OPT
{
  { "nogpu", false }
};

ProgramParser::ProgramParser()
              : noError_{true}
              , helpArg_{false}
{
  arguments_["samples"].intValue = 125;
  arguments_["nogpu"].boolValue = false;
}

bool
ProgramParser::parseArgs(int argc, char **argv)
{
  noError_ = true;
  helpArg_ = false;

  for (uint16_t i = 0; i < argc; ++i)
  {
    if (std::strcmp("-h", argv[i]) == 0 || std::strcmp("--help", argv[i]) == 0)
    {
      helpArg_ = true;
      return true;
    }

    auto optMapIt = OPT_TO_KEY.find(argv[i]);
    if (optMapIt == OPT_TO_KEY.end())
      continue;

    // Checks if i-nth argument is a flag option
    if (FLAG_OPT.count(optMapIt->second))
    {
      arguments_[optMapIt->second].boolValue = true;
      continue;
    }

    // Parser reached the last argument and it is
    // not a flag option.
    if (i == argc - 1)
    {
      std::cerr << "Invalid option: '" << argv[i] << "'"
                << " should be given a value."
                << std::endl;
      return false;
    }

    std::string nextArg = std::string(argv[i + 1]);
    if (NUMERICAL_OPT.count(optMapIt->second))
    {
      if ((nextArg.find_first_not_of( "0123456789" ) != std::string::npos))
      {
        std::cerr << "Invalid option: '" << argv[i] << "'"
                  << " should contain a positive integer value."
                  << std::endl;
        return false;
      }
      arguments_[optMapIt->second].intValue = atoi(nextArg.c_str());
      continue;
    }
    arguments_[optMapIt->second].strValue = nextArg;
  }

  checkRequiredOpt("src", "-s/-src [SOURCE]");
  checkRequiredOpt("dst", "-d/-dst [DESTINATION]");

  return noError_;
}

bool
ProgramParser::helpRequested()
{
  return helpArg_;
}


void
ProgramParser::printHelp()
{
  std::cout << "usage: prefilter "
            << "[-s/--src <path>]"
            << "[-d/--dst <path>]"
            << "\n" << std::endl;

  std::cout << "The command listed below describes how to use the binary:\n"
            << std::endl;

  // Prints help for every possible option
  for (const auto& e : OPT_TO_KEY)
  {
    if (!OPT_TO_DESC.count(e.first)) continue;

    const auto& optStr = OPT_TO_DESC.at(e.first);
    auto cmdStr = optStr.substr(0, optStr.find('{'));
    auto descStr = optStr.substr(optStr.find('{') + 1);

    std::cout << std::setw(2)
              << cmdStr << " "
              << std::setw(15)
              << descStr
              << std::endl;
  }
}

void
ProgramParser::checkRequiredOpt(const char* optKey, const char* message)
{
  if (!arguments_.count(optKey))
  {
    std::cerr << "Missing option: " << message << std::endl;
    noError_ = false;
  }
}

} // namespace tools

} // namespace albedo
