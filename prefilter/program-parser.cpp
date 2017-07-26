#include <cstring>
#include "program-parser.hpp"

namespace albedo
{

namespace tools
{

std::shared_ptr<ProgramParser> ProgramParser::instance_ = nullptr;

const std::unordered_map<std::string, std::string> ProgramParser::OPT_TO_KEY
{
  {
    "-s", "src"
  },
  {
    "--src", "src"
  },
  {
    "-d", "dst"
  },
  {
    "--dst", "dst"
  },
  {
    "-e", "ext"
  },
  {
    "--ext", "ext"
  },
  {
    "-n", "samples"
  },
  {
    "--nb-samples", "samples"
  },
  {
    "--input-type", "inputtype"
  },
  {
    "--output-type", "outputtype"
  },
  {
    "--no-gpu", "nogpu"
  },
  {
    "--one-thread", "onethread"
  },
  {
    "--irradiance", "irradiance"
  },
  {
    "--specular", "specular"
  },
  {
    "--convert", "convert"
  }
};

const std::unordered_map<std::string, std::string> ProgramParser::OPT_TO_DESC
{
  {
    "-s",
    "-s, --src <path>{defines the path of the input texture"
  },
  {
    "-d",
    "-d, --dst <path>{defines the path of the outputs texture"
  },
  {
    "",
    "--ext <IMAGE_EXTENSION>{"
    "precise the extension of image that is given in input.\nSupported: "
    "TGA - HDR"
  },
  { "",
    "--input-type <IMAGE_TYPE>{"
      "defines the type of image that is given in input.\nSupported: "
      "LATLONG, CUBEMAP, CUBECROSS"
  },
  {
    "-ot",
    "--output-type <IMAGE_TYPE>{defines the type of image use for output."
    "\nSupported: CUBEMAP, CUBECROSS"
  },
  {
    "-n",
    "-n, --nb-samples <N>{defines the number of samples to compute "
      "the irradiance map. Default: 125"
  },
  {
    "",
    "--irradiance <INTxINT>{computes irradiance map saved as dst-irradiance.ext"
  },
  {
    "",
    "--specular <INTxINT>{computes specular map saved as dst-specular.ext"
  },
  {
    "",
    "--convert <INTxINT>{saves the map convert to output-type as dst-converted.ext"
  },
};

const std::unordered_map<std::string, bool> ProgramParser::NUMERICAL_OPT
{
  { "samples", true }
};

const std::unordered_map<std::string, bool> ProgramParser::FLAG_OPT
{
  { "nogpu", false },
  { "onethread", false }
};

ProgramParser::ProgramParser()
              : noError_{true}
              , helpArg_{false}
{
  arguments_["samples"] = "125";
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
    if (FLAG_OPT.count(optMapIt->second) != 0)
    {
      arguments_[optMapIt->second] = "";
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
    if (NUMERICAL_OPT.count(optMapIt->second) != 0)
    {
      if ((nextArg.find_first_not_of( "0123456789" ) != std::string::npos))
      {
        std::cerr << "Invalid option: '" << argv[i] << "'"
                  << " should contain a positive integer value."
                  << std::endl;
        return false;
      }
      arguments_[optMapIt->second] = nextArg;
      continue;
    }
    arguments_[optMapIt->second] = nextArg;
  }

  checkRequiredOpt("src", "-s/--src [SOURCE]");
  checkRequiredOpt("dst", "-d/--dst [DESTINATION]");
  checkRequiredOpt("inputtype", "--input-type [IMAGE_TYPE]");
  checkRequiredOpt("outputtype", "--output-type [IMAGE_TYPE]");
  checkRequiredOpt("ext", "--ext [IMAGE_EXTENSION]");

  return noError_;
}

void
ProgramParser::extractSize(std::string key, int& width, int& height)
{
  if (arguments_.count(key) == 0)
  {
    std::string error = "ProgramParser: extractSize: '" + key;
    error = "' is not an argument";
    throw std::invalid_argument(error);
  }

  auto outputSizeStr = arguments_[key];
  auto splitIdx = outputSizeStr.find('x');
  if (splitIdx == std::string::npos)
  {
    std::string error = "ProgramParser: extractSize: invalid value: '";
    error += outputSizeStr + "' should be of the type UINTxUINT.";
    throw std::invalid_argument(error);
  }

  width = std::stoi(outputSizeStr.substr(0, splitIdx));
  height = std::stoi(outputSizeStr.substr(splitIdx + 1));
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

  std::cout << "The commands listed below describes how to use the binary:\n"
            << std::endl;

  // Prints help for every possible option
  for (const auto& e : OPT_TO_KEY)
  {
    if (OPT_TO_DESC.count(e.first) == 0) continue;

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
ProgramParser::printInfo()
{
  const auto& src = arguments_["src"];
  const auto& format = arguments_["type"];
  const auto& nb = arguments_["samples"];

  auto intermSize = arguments_.count("convert") != 0 ?
                    arguments_["convert"] : "512x512";

#if ALBEDO_TOOLS_MODE <= ALBEDO_TBB_GPU_MODE
  bool onGPU = (arguments_.count("nogpu") == 0);
#else
  bool onGPU = false;
#endif
  bool multithread = (arguments_.count("onethread") == 0);

  std::cout << "Starting environment processing on "
            << (onGPU ? "GPU" : "CPU")
            << "... ";
  if (!onGPU)
  {
    std::cout << "("
              << (multithread ? "multi-threading" : "single-threading")
              << ")"
              << std::endl;
  }
  else
    std::cout << std::endl;

  std::cout << "-input file: " << src << "\n"
            << "-image format: " << format << "\n"
            << "-number of samples: " << nb << "\n"
            << "-intermediary cubemap size: " << intermSize << "\n";

  if (arguments_.count("irradiance") != 0)
    std::cout << "-irradiance size: " << arguments_["irradiance"];
  if (arguments_.count("specular") != 0)
    std::cout << "-specular size: " << arguments_["irradiance"];

  std::cout << std::endl;
}

void
ProgramParser::checkRequiredOpt(const char* optKey, const char* message)
{
  if (arguments_.count(optKey) == 0)
  {
    std::cerr << "Missing option: " << message << std::endl;
    noError_ = false;
  }
}

} // namespace tools

} // namespace albedo
