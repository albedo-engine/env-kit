#include <math/vector.hpp>

namespace albedo {

namespace tools {

namespace math {

Vector::Vector()
      : data_({0.0f, 0.0f, 0.0f})
      , norm_{0.0f}
      , dirty_{false}
{ }

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
  if (norm_ >= 0.999f && norm_ <= 1.001) return;

  float norm = this->norm();
  for (auto& x : data_) x /= norm;

  norm_ = 1.0f;
}

float
Vector::norm()
{
  if (dirty_) computeNorm();

  return norm_;
}

void
Vector::set(float x, float y, float z)
{
  data_[0] = x;
  data_[1] = y;
  data_[2] = z;

  dirty_ = true;
}

float
Vector::operator*(const Vector& rhs) const
{
  float dot = 0.0f;
  for (size_t i = 0; i < data_.size(); ++i) dot += data_[i] * rhs[i];

  return dot;
}

const Vector
Vector::operator*(float scalar) const
{
  return Vector(this->x() * scalar, this->y() * scalar, this->z() * scalar);
}

Vector&
Vector::operator*=(float scalar)
{
  data_[0] *= scalar;
  data_[1] *= scalar;
  data_[2] *= scalar;

  return *this;
}

const Vector
Vector::operator/(float scalar) const
{
  return Vector(this->x() / scalar, this->y() / scalar, this->z() / scalar);
}

Vector&
Vector::operator/=(float scalar)
{
  data_[0] /= scalar;
  data_[1] /= scalar;
  data_[2] /= scalar;

  return *this;
}

const Vector
Vector::operator^(const Vector& rhs) const
{
  float x = data_[1] * rhs[2] - data_[2] * rhs[1];
  float y = data_[2] * rhs[0] - data_[0] * rhs[2];
  float z = data_[0] * rhs[1] - data_[1] * rhs[0];

  return Vector(x, y, z);
}

const Vector
Vector::operator+(const Vector& rhs) const
{
  return Vector(data_[0] + rhs[0], data_[1] + rhs[1], data_[2] + rhs[2]);
}

const Vector
Vector::operator+(float scalar) const
{
  return Vector(data_[0] + scalar, data_[1] + scalar, data_[2] + scalar);
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
