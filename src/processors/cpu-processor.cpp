#include <processors/cpu-processor.hpp>

namespace albedo
{

namespace tools
{

namespace process
{

data::Cubemap
CPUProcessor::computeDiffuseIS(const data::Cubemap& cubemap, uint16_t nbSamples)
{
  int size = cubemap.getSize();
  int halfSize = size / 2;
  int nbComp = cubemap.getNbComp();

  std::vector<float*> faces;
  for (uint8_t i = 0; i < 6; ++i)
  {
    faces.push_back(new float[size * size * nbComp]);
  }

  static const float TWO_PI = (2.0f * M_PI);
  static const float PI_2 = (0.5f * M_PI);

  float step = TWO_PI / (float)nbSamples;

  for (uint8_t f = 0; f < 6; ++f)
  {
    for (int u = -halfSize; u < halfSize; ++u)
    {
      for (int v = -halfSize; v < halfSize; ++v)
      {
        math::Vector normal;
        if (f == 0 || f == 1)
          normal = math::Vector((f == 0) ? halfSize : - halfSize, u, v);
        else if (f == 2 || f == 3)
          normal = math::Vector(u, (f == 2) ? halfSize : - halfSize, v);
        else if (f == 4 || f == 5)
          normal = math::Vector(u, v, (f == 4) ? halfSize : - halfSize);
        normal.normalize();

        math::Vector up(0.0f, 1.0f, 0.0f);
        math::Vector right = up ^ normal;
        up = normal ^ right;

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
            math::Vector fetch = right * (std::sin(theta) * std::cos(phi))
                                 + up * (std::sin(theta) * std::sin(phi))
                                 + normal * std::cos(theta);

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
        cubemap.getFetchCoord(f, normal, px, py);

        int idx = (px + py * size) * 3;
        faces[f][idx] = r;
        faces[f][idx + 1] = g;
        faces[f][idx + 2] = b;
      }
    }
  }

  return data::Cubemap(faces, size, nbComp);
}

data::Cubemap
CPUProcessor::computeDiffuseIS(const data::Equirectangular &map,
                                             uint16_t nbSamples)
{
  throw "Not implemented.";
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
CPUProcessor::toCubemap(const data::Equirectangular& map)
{
  throw "Not implemented yet.";
}

data::Equirectangular
CPUProcessor::toEquirectangular(const data::Cubemap& map)
{
  throw "Not implemented yet.";
}

} // process

} // tools

} // albedo
