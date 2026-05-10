#include "math/ge_vector.hpp"

#include <gtest/gtest.h>

#if defined(GE_CLANG_COMPILER)
  #pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

using namespace GE;

TEST(Vec3, MinusOperator)
{
  Vec3 v1{ 1, 2, 3 };
  Vec3 v2{ 4, 5, 6 };
  Vec3 res = v2 - v1;

  ASSERT_EQ(res.x, 3);
  ASSERT_EQ(res.y, 3);
  ASSERT_EQ(res.z, 3);
}

TEST(Vec3, UnaryMinusOperator)
{
  Vec3 res{ 1, 2, 3 };
  -res;

  ASSERT_EQ(res.x, -1);
  ASSERT_EQ(res.y, -2);
  ASSERT_EQ(res.z, -3);
}

TEST(Vec3, Normalize)
{
  Vec3 v1{ 1, 2, 3 };
  Vec3 res = v1.Normalize();

  ASSERT_FLOAT_EQ(res.x, 0.267261241912424f);
  ASSERT_FLOAT_EQ(res.y, 0.534522483824849f);
  ASSERT_FLOAT_EQ(res.z, 0.801783725737273f);
}

TEST(Vec3, Cross)
{
  Vec3 v1{ 1, 2, 3 };
  Vec3 v2{ 4, 5, 6 };
  Vec3 res = v1.Cross(v2);

  ASSERT_EQ(res.x, -3);
  ASSERT_EQ(res.y, 6);
  ASSERT_EQ(res.z, -3);
}

TEST(Vec3, Dot)
{
  Vec3 v1{ 1, 2, 3 };
  Vec3 v2{ 4, 5, 6 };
  float res = v1.Dot(v2);

  ASSERT_FLOAT_EQ(res, 32);
}

TEST(Mat4, Mat4Multiply)
{
  Mat4 m1{
    std::array<f32, 4>{ { 1, 2, 3, 4 } },
    std::array<f32, 4>{ { 5, 6, 7, 8 } },
    std::array<f32, 4>{ { 9, 10, 11, 12 } },
    std::array<f32, 4>{ { 13, 14, 15, 16 } },
  };

  Mat4 m2 = m1;

  auto prod = m1 * m2;

  ASSERT_EQ(prod(0, 0), 90);
  ASSERT_EQ(prod(0, 1), 100);
  ASSERT_EQ(prod(0, 2), 110);
  ASSERT_EQ(prod(0, 3), 120);

  ASSERT_EQ(prod(1, 0), 202);
  ASSERT_EQ(prod(1, 1), 228);
  ASSERT_EQ(prod(1, 2), 254);
  ASSERT_EQ(prod(1, 3), 280);

  ASSERT_EQ(prod(2, 0), 314);
  ASSERT_EQ(prod(2, 1), 356);
  ASSERT_EQ(prod(2, 2), 398);
  ASSERT_EQ(prod(2, 3), 440);

  ASSERT_EQ(prod(3, 0), 426);
  ASSERT_EQ(prod(3, 1), 484);
  ASSERT_EQ(prod(3, 2), 542);
  ASSERT_EQ(prod(3, 3), 600);
}

TEST(Mat4, Mat4MultiplyVec3)
{
  Mat4 m1{
    std::array<f32, 4>{ { 1, 2, 3, 4 } },
    std::array<f32, 4>{ { 5, 6, 7, 8 } },
    std::array<f32, 4>{ { 9, 10, 11, 12 } },
    std::array<f32, 4>{ { 13, 14, 15, 16 } },
  };

  Vec3 v3{ 1, 2, 3 };

  auto prod = m1 * v3;

  ASSERT_EQ(prod.x, 18);
  ASSERT_EQ(prod.y, 46);
  ASSERT_EQ(prod.z, 74);
}

TEST(Mat3, Determinant)
{
  Mat3 m{
    std::array<f32, 3>{ 5, -2, 2 }, //
    std::array<f32, 3>{ 1, 0, 0 },  //
    std::array<f32, 3>{ -3, 1, 5 }, //
  };

  f32 det = m.Determinant();

  ASSERT_EQ(det, 12);
}

TEST(Mat4, Determinant)
{
  Mat4 m1{
    std::array<f32, 4>{ { 5, -2, 2, 7 } },
    std::array<f32, 4>{ { 1, 0, 0, 3 } },
    std::array<f32, 4>{ { -3, 1, 5, 0 } },
    std::array<f32, 4>{ { 3, -1, -9, 4 } },
  };

  f32 det = m1.Determinant();

  ASSERT_EQ(det, 88);
}

TEST(Mat4, Transpose)
{
  Mat4 m1{
    std::array<f32, 4>{ { 5, -2, 2, 7 } },
    std::array<f32, 4>{ { 1, 0, 0, 3 } },
    std::array<f32, 4>{ { -3, 1, 5, 0 } },
    std::array<f32, 4>{ { 3, -1, -9, 4 } },
  };

  auto m_transp = m1.Transpose();

  ASSERT_EQ(m_transp(0, 1), m1(1, 0));
  ASSERT_EQ(m_transp(0, 2), m1(2, 0));
  ASSERT_EQ(m_transp(0, 3), m1(3, 0));
  ASSERT_EQ(m_transp(1, 0), m1(0, 1));
  ASSERT_EQ(m_transp(1, 2), m1(2, 1));
  ASSERT_EQ(m_transp(1, 3), m1(3, 1));
  ASSERT_EQ(m_transp(2, 0), m1(0, 2));
  ASSERT_EQ(m_transp(2, 1), m1(1, 2));
  ASSERT_EQ(m_transp(2, 3), m1(3, 2));
  ASSERT_EQ(m_transp(3, 0), m1(0, 3));
  ASSERT_EQ(m_transp(3, 1), m1(1, 3));
  ASSERT_EQ(m_transp(3, 2), m1(2, 3));
}

TEST(Mat4, Adj)
{
  Mat4 m1{
    std::array<f32, 4>{ { 5, -2, 2, 7 } },
    std::array<f32, 4>{ { 1, 0, 0, 3 } },
    std::array<f32, 4>{ { -3, 1, 5, 0 } },
    std::array<f32, 4>{ { 3, -1, -9, 4 } },
  };

  auto adj = m1.Adjoint();

  ASSERT_EQ(adj(0, 0), -12);
  ASSERT_EQ(adj(0, 1), 76);
  ASSERT_EQ(adj(0, 2), -60);
  ASSERT_EQ(adj(0, 3), -36);
  ASSERT_EQ(adj(1, 0), -56);
  ASSERT_EQ(adj(1, 1), 208);
  ASSERT_EQ(adj(1, 2), -82);
  ASSERT_EQ(adj(1, 3), -58);
  ASSERT_EQ(adj(2, 0), 4);
  ASSERT_EQ(adj(2, 1), 4);
  ASSERT_EQ(adj(2, 2), -2);
  ASSERT_EQ(adj(2, 3), -10);
  ASSERT_EQ(adj(3, 0), 4);
  ASSERT_EQ(adj(3, 1), 4);
  ASSERT_EQ(adj(3, 2), 20);
  ASSERT_EQ(adj(3, 3), 12);
}

TEST(Mat4, Inverse)
{
  Mat4 m1{
    std::array<f32, 4>{ { 5, -2, 2, 7 } },
    std::array<f32, 4>{ { 1, 0, 0, 3 } },
    std::array<f32, 4>{ { -3, 1, 5, 0 } },
    std::array<f32, 4>{ { 3, -1, -9, 4 } },
  };

  auto inv = m1.Inverse();

  ASSERT_NEAR(inv(0, 0), 1.0f / 44.0f * -6, 0.001f);
  ASSERT_NEAR(inv(0, 1), 1.0f / 44.0f * 38, 0.001f);
  ASSERT_NEAR(inv(0, 2), 1.0f / 44.0f * -30, 0.001f);
  ASSERT_NEAR(inv(0, 3), 1.0f / 44.0f * -18, 0.001f);
  ASSERT_NEAR(inv(1, 0), 1.0f / 44.0f * -28, 0.001f);
  ASSERT_NEAR(inv(1, 1), 1.0f / 44.0f * 104, 0.001f);
  ASSERT_NEAR(inv(1, 2), 1.0f / 44.0f * -41, 0.001f);
  ASSERT_NEAR(inv(1, 3), 1.0f / 44.0f * -29, 0.001f);
  ASSERT_NEAR(inv(2, 0), 1.0f / 44.0f * 2, 0.001f);
  ASSERT_NEAR(inv(2, 1), 1.0f / 44.0f * 2, 0.001f);
  ASSERT_NEAR(inv(2, 2), 1.0f / 44.0f * -1, 0.001f);
  ASSERT_NEAR(inv(2, 3), 1.0f / 44.0f * -5, 0.001f);
  ASSERT_NEAR(inv(3, 0), 1.0f / 44.0f * 2, 0.001f);
  ASSERT_NEAR(inv(3, 1), 1.0f / 44.0f * 2, 0.001f);
  ASSERT_NEAR(inv(3, 2), 1.0f / 44.0f * 10, 0.001f);
  ASSERT_NEAR(inv(3, 3), 1.0f / 44.0f * 6, 0.001f);
}

TEST(Mat3, Adjoint)
{
  Mat3 m({ 1, -1, 2 }, //
         { 4, 0, 6 },  //
         { 0, 1, -1 }  //
  );

  auto adj = m.Adjoint();

  ASSERT_EQ(adj(0, 0), -6);
  ASSERT_EQ(adj(0, 1), 1);
  ASSERT_EQ(adj(0, 2), -6);
  ASSERT_EQ(adj(1, 0), 4);
  ASSERT_EQ(adj(1, 1), -1);
  ASSERT_EQ(adj(1, 2), 2);
  ASSERT_EQ(adj(2, 0), 4);
  ASSERT_EQ(adj(2, 1), -1);
  ASSERT_EQ(adj(2, 2), 4);
}

TEST(Mat3, Inverse)
{
  Mat3 m({ -1, -5, 12 }, //
         { 0, 1, -2 },   //
         { 0, 0, -1 }    //
  );

  auto inv = m.Inverse();

  ASSERT_EQ(inv(0, 0), -1);
  ASSERT_EQ(inv(0, 1), -5);
  ASSERT_EQ(inv(0, 2), -2);
  ASSERT_EQ(inv(1, 0), 0);
  ASSERT_EQ(inv(1, 1), 1);
  ASSERT_EQ(inv(1, 2), -2);
  ASSERT_EQ(inv(2, 0), 0);
  ASSERT_EQ(inv(2, 1), 0);
  ASSERT_EQ(inv(2, 2), -1);
}
