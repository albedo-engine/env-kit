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

      Data() { }
      ~Data() { }
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

    inline Data&
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

    bool
    helpRequested();

    void
    printHelp();

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
    std::unordered_map<std::string, Data> arguments_;
    bool                                  noError_;
    bool                                  helpArg_;
};

} // namespace tools

} // namespace albedo
