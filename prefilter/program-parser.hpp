#pragma once

#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

namespace albedo
{

namespace tools
{

class ProgramParser
{
  public:
    union Data
    {
      std::string strValue;
      int         intValue;
      bool        boolValue;

      Data() = default;
      ~Data() = default;
    };

  public:
    static std::shared_ptr<ProgramParser>
    instance()
    {
      if (instance_ == nullptr) instance_ = std::make_shared<ProgramParser>();

      return instance_;
    }

  public:
    ProgramParser();

  public:
    bool
    parseArgs(int argc, char** args);

    inline std::string
    getArg(std::string key)
    {
      if (!arguments_.count(key))
      {
        std::string error = "ProgramData: request argument '"
                            + key + "' does not exist";
        throw std::invalid_argument(error);
      }

      return arguments_[key];
    }

    inline bool
    hasFlag(std::string key)
    {
      return arguments_.count(key) > 0;
    }

    void
    extractSize(std::string key, int& width, int& height);

    bool
    helpRequested();

    void
    printHelp();

    void
    printInfo();

  private:
    void
    checkRequiredOpt(const char* optKey, const char* message);

  private:
    static std::shared_ptr<ProgramParser>                     instance_;

    static const std::unordered_map<std::string, std::string> OPT_TO_KEY;
    static const std::unordered_map<std::string, std::string> OPT_TO_DESC;
    static const std::unordered_map<std::string, bool>        NUMERICAL_OPT;
    static const std::unordered_map<std::string, bool>        FLAG_OPT;

  private:
    std::unordered_map<std::string, std::string>  arguments_;
    bool                                          noError_;
    bool                                          helpArg_;
};

} // namespace tools

} // namespace albedo
