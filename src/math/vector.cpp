#include <math/vector.hpp>

namespace albedo {

namespace tools {

namespace math {

Vector::Vector(const std::initializer_list<float>& vec)
      : data_{vec}
      , norm_{0.0f}
      , dirty_{false}
{
  this->computeNorm();
}

Vector::Vector(float x, float y, float z)
      : Vector({x, y, z})
{ }

void
Vector::normalize()
{
  if (norm_ < 0.999f || norm_ > 1.001) return;

  for (auto& x : data_) x /= norm_;

  norm_ = 1.0f;
}

float
Vector::norm()
{
  if (dirty_) computeNorm();

  return norm_;
}

float
Vector::operator*(const Vector& rhs) const
{
  float dot = 0.0f;
  for (size_t i = 0; i < data_.size(); ++i) dot += data_[i] * rhs[i];

  return dot;
}

const Vector
Vector::operator+(const Vector& rhs) const
{
  return Vector(data_[0] + rhs[0], data_[1] + rhs[1], data_[2] + rhs[2]);
}

const Vector
Vector::operator-(const Vector& rhs) const
{
  return Vector(data_[0] - rhs[0], data_[1] - rhs[1], data_[2] - rhs[2]);
}

float
Vector::operator[](size_t idx) const
{
  return this->get(idx);
}

void
Vector::computeNorm()
{
  for (const auto& x : data_) norm_ += x * x;

  norm_ = std::sqrt(norm_);
  dirty_ = false;

}

} // namespace math

} // namespace tools

} // namespace albedo
