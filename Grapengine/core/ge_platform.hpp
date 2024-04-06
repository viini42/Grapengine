#ifndef GRAPENGINE_GE_PLATFORM_HPP
#define GRAPENGINE_GE_PLATFORM_HPP

namespace GE
{
  class Platform
  {
  public:
    static u64 GetCurrentTimeMS();

    [[noreturn]] static void Unreachable();
  };
}

#endif // GRAPENGINE_GE_PLATFORM_HPP
