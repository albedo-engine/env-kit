#pragma once

#include <cstddef>

#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

#include <data/2D-image.hpp>
#include <data/cubemap.hpp>

namespace albedo
{

namespace tools
{

namespace data
{

class UniCubemap : public Image2D
{
  public:
    UniCubemap(float* data, int width, int height, int nbComponents);
};

} // namespace data

} // namespace tools

} // namespace albedo
