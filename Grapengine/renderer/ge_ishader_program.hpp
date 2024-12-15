#ifndef GRAPENGINE_ISHADER_PROGRAM_HPP
#define GRAPENGINE_ISHADER_PROGRAM_HPP

#include "ge_renderer_id.hpp"
#include "math/ge_vector.hpp"

namespace GE
{
  struct VertexStruct
  {
    Vec3 position;
    Vec2 texture_coord;
    Vec4 color;
    Vec3 normal;
    u32 texture_slot;

    bool operator==(const VertexStruct& other) const = default;
    auto operator<=>(const VertexStruct& other) const
    {
      if (color.x3 < other.color.x3)
        return 1;
      if (color.x3 > other.color.x3)
        return -1;
      return 0;
    }
  };

  class IShaderProgram
  {
  public:
    virtual ~IShaderProgram();

    virtual void Activate() = 0;
    virtual void Deactivate() = 0;

    virtual void UpdateViewProjectionMatrix(const Mat4& viewProj, const Vec3& viewPosition) = 0;
    virtual void UpdateTexture(int id) = 0;
  };
}

#endif // GRAPENGINE_ISHADER_HPP
