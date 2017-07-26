#pragma once

#include <chrono>
#include <iostream>

#include <utils/singleton.hpp>

namespace albedo
{

namespace tools
{

namespace utils
{

class Logger : public Singleton<Logger>
{
  public:
    inline void
    start(std::string message)
    {
      std::cout << message << std::endl;
      this->start();
    }

    inline void
    start()
    {
      begin_ = std::chrono::high_resolution_clock::now();
      auto time_begin = std::chrono::high_resolution_clock::now();
    }

    inline void
    stop(std::string message)
    {
      auto end = std::chrono::high_resolution_clock::now();
      auto elapsed = end - begin_;
      auto seconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
      std::cout << message << (seconds.count() * 0.001f) << "s\n" << std::endl;
    }

  private:
    std::chrono::high_resolution_clock::time_point begin_;

};

} // namespace utils

} // namespace tools

} // namespace albedo
