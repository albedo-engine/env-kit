#include <processors/cpu-processor.hpp>

namespace albedo
{

namespace tools
{

namespace process
{

const glm::vec2 CPUProcessor::INV_ATAN = glm::vec2(0.1591, 0.3183);

void
CPUProcessor::init()
{ }

data::Cubemap
CPUProcessor::computeDiffuseIS(const data::Cubemap& cubemap,
                               uint16_t nbSamples, int writeSize)
{
  int size = cubemap.getWidth();
  int halfSize = size / 2;
  int nbComp = cubemap.getNbComp();

  std::vector<float*> faces;
  for (uint8_t i = 0; i < 6; ++i)
    faces.push_back(utils::createImage(size, size, nbComp));

  static const float TWO_PI = (2.0f * M_PI);
  static const float PI_2 = (0.5f * M_PI);

  float step = TWO_PI / (float)nbSamples;

  for (uint8_t f = 0; f < 6; ++f)
  {
    for (int u = -halfSize; u < halfSize; ++u)
    {
      for (int v = -halfSize; v < halfSize; ++v)
      {
        auto normal = this->faceIDXtoVector(f, halfSize, u, v);
        normal = glm::normalize(normal);

        auto up = glm::vec3(0.0f, 1.0f, 0.0f);
        auto right = glm::cross(up, normal);
        up = glm::cross(up, right);

        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;

        // Integrates over the the y axis
        float nrSamples = 0.0f;
        for (float phi = 0.0f; phi < TWO_PI; phi += step)
        {
          // Integrates over the the normal axis
          for (float theta = 0.0f; theta < PI_2; theta += step)
          {
            auto fetch = right * (float)(sin(theta) * cos(phi))
                         + up * (float)(sin(theta) * sin(phi))
                         + normal * (float)cos(theta);

            float cR = 0.0f;
            float cG = 0.0f;
            float cB = 0.0f;
            int fetchX = 0;
            int fetchY = 0;
            cubemap.getPixel(0, fetch, cR, cG, cB, fetchX, fetchY);

            r += cR * cos(theta) * sin(theta);
            g += cG * cos(theta) * sin(theta);
            b += cB * cos(theta) * sin(theta);

            nrSamples++;
          }
        }
        r = M_PI * r * (1.0f / nrSamples);
        g = M_PI * g * (1.0f / nrSamples);
        b = M_PI * b * (1.0f / nrSamples);

        int px = 0;
        int py = 0;
        cubemap.getFetchCoord(f, normal, size, px, py);

        int idx = (px + py * size) * 3;
        faces[f][idx] = r;
        faces[f][idx + 1] = g;
        faces[f][idx + 2] = b;
      }
    }
  }

  return data::Cubemap(faces, size, nbComp);
}

void
CPUProcessor::computeSpecularIS()
{
  throw "Not implemented.";
}

void
CPUProcessor::computeBRDFLUT()
{
  throw "Not implemented.";
}

data::Cubemap
CPUProcessor::toCubemap(const data::Equirectangular& map, int size)
{
  int nbComp = map.getNbComp();
  int halfSize = size / 2;

  std::vector<float*> faces;
  for (uint8_t i = 0; i < 6; ++i)
    faces.push_back(utils::createImage(size, size, nbComp));

  for (uint8_t f = 0; f < 6; ++f)
  {
    for (int u = -halfSize; u < halfSize; ++u)
    {
      for (int v = -halfSize; v < halfSize; ++v)
      {
        auto normal = faceIDXtoVector(f, halfSize, u, v);
        normal = glm::normalize(normal);

        // TODO: Apply bilinear interpolation

        glm::vec2 uv = glm::vec2(glm::atan(normal.z, normal.x), asin(normal.y));
        uv *= INV_ATAN;
        uv += 0.5;

        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        map.getPixel(0, uv[0], uv[1], r, g, b);

        int px = 0;
        int py = 0;
        data::Cubemap::getFetchCoord(f, normal, size, px, py);

        int idx = (px + py * size) * nbComp;
        faces[f][idx] = r;
        faces[f][idx + 1] = g;
        faces[f][idx + 2] = b;

      }
    }
  }

  return data::Cubemap(faces, size, nbComp);
}

data::Equirectangular
CPUProcessor::toEquirectangular(const data::Cubemap& map)
{
  throw "Not implemented yet.";
}

glm::vec3
CPUProcessor::faceIDXtoVector(uint8_t faceIDX, int distToCenter, int u, int v)
{
  if (faceIDX == 0 || faceIDX == 1)
    return glm::vec3((faceIDX == 0) ? distToCenter : - distToCenter, u, v);

  if (faceIDX == 2 || faceIDX == 3)
    return glm::vec3(u, (faceIDX == 2) ? distToCenter : - distToCenter, v);

  return glm::vec3(u, v, (faceIDX == 4) ? distToCenter : - distToCenter);
}

} // process

} // tools

} // albedo
