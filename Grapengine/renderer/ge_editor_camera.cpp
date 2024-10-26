#include "renderer/ge_editor_camera.hpp"

#include "core/ge_platform.hpp"
#include "math/ge_geometry.hpp"
#include "math/ge_quaternions.hpp"
#include "profiling/ge_profiler.hpp"

#include <core/ge_time_step.hpp>
#include <events/ge_event.hpp>
#include <input/ge_input.hpp>
#include <math/ge_transformations.hpp>
#include <math/ge_vector.hpp>

using namespace GE;

namespace
{
  constexpr Vec3 UP_DIR{ 0, 1, 0 };

  constexpr auto DEFAULT_FOV = 30.0f;
  constexpr auto ZOOM_FACTOR = 15;
}

EditorCamera::EditorCamera(const Vec3& eye, const Vec3& target) :
    m_field_of_view(DEFAULT_FOV), m_aspect_ratio(1), m_eye(eye), m_focal_point(target)
{
  m_projection_mat = Transform::Perspective(m_field_of_view, m_aspect_ratio);
}

void EditorCamera::OnUpdate(TimeStep /*ts*/)
{
  if (Input::IsKeyPressed(KeyCode::LEFT_CONTROL))
    ProcessMouseAction();

  UpdateView();
}

void EditorCamera::OnEvent(Event& event)
{
  event //
    .When(EvType::MOUSE_SCROLL)
    .Then([this](const EvData& data) { OnMouseScroll(*std::get_if<MousePairData>(&data)); });

  event //
    .When(EvType::MOUSE_BUTTON_PRESSED)
    .ThenWithRes([this](const EvData& data) -> bool
                 { return OnMousePressed(*std::get_if<KeyCode>(&data)); });

  event //
    .When(EvType::MOUSE_BUTTON_RELEASE)
    .ThenWithRes([this](const EvData& data) -> bool
                 { return OnMouseReleased(*std::get_if<KeyCode>(&data)); });
}

Mat4 EditorCamera::GetViewProjection() const
{
  return ViewProjection();
}

const Vec3& EditorCamera::GetPosition() const
{
  return m_eye;
}

void EditorCamera::SetView(const Vec3& position, const Vec3& target)
{
  GE_PROFILE;
  m_eye = position;
  m_focal_point = target;
  RecalculateProjection();
}

bool EditorCamera::operator==(const EditorCamera& other) const
{
  return m_eye == other.m_eye && m_focal_point == other.m_focal_point;
}

void EditorCamera::OnResize(const Dimensions& newSize)
{
  UpdateAspectRatio(newSize);
}

void EditorCamera::ProcessMouseAction()
{
  const Vec2 mouse_pos = Input::GetMouseXY();
  const Vec2 delta = (mouse_pos - m_mouse_init_pos);
  m_mouse_init_pos = mouse_pos;


  if (std::abs(delta.x) < std::numeric_limits<f32>::epsilon() &&
      std::abs(delta.y) < std::numeric_limits<f32>::epsilon())
    return;

  auto [x_eye, y_eye, z_eye] = Transform::LookAtVector(m_eye, m_focal_point, UP_DIR);

  if (m_can_rotate)
  {
    // Rotate a given point around plane contains x_eye
    const auto eye_origin_reference = (m_eye - m_focal_point).Normalize();
    const auto yaw_angle = -delta.x  * 0.1f;
    const auto pitch_angle = -delta.y * 0.1f;
    const auto q =                               //
      Quaternion(yaw_angle, y_eye.Normalize()) * //
      Quaternion(pitch_angle, x_eye.Normalize());

    const auto new_eye_origin_reference = q.RotateVector(eye_origin_reference);

    const f32 eye_focal_pt_dist = m_focal_point.Distance(m_eye);
    const auto new_eye = m_focal_point + new_eye_origin_reference * eye_focal_pt_dist;

    auto new_view_direction = (m_focal_point - new_eye).Normalize();

    auto pos_angle = Geom::AngleBetween(new_view_direction, Vec3{ 0, 1, 0 });
    if (new_view_direction.y > 0 && pos_angle < 1)
      return;

    auto neg_angle = Geom::AngleBetween(new_view_direction, Vec3{ 0, -1, 0 });
    if (new_view_direction.y < 0 && neg_angle < 1)
      return;

    m_eye = new_eye;
  }

  if (m_can_pan)
  {
    const auto diff_x = x_eye * 0.01f * -delta.x;
    const auto diff_y = y_eye * 0.01f * delta.y;

    m_focal_point += diff_x + diff_y;
    m_eye += diff_x + diff_y;
  }
}

Mat4 EditorCamera::ViewProjection() const
{
  return m_projection_mat * m_view_mat;
}

void EditorCamera::MouseZoom(const f32 delta)
{
  const auto distance = m_focal_point.Distance(m_eye);
  const auto direction = (m_eye - m_focal_point).Normalize();
  m_eye = m_focal_point + direction * (distance + -delta * ZOOM_FACTOR);
  UpdateView();
}

void EditorCamera::UpdateView()
{
  auto up = Vec3{ 0, 1, 0 };
  m_view_mat = Transform::LookAt(m_eye, m_focal_point, up);
}

void EditorCamera::OnMouseScroll(MousePairData data)
{
  const f32 delta = std::get<1>(data) * 0.1f;
  MouseZoom(delta);
  UpdateView();
}

bool EditorCamera::OnMousePressed(KeyCode bt)
{
  if (bt == KeyCode::MOUSE_BT_LEFT)
  {
    m_mouse_init_pos = Input::GetMouseXY();
    m_can_rotate = true;
    return true;
  }

  if (bt == KeyCode::MOUSE_BT_MIDDLE)
  {
    m_mouse_init_pos = Input::GetMouseXY();
    m_can_pan = true;
    return true;
  }

  return false;
}

bool EditorCamera::OnMouseReleased(KeyCode bt)
{
  if (bt == KeyCode::MOUSE_BT_LEFT)
  {
    m_can_rotate = false;
    return true;
  }
  if (bt == KeyCode::MOUSE_BT_MIDDLE)
  {
    m_can_pan = false;
    return true;
  }
  return false;
}

void EditorCamera::UpdateAspectRatio(const Dimensions& viewport)
{
  m_aspect_ratio = f32(viewport.width) / f32(viewport.height);
  RecalculateProjection();
}

void EditorCamera::RecalculateProjection()
{
  GE_PROFILE;

  m_projection_mat = Transform::Perspective(m_field_of_view, m_aspect_ratio);
}
