#pragma once

#include <cstddef>
#include <cmath>

#include <stdexcept>

#include <vector>

namespace albedo {

namespace tools {

namespace math {

class Vector
{
  public:
    Vector(const std::initializer_list<float>& vec);

    Vector(float x, float y, float z);

  public:
    void
    normalize();

    float
    norm();

    inline float
    get(std::size_t idx) const
    {
      if (idx < 0 || idx >= 3)
      {
        throw std::invalid_argument("Vector: invalid component.");
      }
      return data_[idx];
    }

  public:
    float
    operator*(const Vector& rhs) const;

    const Vector
    operator*(float scalar) const;

    const Vector
    operator+(const Vector& rhs) const;

    const Vector
    operator-(const Vector& rhs) const;

    float
    operator[](size_t idx) const;

  private:
    void
    computeNorm();

  private:
    std::vector<float>  data_;
    float               norm_;
    bool                dirty_;

};

} // namespace math

} // namespace tools

} // namespace albedo
