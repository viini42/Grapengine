#include "CamController.hpp"

namespace
{
  const auto& MoveKeys()
  {
    static const std::set<KeyCode> MOVE_KEYS{ KeyCode::A, KeyCode::D, KeyCode::W,
                                              KeyCode::S, KeyCode::E, KeyCode::Q };
    return MOVE_KEYS;
  }

}

CamController::CamController(Entity e, Scene& s) : ScriptableEntity(e, s) {}

void CamController::OnCreate() {}

void CamController::OnDestroy() {}

void CamController::OnUpdate(TimeStep ts)
{
  if (std::ranges::none_of(MoveKeys(), [](auto&& k) { return Input::IsKeyPressed(k); }))
    return;

  constexpr f32 speed = 0.01f;
  const f32 incr = speed * static_cast<f32>(ts.Secs());

  auto& camera_comp = m_scene.GetComponent<CameraComponent>(m_entity);
  if (!camera_comp.IsActive())
    return;

  auto cam_pos = camera_comp.GetCamera().GetPosition();
  auto tar_pos = camera_comp.GetCamera().GetTarget();

  if (Input::IsKeyPressed(KeyCode::A))
  {
    cam_pos.x -= incr;
    tar_pos.x -= incr;
  }
  else if (Input::IsKeyPressed(KeyCode::D))
  {
    cam_pos.x += incr;
    tar_pos.x += incr;
  }
  else if (Input::IsKeyPressed(KeyCode::W))
  {
    cam_pos.y += incr;
    tar_pos.y += incr;
  }
  else if (Input::IsKeyPressed(KeyCode::S))
  {
    cam_pos.y -= incr;
    tar_pos.y -= incr;
  }
  else if (Input::IsKeyPressed(KeyCode::E))
  {
    cam_pos.z -= incr;
    tar_pos.z -= incr;
  }
  else if (Input::IsKeyPressed(KeyCode::Q))
  {
    cam_pos.z += incr;
    tar_pos.z += incr;
  }

  camera_comp.GetCamera().SetView(cam_pos, tar_pos);
}

CamController::~CamController() = default;
