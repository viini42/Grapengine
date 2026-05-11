#include "grapengine.hpp"

#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  GE::Logger::Init();
  return RUN_ALL_TESTS();
}
