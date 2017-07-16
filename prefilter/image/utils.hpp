#pragma once

namespace albedo
{

namespace tools
{

namespace image
{

template <typename T>
void
verticalSymetry(T* imgData, uint16_t w, uint16_t h)
{
  for (uint16_t y = 0; y < h; ++y)
  {
    // Swaps line together
    for (uint16_t x = 0; x < w; ++x)
    {
      int beginIdx = (x + y * w) * 3;
      int endIdx = (x + (h - y - 1) * w) * 3;

      auto tmpR = imgData[beginIdx];
      auto tmpG = imgData[beginIdx + 1];
      auto tmpB = imgData[beginIdx + 2];

      imgData[beginIdx] = imgData[endIdx];
      imgData[beginIdx + 1] = imgData[endIdx + 1];
      imgData[beginIdx + 2] = imgData[endIdx + 2];

      imgData[endIdx] = tmpR;
      imgData[endIdx + 1] = tmpG;
      imgData[endIdx + 2] = tmpB;
    }
  }
}

} // namespace data

} // namespace tools

} // namespace albedo
