#include "utils/ge_random.hpp"

#include <array>
#include <cmath>
#include <gtest/gtest.h>

using namespace GE;

#if defined(GE_CLANG_COMPILER)
  #pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

TEST(Random, Int)
{
  constexpr u32 OPTIONS = 10;
  std::array<u32, OPTIONS> counting{ 0 };
  for (u32 i = 0; i < OPTIONS * 100; ++i)
  {
    u32 random = Random::GenInt(0, OPTIONS - 1);
    counting[random]++;
  }

  for (const auto& item : counting)
  {
    EXPECT_GE(item, 50);
    EXPECT_LE(item, 150);
  }
}

TEST(Random, Float)
{
  constexpr u32 OPTIONS = 10;
  std::array<u32, OPTIONS> counting{ 0 };
  for (u32 i = 0; i < OPTIONS * 100; ++i)
  {
    float random = Random::GenFloat(0, OPTIONS);
    u32 id = static_cast<u32>(std::floor(random));
    counting[id]++;
  }

  for (const auto& item : counting)
  {
    EXPECT_GE(item, 50);
    EXPECT_LE(item, 150);
  }
}
