#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <iostream>

#include <data/cubemap.hpp>

namespace albedo
{

namespace tools
{

namespace data
{

class ReaderWriter
{
  public:
    static std::shared_ptr<ReaderWriter>
    instance()
    {
      if (instance_ == nullptr) instance_ = std::make_shared<ReaderWriter>();

      return instance_;
    }

  public:
    ~ReaderWriter();

  public:
    std::vector<float*>
    loadCubemap(char const* path, char const* ext,
                int& width, int& nbComponents);

    void
    save(const data::Cubemap& cubemap, const char* path, const char* ext) const;

  private:
    float*
    toFloat(const unsigned char* data, int size, int nbComp) const;

    unsigned char*
    toChar(const float* data, int size, int nbComp) const;

  private:
    static std::shared_ptr<ReaderWriter> instance_;

  private:
    std::vector<float*> images_;

};

} // namespace data

} // namespace tools

} // namespace albedo
