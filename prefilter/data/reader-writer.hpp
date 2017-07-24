#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <iostream>

#include <data/cubemap.hpp>
#include <data/equirectangular.hpp>
#include <data/image.hpp>
#include <data/unicubemap.hpp>

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
    data::Equirectangular
    loadEquirect(char const* path, char const* ext);

    data::Cubemap
    loadCubemap(char const* path, char const* ext);

    void
    save(const data::Cubemap& cubemap, const char* path, const char* ext) const;

    void
    save(const data::Image2D& map, const char* path, const char* ext) const;

  private:
    float*
    loadFromExt(char const* fullPath, char const* ext,
                int& width, int& height, int& nbComponents);

    unsigned char*
    loadUnsigned(char const* path, int& width, int& height, int& nbComponents);

    float*
    loadFloat(char const* path, int& width, int& height, int& nbComponents);

    float*
    toFloat(const unsigned char* data, int size, int nbComp) const noexcept;

    unsigned char*
    toChar(const float* data, int size, int nbComp) const noexcept;

  private:
    static std::shared_ptr<ReaderWriter> instance_;

  private:
    std::vector<float*> images_;

};

} // namespace data

} // namespace tools

} // namespace albedo
