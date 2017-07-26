#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <iostream>

#include <data/cubemap.hpp>
#include <data/latlong.hpp>
#include <data/image.hpp>
#include <data/cubecross.hpp>

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
    std::shared_ptr<data::Image>
    load(char const* path, char const* ext, std::string type);

    void
    save(const data::Image& map, std::string path, std::string ext) const;

    void
    save(const data::Cubemap& cubemap,
         std::string path, std::string ext) const;

    void
    save(const data::Image2D& map,
         std::string path, std::string ext) const;

  private:
    float*
    load2D(char const* path, char const* ext,
           int& width, int& height, int& nbComp);

    std::vector<float*>
    loadCubemap(char const* path, char const* ext, int& width, int& nbComp);

    float*
    loadFloat(char const* path, int& width, int& height, int& nbComponents);

  private:
    static std::shared_ptr<ReaderWriter> instance_;

  private:
    std::vector<float*> images_;

};

} // namespace data

} // namespace tools

} // namespace albedo
