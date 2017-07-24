#pragma once

namespace albedo
{

namespace tools
{

namespace utils
{

inline float*
createImage(int w, int h, int nbComponents)
{
  int size = w * h * nbComponents;
  auto* data = new float[size];

  for (auto i = 0; i < size; ++i) data[i] = 0.0f;

  return data;
}

inline std::size_t
getIdx(int x, int y, int width, int nbComp)
{
  return (x + y * width) * nbComp;
}

inline void
copyData(std::pair<std::size_t, std::size_t> srcStart,
         std::pair<std::size_t, std::size_t> dstStart,
         int fromWidth, int fromHeight, int toWidth,
         int nbComp, const float* src, float* dst)
{
  auto srcX = srcStart.first;
  auto srcY = srcStart.second;

  auto dstX = dstStart.first;
  auto dstY = dstStart.second;

  for (auto y = 0; y < fromHeight; ++y)
  {
    auto startIdx = utils::getIdx(srcX, srcY + y, fromWidth, nbComp);
    auto toIdx = utils::getIdx(dstX, dstY + y, toWidth, nbComp);

    const float* toCopy = src + startIdx;
    std::copy(toCopy, toCopy + fromWidth * nbComp, dst + toIdx);
  }
}

} // namespace utils

} // namespace tools

} // namespace albedo
