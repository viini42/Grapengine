#ifndef GRAPENGINE_VECTOR_HPP
#define GRAPENGINE_VECTOR_HPP

#include "core/ge_config.hpp"
#include "core/ge_type_aliases.hpp"

namespace GE
{
  struct IVec2
  {
    i32 x;
    i32 y;
  };

  struct Vec2
  {
    f32 x;
    f32 y;
    Vec2 operator-(const Vec2& other) const noexcept;
    Vec2 operator+(const Vec2& other) const noexcept;
    Vec2 operator*(f32 fac) const;
    [[nodiscard]] f32 Length() const noexcept;
    [[nodiscard]] f32 Dot(Vec2 other) const noexcept;

    friend std::ostream& operator<<(std::ostream& os, const Vec2& vec);
    bool operator==(const Vec2& rhs) const;
    bool operator!=(const Vec2& rhs) const;
  };

  struct IVec3
  {
    i32 x;
    i32 y;
    i32 z;

    bool operator<(const IVec3& other) const;
  };

  struct Vec3
  {
    f32 x;
    f32 y;
    f32 z;

    constexpr Vec3() : x(0), y(0), z(0) {}
    constexpr Vec3(f32 xx, f32 yy, f32 zz) : x(xx), y(yy), z(zz) {}
    Vec3(Vec2 xy, f32 zz) : x(xy.x), y(xy.y), z(zz) {}

    Vec3 operator+(const Vec3& other) const;
    Vec3 operator-(const Vec3& other) const;
    Vec3& operator-();
    Vec3 operator*(f32 fac) const;
    Vec3 operator/(f32 fac) const;
    Vec3& operator+=(const Vec3& other);
    [[nodiscard]] Vec3 Normalize() const;
    [[nodiscard]] Vec3 Cross(const Vec3& other) const;
    [[nodiscard]] f32 Dot(const Vec3& other) const;
    [[nodiscard]] f32 Distance(const Vec3& other) const;

    bool operator==(const Vec3&) const;

    friend std::ostream& operator<<(std::ostream& os, const Vec3& vec3);
    [[nodiscard]] f32 Length() const;
  };

  struct Vec4
  {
    f32 x0;
    f32 x1;
    f32 x2;
    f32 x3;

    Vec4() : x0(0), x1(0), x2(0), x3(0) {}
    Vec4(f32 xx0, f32 xx1, f32 xx2, f32 xx3) : x0(xx0), x1(xx1), x2(xx2), x3(xx3) {}
    Vec4(const Vec3& v3, f32 w = 1.0f) : x0(v3.x), x1(v3.y), x2(v3.z), x3(w) {}

    Vec4 operator*(const Vec4& other) const;
    Vec4 operator+(const Vec4& other) const;
    Vec4 operator-(const Vec4& other) const;

    bool operator==(const Vec4&) const;
  };

  class Mat3
  {
  public:
    Mat3();

    Mat3(const std::array<f32, 3>& row0,
         const std::array<f32, 3>& row1,
         const std::array<f32, 3>& row2);

    Vec3 operator*(const Vec3& other) const;
    Mat3& operator/(f32 div);
    f32& operator()(u32 row, u32 col);
    const f32& operator()(u32 row, u32 col) const;

    f32 Determinant() const;

    Mat3 Adjoint() const;

    Mat3 Inverse() const;

  private:
    std::array<std::array<f32, 3>, 3> data{ { { 0.0F } } };
    Mat3 Transpose() const;
  };

  class Mat4
  {
  public:
    Mat4();

    Mat4(const std::array<f32, 4>& row0,
         const std::array<f32, 4>& row1,
         const std::array<f32, 4>& row2,
         const std::array<f32, 4>& row3);

    Mat4(const Vec4& col0, const Vec4& col1, const Vec4& col2, const Vec4& col3);

    bool operator==(const Mat4& other) const;
    Mat4 operator*(const Mat4& other) const;

    Vec4 operator*(const Vec4& other) const;
    Vec3 operator*(const Vec3& other) const;
    Mat4 operator*(f32 other) const;
    Mat4& operator/(f32 div);

    f32& operator()(u32 row, u32 col);
    const f32& operator()(u32 row, u32 col) const;
    Vec4 operator()(u32 col) const;

    [[nodiscard]] const f32* ValuePtr() const;

    f32 Determinant() const;

    Mat4 Inverse() const;

    Mat4 Adjoint() const;

    Mat4 Transpose() const;

    Mat3 ToMat3() const;

  private:
    std::array<std::array<f32, 4>, 4> data{ { { 0.0F } } };
  };
}
#endif
