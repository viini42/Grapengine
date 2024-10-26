#ifndef GE_EDITOR_CAMERA_HPP
#define GE_EDITOR_CAMERA_HPP

#include "events/ge_event_type.hpp"
#include "utils/ge_dimension.hpp"

#include <math/ge_vector.hpp>

namespace GE
{
  class TimeStep;
  class Event;

  class EditorCamera
  {
  public:
    EditorCamera(const Vec3& eye, const Vec3& target);

    void OnUpdate(TimeStep ts);
    void OnEvent(Event& event);

    void OnResize(const Dimensions& newSize);

    [[nodiscard]] Mat4 GetViewProjection() const;
    [[nodiscard]] const Vec3& GetPosition() const;

    void SetView(const Vec3& position, const Vec3& target);
    void UpdateAspectRatio(const Dimensions& viewport);

    bool operator==(const EditorCamera&) const;

  private:
    void ProcessMouseAction();
    [[nodiscard]] Mat4 ViewProjection() const;
    void MouseZoom(f32 delta);
    void UpdateView();
    void OnMouseScroll(MousePairData data);
    [[nodiscard]] bool OnMousePressed(KeyCode bt);
    [[nodiscard]] bool OnMouseReleased(KeyCode bt);
    void RecalculateProjection();

    f32 m_field_of_view;
    f32 m_aspect_ratio;
    Mat4 m_projection_mat;
    Mat4 m_view_mat;
    Vec3 m_eye;
    Vec3 m_focal_point;
    Vec2 m_mouse_init_pos{};
    bool m_can_rotate = false;
    bool m_can_pan = false;
  };
}

#endif // GE_EDITOR_CAMERA_HPP
