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

  float r = 0.0f;
  float g = 0.0f;
  float b = 0.0f;

  // TODO: It could be a better idea to change the first loop
  // TODO: by a loop going from 0 to 5.
  //for (int x = -halfSize; x < halfSize; ++x)
  for (int f = 0; f < 6; ++f)
  {
    for (int u = -halfSize; u < halfSize; ++u)
    {
      for (int v = -halfSize; v < halfSize; ++v)
      {
        r = 0.0f;
        g = 0.0f;
        b = 0.0f;

        math::Vector up(0.0f, 1.0f, 0.0f);
        //math::Vector faceVec = (data::Cubemap::FACE_TO_VEC.at(f) * halfSize);
        // DEBUG
        math::Vector normal;
        if (f == 0)
        {
          normal = math::Vector(halfSize, u, v);
        }
        else if (f == 1)
        {
          normal = math::Vector(- halfSize, u, v);
        }
        else if (f == 2)
        {
          normal = math::Vector(u, halfSize, v);
        }
        else if (f == 3)
        {
          normal = math::Vector(u, - halfSize, v);
        }
        else if (f == 4)
        {
          normal = math::Vector(u, v, halfSize);
        }
        else if (f == 5)
        {
          normal = math::Vector(u, v, - halfSize);
        }
        //normal.set(x, y, z);
        normal.normalize();
        // END DEBUG

        math::Vector right = up ^ normal;
        up = normal ^ right;

        // Integrates over the the y axis
        /*float nrSamples = 0.0f;
        for (float phi = 0.0f; phi < 2.0f * M_PI; phi += step)
        {
          // Integrates over the the normal axis
          for (float theta = 0.0f; theta < 0.5f * M_PI; theta += step)
          {
            math::Vector fetch = right * (std::sin(theta) * std::cos(phi)) +
                                 up * (std::sin(theta) * std::sin(phi) * right.x())
                                 + normal * std::cos(theta);

            float cR = 0.0f;
            float cG = 0.0f;
            float cB = 0.0f;
            cubemap.getPixel(0, fetch, cR, cG, cB);

            r += cR * cos(theta) * sin(theta);
            g += cG * cos(theta) * sin(theta);
            b += cB * cos(theta) * sin(theta);

            nrSamples++;
          }
        }
        r = M_PI * r * (1.0f / nrSamples);
        g = M_PI * g * (1.0f / nrSamples);
        b = M_PI * b * (1.0f / nrSamples);*/

        // DEBUG
        cubemap.getPixel(0, normal, r, g, b);
        // END DEBUG

        int px = u + halfSize;
        int py = v + halfSize;
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
