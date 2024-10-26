#ifndef GRAPENGINE_COLOR_HPP
#define GRAPENGINE_COLOR_HPP

#include "math/ge_vector.hpp"

namespace GE
{
  constexpr auto MAX_U8 = std::numeric_limits<u8>::max();
  constexpr u32 BITS_ONE_BYTE = 8;

  class Color
  {
  public:
    constexpr Color() = default;

    constexpr explicit Color(u32 rgb, u8 alpha = 0xFF) :
        m_alpha(alpha),
        m_rgb(rgb)
    {
    }

    u32 HexRGB() const;

    explicit Color(Vec3 rgbDecimal);

    explicit Color(Vec4 rgbaDecimal);

    template <typename T>
    [[nodiscard]] T ToVec4() const
    {
      return T{
        f32(R()) / f32(MAX_U8),
        f32(G()) / f32(MAX_U8),
        f32(B()) / f32(MAX_U8),
        f32(A()) / f32(MAX_U8),
      };
    }

    [[nodiscard]] Vec4 ToVec4() const;

    [[nodiscard]] Vec3 ToVec3() const;

    bool operator==(const Color& rhs) const = default;

    u8 A() const { return m_alpha; }
    u8 R() const { return static_cast<u8>((m_rgb >> BITS_ONE_BYTE * 2) & MAX_U8); }
    u8 G() const { return static_cast<u8>((m_rgb >> BITS_ONE_BYTE * 1) & MAX_U8); }
    u8 B() const { return static_cast<u8>((m_rgb >> BITS_ONE_BYTE * 0) & MAX_U8); }

  private:
    u8 m_alpha = MAX_U8;
    u32 m_rgb = 0x0;
  };

  namespace Colors
  {
    [[maybe_unused]] constexpr Color BLACK{ 0x000000 };
    [[maybe_unused]] constexpr Color WHITE{ 0xFFFFFF };
    [[maybe_unused]] constexpr Color RED{ 0xFF3333 };
    [[maybe_unused]] constexpr Color BLUE{ 0x3333FF };
    [[maybe_unused]] constexpr Color GREEN{ 0x33FF33 };
    [[maybe_unused]] constexpr Color YELLOW{ 0xFFFF33 };
    [[maybe_unused]] constexpr Color MAGENTA{ 0xFF33FF };
    [[maybe_unused]] constexpr Color ORANGE{ 0xF79300 }; // McLaren
    [[maybe_unused]] constexpr Color GRAY{ 0x57514D };

    Color RandomColor();
  }
}

#endif // GRAPENGINE_COLOR_HPP
