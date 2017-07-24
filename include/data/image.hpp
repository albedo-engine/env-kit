#pragma once

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

  protected:
    int width_;
    int height_;
    int nbComponents_;
};

} // data

} // tools

} // albedo
