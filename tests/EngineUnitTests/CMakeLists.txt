file(GLOB_RECURSE TestsSources *.cpp)

add_executable(EngineUnitTests
  ${TestsSources}
)
target_precompile_headers(EngineUnitTests PRIVATE tests_pch.hpp)
find_package(GTest CONFIG REQUIRED)
target_link_libraries(EngineUnitTests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

target_link_libraries(EngineUnitTests
  PRIVATE Grapengine
)
gtest_discover_tests(EngineUnitTests)