#include <gtest/gtest.h>
#include <math/ge_transformations.hpp>

#if defined(GE_CLANG_COMPILER)
  #pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

TEST(Camera, EyeAndDir)
{
  //  {
  //    Vec3 eye{ 0, 4, 4 }; // World coordinates
  //    // Objects are in origin
  //    Vec3 dir{ 0, 3.2929f, 3.2929f };
  //    Camera cam{ eye, dir };
  //
  //    const auto vp1 = cam.GetViewProjection();
  //
  //    Camera cam2{ eye, 225, 0, 0 };
  //    const auto vp2 = cam2.GetViewProjection();
  //
  //    for (int i = 0; i < 4; ++i)
  //    {
  //      for (int j = 0; j < 4; ++j)
  //      {
  //        const float& f1 = vp1(i, j);
  //        const float& f2 = vp2(i, j);
  //        EXPECT_NEAR(f1, f2, 1e-5);
  //      }
  //    }
  //  }
  //
  //  {
  //    Vec3 eye{ 0, 4, 4 }; // World coordinates
  //    // Objects are in origin
  //    Vec3 dir{ 0, 4, 3 };
  //    Camera cam{ eye, dir };
  //
  //    const auto vp1 = cam.GetViewProjection();
  //
  //    Camera cam2{ eye, 180, 0, 0 };
  //    const auto vp2 = cam2.GetViewProjection();
  //
  //    for (int i = 0; i < 4; ++i)
  //    {
  //      for (int j = 0; j < 4; ++j)
  //      {
  //        const float& f1 = vp1(i, j);
  //        const float& f2 = vp2(i, j);
  //        EXPECT_NEAR(f1, f2, 1e-5);
  //      }
  //    }
  //  }
  //
  //  {
  //    Vec3 eye{ 0, 1, 3 }; // World coordinates
  //    // Objects are in origin
  //    Vec3 dir{ 0, 0, 0 };
  //    Camera cam{ eye, dir };
  //
  //    const auto vp1 = cam.GetViewProjection();
  //
  //    Camera cam2{ eye, 180 + 18.435, 0, 0 };
  //    const auto vp2 = cam2.GetViewProjection();
  //
  //    for (int i = 0; i < 4; ++i)
  //    {
  //      for (int j = 0; j < 4; ++j)
  //      {
  //        const float& f1 = vp1(i, j);
  //        const float& f2 = vp2(i, j);
  //        EXPECT_NEAR(f1, f2, 1e-5);
  //      }
  //    }
  //  }
  // {
  //   Vec3 eye{ 0, 0, 0 }; // World coordinates
  //   // Objects are in origin
  //   Vec3 dir{ 0.7554, 0.4904, 0.4345 };
  //   Camera cam{ eye, dir };
  //
  //   const auto vp1 = cam.GetViewProjection();
  //
  //   Camera cam2{ eye, 29.37, 60.09, 0 };
  //   const auto vp2 = cam2.GetViewProjection();
  //
  //   for (int i = 0; i < 4; ++i)
  //   {
  //     for (int j = 0; j < 4; ++j)
  //     {
  //       const float& f1 = vp1(i, j);
  //       const float& f2 = vp2(i, j);
  //       EXPECT_NEAR(f1, f2, 1e-4);
  //     }
  //   }
  // }
}
