#ifndef GRAPENGINE_PCH_HPP
#define GRAPENGINE_PCH_HPP

#include <algorithm>
#include <any>
#include <array>
#include <bit>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <ctime>
#include <deque>
#include <exception>
#include <execution>
#include <filesystem>
#if __has_include(<format>)
  #include <format>
#endif
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numbers>
#include <numeric>
#include <random>
#include <ranges>
#include <set>
#include <source_location>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

using namespace std::string_literals;

// clang-format off
#include "log/ge_logger.hpp"

#include "core/ge_config.hpp"
#include "core/ge_assert.hpp"
#include "core/ge_type_aliases.hpp"
#include "core/ge_memory.hpp"
// clang-format on

#endif
