#include "program-parser.hpp"

namespace albedo
{

namespace tools
{

std::shared_ptr<ProgramParser> ProgramParser::instance_ = nullptr;

const std::unordered_map<std::string, std::string> ProgramParser::OPT_TO_KEY
{
  { "-h", "help" },
  { "--help", "help" },
  { "-s", "src" },
  { "--src", "src" },
  { "-d", "dst" },
  { "--dst", "dst" },
  { "-n", "samples" },
  { "--nb-samples", "samples" },
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

ProgramParser::ProgramParser()
            : noError_{true}
{
  arguments_["samples"].intValue = 125;
}

bool
ProgramParser::parseArgs(int argc, char **argv)
{
  for (uint16_t i = 0; i < argc - 1; ++i)
  {
    auto optMapIt = OPT_TO_KEY.find(argv[i]);
    if (optMapIt != OPT_TO_KEY.end())
    {
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
    else if (optMapIt == OPT_TO_KEY.end() && argv[i][0] == '-')
    {
      std::cerr << "Unrecognized option: '" << argv[i] << "'" << std::endl;
      printHelp();
      return false;
    }
  }

  checkRequiredOpt("src", "-s/-src [SOURCE]");
  checkRequiredOpt("dst", "-d/-dst [DESTINATION]");

  return noError_;
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

} // namespace tools

} // namespace albedo
