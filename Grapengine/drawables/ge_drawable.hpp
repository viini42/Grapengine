#ifndef GRAPENGINE_DRAWABLE_HPP
#define GRAPENGINE_DRAWABLE_HPP

#include "drawables/ge_color.hpp"
#include "renderer/ge_vertices_data.hpp"

namespace GE
{
  class Drawable final
  {
  public:
    Drawable() = default;

    explicit Drawable(const VerticesData& vertices, const std::vector<u32>& indices);

    void UpdateColor(const Color&);
    void UpdateTexture(u32 textureID);
    [[nodiscard]] const VerticesData& GetVerticesData() const;
    [[nodiscard]] const std::vector<u32>& GetIndicesData() const;

    bool operator==(const Drawable& other) const = default;

  private:
    VerticesData m_vertices_data;
    std::vector<u32> m_indices_data;
  };
}

#endif // GRAPENGINE_DRAWABLE_HPP
