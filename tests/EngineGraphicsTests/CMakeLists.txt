file(GLOB_RECURSE TestsSources *.cpp)

add_executable(EngineGraphicsTests
  ${TestsSources}
)
target_precompile_headers(EngineGraphicsTests PRIVATE tests_pch.hpp)
find_package(GTest CONFIG REQUIRED)
target_link_libraries(EngineGraphicsTests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

target_link_libraries(EngineGraphicsTests
  PRIVATE Grapengine
)
gtest_discover_tests(EngineGraphicsTests)