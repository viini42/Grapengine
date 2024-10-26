#include "math/ge_vector.hpp"

#include <math/ge_arithmetic.hpp>

using namespace GE;

Vec2 Vec2::operator-(const Vec2& other) const noexcept
{
  return { this->x - other.x, this->y - other.y };
}
Vec2 Vec2::operator+(const Vec2& other) const noexcept
{
  return { this->x + other.x, this->y + other.y };
}

Vec2 Vec2::operator*(f32 fac) const
{
  return Vec2{ x * fac, y * fac };
}

f32 Vec2::Length() const noexcept
{
  return std::sqrt(x * x + y * y);
}

f32 Vec2::Dot(Vec2 other) const noexcept
{
  const auto& [x1, y1] = *this;
  const auto& [x2, y2] = other;
  return x1 * x2 + y1 * y2;
}
bool Vec2::operator==(const Vec2& rhs) const
{
  return Arithmetic::IsEqual(x, rhs.x) && //
         Arithmetic::IsEqual(y, rhs.y);
}

bool Vec2::operator!=(const Vec2& rhs) const
{
  return !(rhs == *this);
}
namespace GE
{
  std::ostream& operator<<(std::ostream& os, const Vec2& vec)
  {
#if __cpp_lib_format
    os << std::format("{:1.2f},{:1.2f}", vec.x, vec.y);
#else
    os << std::setprecision(2) << vec.x << "," << vec.y;
#endif
    return os;
  }
}

Vec3 Vec3::operator+(const Vec3& other) const
{
  return { x + other.x, y + other.y, z + other.z };
}

Vec3 Vec3::operator-(const Vec3& other) const
{
  return { x - other.x, y - other.y, z - other.z };
}

Vec3& Vec3::operator-()
{
  x = -x;
  y = -y;
  z = -z;
  return *this;
}
Vec3 Vec3::operator*(const f32 fac) const
{
  return { x * fac, y * fac, z * fac };
}

Vec3 Vec3::operator/(f32 fac) const
{
  return { x / fac, y / fac, z / fac };
}

Vec3& Vec3::operator+=(const Vec3& other)
{
  this->x += other.x;
  this->y += other.y;
  this->z += other.z;
  return *this;
}

Vec3 Vec3::Normalize() const
{
  auto norm = std::sqrt(x * x + y * y + z * z);
  return { x / norm, y / norm, z / norm };
}

Vec3 Vec3::Cross(const Vec3& other) const
{
  const auto& [x1, y1, z1] = *this;
  const auto& [x2, y2, z2] = other;

  // clang-format off
  return {
    y1 * z2 - y2 * z1,
    x2 * z1 - x1 * z2,
    x1 * y2 - x2 * y1
  };
  // clang-format on
}

f32 Vec3::Dot(const Vec3& other) const
{
  const auto& [x1, y1, z1] = *this;
  const auto& [x2, y2, z2] = other;
  return x1 * x2 + y1 * y2 + z1 * z2;
}

f32 Vec3::Distance(const Vec3& other) const
{
  const Vec3 diff = *this - other;
  return diff.Length();
}

bool Vec3::operator==(const Vec3& rhs) const
{
  return Arithmetic::IsEqual(x, rhs.x) && //
         Arithmetic::IsEqual(y, rhs.y) && //
         Arithmetic::IsEqual(z, rhs.z);
}

f32 Vec3::Length() const
{
  return std::sqrt(x * x + y * y + z * z);
}

namespace GE
{
  std::ostream& operator<<(std::ostream& os, const Vec3& vec3)
  {
#if __cpp_lib_format
    os << std::format("{:1.2f},{:1.2f},{:1.2f}", vec3.x, vec3.y, vec3.z);
#else
    os << std::setprecision(2) << vec3.x << "," << vec3.y << "," << vec3.z;
#endif
    return os;
  }
}

Mat4::Mat4() : Mat4({ { 1, 0, 0, 0 } }, { { 0, 1, 0, 0 } }, { { 0, 0, 1, 0 } }, { { 0, 0, 0, 1 } })
{
}

Mat4::Mat4(const std::array<f32, 4>& row0,
           const std::array<f32, 4>& row1,
           const std::array<f32, 4>& row2,
           const std::array<f32, 4>& row3)
{
  for (u32 i = 0; i < 4; ++i)
  {
    data.at(i).at(0) = row0.at(i);
    data.at(i).at(1) = row1.at(i);
    data.at(i).at(2) = row2.at(i);
    data.at(i).at(3) = row3.at(i);
  }
}

Mat4::Mat4(const Vec4& col0, const Vec4& col1, const Vec4& col2, const Vec4& col3)
{
  data.at(0).at(0) = col0.x0;
  data.at(0).at(1) = col0.x1;
  data.at(0).at(2) = col0.x2;
  data.at(0).at(3) = col0.x3;

  data.at(1).at(0) = col1.x0;
  data.at(1).at(1) = col1.x1;
  data.at(1).at(2) = col1.x2;
  data.at(1).at(3) = col1.x3;

  data.at(2).at(0) = col2.x0;
  data.at(2).at(1) = col2.x1;
  data.at(2).at(2) = col2.x2;
  data.at(2).at(3) = col2.x3;

  data.at(3).at(0) = col3.x0;
  data.at(3).at(1) = col3.x1;
  data.at(3).at(2) = col3.x2;
  data.at(3).at(3) = col3.x3;
}

Mat4 Mat4::operator*(const Mat4& other) const
{
  auto prod = [m1 = *this, m2 = other](u32 r, unsigned c)
  {
    // clang-format off
    return m1(r, 0) * m2(0, c) +
           m1(r, 1) * m2(1, c) +
           m1(r, 2) * m2(2, c) +
           m1(r, 3) * m2(3, c);
    // clang-format on
  };

  Mat4 res;
  for (u32 row = 0; row < 4; ++row)
  {
    for (u32 col = 0; col < 4; ++col)
    {
      res(row, col) = prod(row, col);
    }
  }

  return res;
}

f32& Mat4::operator()(u32 row, u32 col)
{
  return data.at(col).at(row);
}

const f32& Mat4::operator()(u32 row, u32 col) const
{
  return data.at(col).at(row);
}

const f32* Mat4::ValuePtr() const
{
  return data.data()->data();
}

bool Mat4::operator==(const Mat4& other) const
{
  for (u32 i = 0; i < 4; ++i)
  {
    for (u32 j = 0; j < 4; ++j)
    {
      if (!Arithmetic::IsEqual(other(i, j), this->operator()(i, j)))
        return false;
    }
  }
  return true;
}

Vec4 Mat4::operator*(const Vec4& other) const
{
  Vec4 res{ 0, 0, 0, 0 };
  const Mat4& m = *this;
  res.x0 = m(0, 0) * other.x0 + m(0, 1) * other.x1 + m(0, 2) * other.x2 + m(0, 3) * other.x3;
  res.x1 = m(1, 0) * other.x0 + m(1, 1) * other.x1 + m(1, 2) * other.x2 + m(1, 3) * other.x3;
  res.x2 = m(2, 0) * other.x0 + m(2, 1) * other.x1 + m(2, 2) * other.x2 + m(2, 3) * other.x3;
  res.x3 = m(3, 0) * other.x0 + m(3, 1) * other.x1 + m(3, 2) * other.x2 + m(3, 3) * other.x3;
  return res;
}

Vec3 Mat4::operator*(const Vec3& other) const
{
  const Vec4 vec{ other };
  auto res = this->operator*(vec);
  return { res.x0, res.x1, res.x2 };
}

Mat4 Mat4::Inverse() const
{
  const f32 det = this->Determinant();
  const Mat4 res = this->Adjoint() / det;
  return res;
}

Vec4 Mat4::operator()(u32 col) const
{
  return { data.at(col).at(0), data.at(col).at(1), data.at(col).at(2), data.at(col).at(3) };
}

Mat4 Mat4::operator*(f32 other) const
{
  Mat4 res{};
  for (u32 row = 0; row < 4; row++)
    for (u32 col = 0; col < 4; col++)
      res(row, col) = operator()(row, col) * other;
  return res;
}

Mat4 Mat4::Transpose() const
{
  Mat4 res{};
  for (u32 row = 0; row < 4; row++)
    for (u32 col = 0; col < 4; col++)
      res(row, col) = operator()(col, row);
  return res;
}

Mat3 Mat4::ToMat3() const
{
  const auto& m = *this;
  return Mat3{ { { m(0, 0), m(0, 1), m(0, 2) } }, //
               { { m(1, 0), m(1, 1), m(1, 2) } }, //
               { { m(2, 0), m(2, 1), m(2, 2) } } };
}

f32 Mat4::Determinant() const
{
  const auto& m = *this;
  const Mat3 m0{
    { { m(1, 1), m(1, 2), m(1, 3) } },
    { { m(2, 1), m(2, 2), m(2, 3) } },
    { { m(3, 1), m(3, 2), m(3, 3) } },
  };

  const Mat3 m1{
    { { m(1, 0), m(1, 2), m(1, 3) } },
    { { m(2, 0), m(2, 2), m(2, 3) } },
    { { m(3, 0), m(3, 2), m(3, 3) } },
  };

  const Mat3 m2{
    { { m(1, 0), m(1, 1), m(1, 3) } },
    { { m(2, 0), m(2, 1), m(2, 3) } },
    { { m(3, 0), m(3, 1), m(3, 3) } },
  };

  const Mat3 m3{
    { { m(1, 0), m(1, 1), m(1, 2) } },
    { { m(2, 0), m(2, 1), m(2, 2) } },
    { { m(3, 0), m(3, 1), m(3, 2) } },
  };

  return                         //
    m(0, 0) * m0.Determinant()   //
    - m(0, 1) * m1.Determinant() //
    + m(0, 2) * m2.Determinant() //
    - m(0, 3) * m3.Determinant();
}

Mat4 Mat4::Adjoint() const
{
  const auto& m = *this;
  // clang-format off
  const f32 d00 = +Mat3{{{ m(1, 1), m(1, 2), m(1, 3) }}, {{ m(2, 1), m(2, 2), m(2, 3) }}, {{ m(3, 1), m(3, 2), m(3, 3) }}}.Determinant();
  const f32 d01 = -Mat3{{{ m(1, 0), m(1, 2), m(1, 3) }}, {{ m(2, 0), m(2, 2), m(2, 3) }}, {{ m(3, 0), m(3, 2), m(3, 3) }}}.Determinant();
  const f32 d02 = +Mat3{{{ m(1, 0), m(1, 1), m(1, 3) }}, {{ m(2, 0), m(2, 1), m(2, 3) }}, {{ m(3, 0), m(3, 1), m(3, 3) }}}.Determinant();
  const f32 d03 = -Mat3{{{ m(1, 0), m(1, 1), m(1, 2) }}, {{ m(2, 0), m(2, 1), m(2, 2) }}, {{ m(3, 0), m(3, 1), m(3, 2) }}}.Determinant();
  const f32 d10 = -Mat3{{{ m(0, 1), m(0, 2), m(0, 3) }}, {{ m(2, 1), m(2, 2), m(2, 3) }}, {{ m(3, 1), m(3, 2), m(3, 3) }}}.Determinant();
  const f32 d11 = +Mat3{{{ m(0, 0), m(0, 2), m(0, 3) }}, {{ m(2, 0), m(2, 2), m(2, 3) }}, {{ m(3, 0), m(3, 2), m(3, 3) }}}.Determinant();
  const f32 d12 = -Mat3{{{ m(0, 0), m(0, 1), m(0, 3) }}, {{ m(2, 0), m(2, 1), m(2, 3) }}, {{ m(3, 0), m(3, 1), m(3, 3) }}}.Determinant();
  const f32 d13 = +Mat3{{{ m(0, 0), m(0, 1), m(0, 2) }}, {{ m(2, 0), m(2, 1), m(2, 2) }}, {{ m(3, 0), m(3, 1), m(3, 2) }}}.Determinant();
  const f32 d20 = +Mat3{{{ m(0, 1), m(0, 2), m(0, 3) }}, {{ m(1, 1), m(1, 2), m(1, 3) }}, {{ m(3, 1), m(3, 2), m(3, 3) }}}.Determinant();
  const f32 d21 = -Mat3{{{ m(0, 0), m(0, 2), m(0, 3) }}, {{ m(1, 0), m(1, 2), m(1, 3) }}, {{ m(3, 0), m(3, 2), m(3, 3) }}}.Determinant();
  const f32 d22 = +Mat3{{{ m(0, 0), m(0, 1), m(0, 3) }}, {{ m(1, 0), m(1, 1), m(1, 3) }}, {{ m(3, 0), m(3, 1), m(3, 3) }}}.Determinant();
  const f32 d23 = -Mat3{{{ m(0, 0), m(0, 1), m(0, 2) }}, {{ m(1, 0), m(1, 1), m(1, 2) }}, {{ m(3, 0), m(3, 1), m(3, 2) }}}.Determinant();
  const f32 d30 = -Mat3{{{ m(0, 1), m(0, 2), m(0, 3) }}, {{ m(1, 1), m(1, 2), m(1, 3) }}, {{ m(2, 1), m(2, 2), m(2, 3) }}}.Determinant();
  const f32 d31 = +Mat3{{{ m(0, 0), m(0, 2), m(0, 3) }}, {{ m(1, 0), m(1, 2), m(1, 3) }}, {{ m(2, 0), m(2, 2), m(2, 3) }}}.Determinant();
  const f32 d32 = -Mat3{{{ m(0, 0), m(0, 1), m(0, 3) }}, {{ m(1, 0), m(1, 1), m(1, 3) }}, {{ m(2, 0), m(2, 1), m(2, 3) }}}.Determinant();
  const f32 d33 = +Mat3{{{ m(0, 0), m(0, 1), m(0, 2) }}, {{ m(1, 0), m(1, 1), m(1, 2) }}, {{ m(2, 0), m(2, 1), m(2, 2) }}}.Determinant();
  // clang-format on
  return Mat4{
    { { d00, d01, d02, d03 } },
    { { d10, d11, d12, d13 } },
    { { d20, d21, d22, d23 } },
    { { d30, d31, d32, d33 } },
  }
    .Transpose();
}

Mat4& Mat4::operator/(f32 div)
{
  GE_ASSERT(!Arithmetic::IsEqual(div, 0.0f), "Division by zero");

  data.at(0).at(0) /= div;
  data.at(0).at(1) /= div;
  data.at(0).at(2) /= div;
  data.at(0).at(3) /= div;
  data.at(1).at(0) /= div;
  data.at(1).at(1) /= div;
  data.at(1).at(2) /= div;
  data.at(1).at(3) /= div;
  data.at(2).at(0) /= div;
  data.at(2).at(1) /= div;
  data.at(2).at(2) /= div;
  data.at(2).at(3) /= div;
  data.at(3).at(0) /= div;
  data.at(3).at(1) /= div;
  data.at(3).at(2) /= div;
  data.at(3).at(3) /= div;
  return *this;
}

bool IVec3::operator<(const IVec3& other) const
{
  if (x < other.x)
    return true;

  if (x == other.x)
  {
    if (y < other.y)
      return true;

    if (y == other.y)
    {
      if (z < other.z)
        return true;
    }
  }
  return false;
}

Vec4 Vec4::operator*(const Vec4& other) const
{
  return { x0 * other.x0, x1 * other.x1, x2 * other.x2, x3 * other.x3 };
}

Vec4 Vec4::operator+(const Vec4& other) const
{
  return { x0 + other.x0, x1 + other.x1, x2 + other.x2, x3 + other.x3 };
}

Vec4 Vec4::operator-(const Vec4& other) const
{
  return { x0 - other.x0, x1 - other.x1, x2 - other.x2, x3 - other.x3 };
}

bool Vec4::operator==(const Vec4& rhs) const
{
  return Arithmetic::IsEqual(x0, rhs.x0) && //
         Arithmetic::IsEqual(x1, rhs.x1) && //
         Arithmetic::IsEqual(x2, rhs.x2) && //
         Arithmetic::IsEqual(x3, rhs.x3);
}

Mat3::Mat3() : Mat3({ { 1, 0, 0 } }, { { 0, 1, 0 } }, { { 0, 0, 1 } }) {}

Mat3::Mat3(const std::array<f32, 3>& row0,
           const std::array<f32, 3>& row1,
           const std::array<f32, 3>& row2)
{
  for (u32 i = 0; i < 3; ++i)
  {
    data.at(i).at(0) = row0.at(i);
    data.at(i).at(1) = row1.at(i);
    data.at(i).at(2) = row2.at(i);
  }
}

Vec3 Mat3::operator*(const Vec3& other) const
{
  Vec3 res{ 0, 0, 0 };
  const auto& m = *this;
  res.x = m(0, 0) * other.x + m(0, 1) * other.y + m(0, 2) * other.z;
  res.y = m(1, 0) * other.x + m(1, 1) * other.y + m(1, 2) * other.z;
  res.z = m(2, 0) * other.x + m(2, 1) * other.y + m(2, 2) * other.z;
  return res;
}

f32& Mat3::operator()(u32 row, u32 col)
{
  return data.at(col).at(row);
}

const f32& Mat3::operator()(u32 row, u32 col) const
{
  return data.at(col).at(row);
}

f32 Mat3::Determinant() const
{
  const auto& a = operator()(0, 0);
  const auto& b = operator()(0, 1);
  const auto& c = operator()(0, 2);
  const auto& d = operator()(1, 0);
  const auto& e = operator()(1, 1);
  const auto& f = operator()(1, 2);
  const auto& g = operator()(2, 0);
  const auto& h = operator()(2, 1);
  const auto& i = operator()(2, 2);

  return a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
}

Mat3 Mat3::Adjoint() const
{
  // A_ij = (-1)^(i+j) det(M_ij)

  const auto& m = *this;

  const f32 cof00 = +(m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2));
  const f32 cof10 = -(m(0, 1) * m(2, 2) - m(2, 1) * m(0, 2));
  const f32 cof20 = +(m(0, 1) * m(1, 2) - m(1, 1) * m(0, 2));
  const f32 cof01 = -(m(1, 0) * m(2, 2) - m(2, 0) * m(1, 2));
  const f32 cof11 = +(m(0, 0) * m(2, 2) - m(2, 0) * m(0, 2));
  const f32 cof21 = -(m(0, 0) * m(1, 2) - m(1, 0) * m(0, 2));
  const f32 cof02 = +(m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1));
  const f32 cof12 = -(m(0, 0) * m(2, 1) - m(2, 0) * m(0, 1));
  const f32 cof22 = +(m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1));

  return Mat3{
    { { cof00, cof01, cof02 } }, //
    { { cof10, cof11, cof12 } }, //
    { { cof20, cof21, cof22 } }  //
  }
    .Transpose();
}

Mat3 Mat3::Inverse() const
{
  const f32 det = this->Determinant();
  const Mat3 res = this->Adjoint() / det;
  return res;
}

Mat3& Mat3::operator/(f32 div)
{
  GE_ASSERT(!Arithmetic::IsEqual(div, 0.0f), "Division by zero");

  data.at(0).at(0) /= div;
  data.at(0).at(1) /= div;
  data.at(0).at(2) /= div;
  data.at(1).at(0) /= div;
  data.at(1).at(1) /= div;
  data.at(1).at(2) /= div;
  data.at(2).at(0) /= div;
  data.at(2).at(1) /= div;
  data.at(2).at(2) /= div;

  return *this;
}

Mat3 Mat3::Transpose() const
{
  Mat3 res{};
  for (u32 row = 0; row < 3; row++)
    for (u32 col = 0; col < 3; col++)
      res(row, col) = operator()(col, row);
  return res;
}
