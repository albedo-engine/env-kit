#include <env-kit.hpp>

namespace albedo
{

namespace tools
{

data::Cubemap
EnvProcessor::computeDiffuseIS(const data::Cubemap& cubemap,
                               std::size_t nbSamples)
{
  int size = cubemap.getSize();
  int halfSize = size / 2;
  int nbComp = cubemap.getNbComp();

  std::vector<float*> faces;
  for (uint8_t i = 0; i < 6; ++i)
  {
    faces.push_back(new float[size * size * nbComp]);
  }

  float step = 0.5f;

  math::Vector normal;

  float r = 0.0f;
  float g = 0.0f;
  float b = 0.0f;

  for (int x = -halfSize; x < halfSize; ++x)
  {
    for (int y = -halfSize; y < halfSize; ++y)
    {
      for (int z = -halfSize; z < halfSize; ++z)
      {
        r = 0.0f;
        g = 0.0f;
        b = 0.0f;

        math::Vector up(0.0f, 1.0f, 0.0f);

        normal.set(x, y, z);
        normal.normalize();

        math::Vector right = up ^ normal;
        up = normal ^ right;

        // Integrates over the the y axis
        float nrSamples = 0.0f;
        for (float phi = 0.0f; phi < 2.0f * M_PI; phi += step)
        {
          // Integrates over the the normal axis
          for (float theta = 0.0f; theta < 0.5f * M_PI; theta += step)
          {
            math::Vector fetch = right * (std::sin(theta) * std::cos(phi)) +
                                 up * (std::sin(theta) * std::sin(phi) *
                                   right.x()) +
                                 normal * std::cos(theta);

            float cR = 0.0f;
            float cG = 0.0f;
            float cB = 0.0f;
            cubemap.getPixel(0, fetch, cR, cG, cB);

            r += cR * cos(theta) * sin(theta);
            g += cG * cos(theta) * sin(theta);
            b += cB * cos(theta) * sin(theta);
          }
        }
        r = M_PI * r * (1.0 / nrSamples);
        g = M_PI * g * (1.0 / nrSamples);
        b = M_PI * b * (1.0 / nrSamples);
      }
    }
  }

  return data::Cubemap(faces, size, nbComp);

}

void
EnvProcessor::computeSpecularIS()
{
  throw "Not implemented.";
}

void
EnvProcessor::computeBRDFLUT()
{
  throw "Not implemented.";
}

} // tools

} // albedo
