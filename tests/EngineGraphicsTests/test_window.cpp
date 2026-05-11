#include "core/ge_memory.hpp"
#include "core/ge_window.hpp"

#include <gtest/gtest.h>

#if defined(GE_CLANG_COMPILER)
  #pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

using namespace GE;

TEST(Window, Initialization)
{
  auto window = MakeScope<Window>(WindowProps{ "Test", { 100, 200 }, {} }, nullptr);
  EXPECT_NE(window, nullptr);

  EXPECT_EQ(window->GetDimension().width, 100);

  EXPECT_EQ(window->GetDimension().height, 200);
}
