#include <grapengine.hpp>
#include <gtest/gtest.h>
#include <utils/ge_io.hpp>

#if defined(GE_CLANG_COMPILER)
  #pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

constexpr auto RANDOM_NUMBERS_COUNT = 128;

TEST(IO, ReadNotFoundFile)
{
  ASSERT_DEATH({ GE::IO::ReadFileToString({}); }, "");
}

TEST(IO, ReadFromFile)
{
  std::vector<u32> randoms;
  randoms.reserve(RANDOM_NUMBERS_COUNT);
  for (i32 i = 0; i < RANDOM_NUMBERS_COUNT; i++)
    randoms.push_back(GE::Random::GenInt());

  auto file_to_read = std::filesystem::temp_directory_path() / "unit_test.txt";
  std::stringstream ss;
  {
    for (u32 r : randoms)
      ss << r;
    std::ofstream testing_file{ file_to_read };
    testing_file << ss.rdbuf();
  }

  std::string read_string = GE::IO::ReadFileToString(file_to_read);
  ASSERT_EQ(read_string, ss.str());
}
