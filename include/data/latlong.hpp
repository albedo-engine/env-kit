#pragma once

#include <cstdint>

#include <glm/glm.hpp>

#include <data/2D-image.hpp>

namespace albedo
{

namespace tools
{

namespace data
{

class Latlong : public Image2D
{
  public:
    Latlong(float* data, int width, int height, int nbComponents);

  public:
    virtual std::string
    getType() const;
};

} // namespace data

} // namespace tools

} // namespace albedo
