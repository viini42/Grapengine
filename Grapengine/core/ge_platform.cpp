#include "core/ge_platform.hpp"

using namespace GE;

u64 Platform::GetCurrentTimeNS()
{
  // Get the current time point
  auto now = std::chrono::system_clock::now();

  // Cast the time point to milliseconds since epoch
  auto duration = now.time_since_epoch();
  auto nano = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

  return static_cast<u64>(nano);
}

void Platform::Unreachable()
{
  // Uses compiler specific extensions if possible.
  // Even if no extension is used, undefined behavior is still raised by
  // an empty function body and the noreturn attribute.
  GE_ASSERT(false, "Unreachable code");
#if defined(_MSC_VER) && !defined(__clang__) // MSVC
  __assume(false);
#else // GCC, Clang
  __builtin_unreachable();
#endif
}
