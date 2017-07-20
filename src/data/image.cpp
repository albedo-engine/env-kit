#include <data/image.hpp>

namespace albedo
{

namespace tools
{

namespace data
{

Image::Image(std::vector<std::vector<float*>> mipmaps,
             int width, int height, int nbComponents)
      : mipmaps_{mipmaps}
      , width_{width}
      , height_{height}
      , nbComponents_{nbComponents}
{ }

} // data

} // tools

} // albedo
