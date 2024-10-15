#ifndef GRAPENGINE_RENDERER_HPP
#define GRAPENGINE_RENDERER_HPP

#include "drawables/ge_color.hpp"
#include "ge_light_source.hpp"
#include "math/ge_vector.hpp"
#include "renderer/ge_vertices_data.hpp"
#include "utils/ge_dimension.hpp"

namespace GE
{
  class VertexArray;
  class DrawingObject;

  class Renderer
  {
  public:
    static void Init();

    static void SetViewport(u32 x, u32 y, Dimensions dim);

    static void SetWireframeRenderMode(bool enabled);

    static void SetClearColor(const Vec4& color);

    static void Clear();

    static void DrawIndexed(const Ptr<VertexArray>& vao, i32 count);

    static void DrawObject(const Ptr<DrawingObject>& primitive);

    static void SetAmbientLight(const Color& color, f32 str);
    static void SetLightSources(const std::vector<LightSource>& props);

    static void SetTextureSlots(const std::vector<i32>& textureSlots);

    class Batch
    {
    public:
      static void Begin(const Mat4& cameraMatrix, const Vec3& viewPosition);

      static void End();

      static void
      PushObject(VerticesData&& vd, const std::vector<u32>& indices, const Mat4& modelMat);
    };

    struct Statistics
    {
      u64 vertices_count = 0;
      u64 indices_count = 0;
      u64 time_spent = 1;
    };

    static Statistics& GetStats();
  };
}

#endif
