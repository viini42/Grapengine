#ifndef GRAPENGINE_GE_SCENE_CAMERA_HPP
#define GRAPENGINE_GE_SCENE_CAMERA_HPP

#include "renderer/ge_camera.hpp"

#include <utils/ge_dimension.hpp>

namespace GE
{

  enum class ProjectionMode : u8
  {
    PERSPECTIVE,
    ORTHO
  };

  class SceneCamera final : public Camera
  {
  public:
    SceneCamera();

    [[nodiscard]] Mat4 GetViewProjection() const;
    [[nodiscard]] Vec3 GetPosition() const { return m_position; }
    [[nodiscard]] Vec3 GetTarget() const { return m_target; }
    [[nodiscard]] f32 GetFov() const { return m_fov; }
    [[nodiscard]] f32 GetOrthographicSize() const { return m_orthographic_size; }
    [[nodiscard]] ProjectionMode GetProjectionMode() const { return m_projection_mode; }
    [[nodiscard]] bool IsInProjectionMode(ProjectionMode m) const { return m == m_projection_mode; }
    [[nodiscard]] const auto& GetViewport() const { return m_viewport; }

    void SetView(Vec3 position, Vec3 target);
    void SetFov(f32 fov);
    void SetOrthographicSize(f32 size);
    void SetViewport(Dimensions vp);
    void SetProjectionMode(ProjectionMode mode);

    bool operator==(const SceneCamera&) const;

  private:
    void RecalculateProjection();

    // View matrix
    Vec3 m_position;
    Vec3 m_target;

    Dimensions m_viewport;

    // Perspective projection
    f32 m_fov;

    // Orthographic projection
    f32 m_orthographic_size;

    ProjectionMode m_projection_mode;
  };

} // GE

#endif // GRAPENGINE_GE_SCENE_CAMERA_HPP
