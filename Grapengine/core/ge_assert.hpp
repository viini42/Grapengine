#ifndef GRAPENGINE_ASSERT_HPP
#define GRAPENGINE_ASSERT_HPP

#ifndef GE_BREAKPOINT
  #ifdef GE_PLATFORM_WINDOWS
    #define GE_BREAKPOINT __debugbreak()
  #elif GE_PLATFORM_LINUX
    #define GE_BREAKPOINT __asm__("int3")
  #else
    #error "Unsupported platform"
  #endif
#endif

namespace GE
{
  template <typename... Args>
  bool Assert(const std::source_location& loc,
              bool condition,
              std::format_string<Args...> str,
              Args&&... args)
  {
    if (condition)
      return true;

    std::string msg = std::format(str, std::forward<Args>(args)...);
    GE_ERROR("Assertion failed at {}:{} {}" , loc.file_name(), loc.line(), msg)
    GE_BREAKPOINT;
    return false;
  }
}

#ifndef GE_ASSERT
  #if defined(GE_DIST)
    #define GE_ASSERT(...)
    #define GE_ASSERT_NO_MSG(...)
  #else
    #define GE_ASSERT(cond, ...) GE::Assert(std::source_location::current(), cond, __VA_ARGS__)
    #define GE_ASSERT_OR_RETURN(cond, ret, ...) if (!GE_ASSERT((cond), __VA_ARGS__))return ret
    #define GE_ASSERT_OR_RETURN_VOID(cond, ...) if (!GE_ASSERT((cond), __VA_ARGS__))return
    #define GE_ASSERT_NO_MSG(cond) GE_ASSERT(cond, "")
  #endif
#endif

#endif // GRAPENGINE_ASSERT_HPP
