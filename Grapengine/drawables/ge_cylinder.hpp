#ifndef GRAPENGINE_CYLINDER_HPP
#define GRAPENGINE_CYLINDER_HPP

#include "drawables/ge_color.hpp"
#include "drawables/ge_drawable.hpp"
#include "drawables/ge_drawing_object.hpp"
#include "math/ge_vector.hpp"

namespace GE
{
  class Cylinder final : public Drawable
  {
  public:
    Cylinder(const Vec3& basePoint, f32 radius,
             const Vec3& direction,
             f32 height, Color color);
    ~Cylinder() override;

    VerticesData GetVerticesData() const override;
    const std::vector<u32>& GetIndicesData() const override;

  private:
    Color m_color{ 0 };
    VerticesData m_vertices;
    std::vector<u32> m_indices;
  };
}

#endif // GRAPENGINE_CYLINDER_HPP
