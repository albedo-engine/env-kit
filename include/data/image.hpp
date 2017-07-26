#pragma once

#include <string>

namespace albedo
{

namespace tools
{

namespace data
{

class Image
{
  public:
    Image(int width, int height, int nbComponents);

  public:
    inline int
    getNbComp() const { return nbComponents_; }

    inline int
    getWidth() const { return width_; }

    inline int
    getHeight() const { return height_; }

    virtual std::string
    getType() const = 0;

  protected:
    int width_;
    int height_;
    int nbComponents_;
};

} // data

} // tools

} // albedo
