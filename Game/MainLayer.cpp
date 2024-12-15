#include "MainLayer.hpp"

namespace
{
  constexpr auto START_EYE = GE::Vec3{ 0, 0, 12 };
  constexpr auto START_TARGET = GE::Vec3{ 0, 0, 0 };

  constexpr std::array PARTICLE_COLORS{
    GE::Color{ 0xfcf700, 0xaa },
    GE::Color{ 0xf59000, 0xaa },
    GE::Color{ 0xfa6707, 0xaa },
    GE::Color{ 0xe60607, 0xaa },
  };

  const std::vector<std::string_view> ASTEROIDS_MODELS{
    { "Assets/objs/1132 T-3 Durech.obj" }, { "Assets/objs/1277 T-2 Durech.obj" },
    { "Assets/objs/1958 TL1 Durech.obj" }, { "Assets/objs/1978 VB3 Durech.obj" },
    { "Assets/objs/1994 WM1 Durech.obj" }, { "Assets/objs/Athene Model 1.obj" },
    { "Assets/objs/Bustelli Durech.obj" }, { "Assets/objs/Fanale Model 1.obj" },
    { "Assets/objs/Gisela Model 1.obj" }
  };

  float GetWorldSize(float distanceFromCam, float fov)
  {
    const float tan_fovy = std::tan(GE::Transform::Deg2Rad(fov / 2.0f));
    const float world_size = 2 * tan_fovy * distanceFromCam;
    return world_size;
  }
}

ExplosionParticles::ExplosionParticles(f32 size, const GE::Vec3& pos) :
    m_size(size), m_position(pos), m_delta(0)
{
}

Particle ExplosionParticles::CreateParticle(GE::Ptr<GE::Scene> scene, u32 id)
{
  GE::Entity par = scene->CreateEntity("par_" + std::to_string(id));
  scene->AddComponent<GE::PrimitiveComponent>(par,
                                              GE::Cube{}.GetDrawable(),
                                              PARTICLE_COLORS.at(GE::Random::GenInt(0, 3)));
  scene->AddComponent<GE::TransformComponent>(par);
  auto& tc = scene->GetComponent<GE::TransformComponent>(par);
  tc.Rotation().x = GE::Random::GenFloat(-90.0f, 90.0f);
  tc.Rotation().y = GE::Random::GenFloat(-90.0f, 90.0f);
  tc.Rotation().z = GE::Random::GenFloat(-90.0f, 90.0f);
  tc.Scale().x = m_size;
  tc.Scale().y = m_size;
  tc.Scale().z = m_size;
  tc.Position() = m_position;

  const float min_velocity = -0.5f;
  const float max_velocity = +0.5f;
  [[maybe_unused]] const float min_acc = -0.005f;
  [[maybe_unused]] const float max_acc = 0.005f;

  const float vx = GE::Random::GenFloat(min_velocity, max_velocity);
  const float vy = GE::Random::GenFloat(min_velocity, max_velocity);
  const float vz = GE::Random::GenFloat(min_velocity, max_velocity);
  const float ax = 5 * vx; // GE::Random::GenFloat(min_acc, max_acc);
  const float ay = 5 * vy; // GE::Random::GenFloat(min_acc, max_acc);
  const float az = 5 * vz; // GE::Random::GenFloat(min_acc, max_acc);

  return { par, { ax, ay, az }, { vx, vy, vz }, m_position };
}

void ExplosionParticles::OnUpdate(GE::Ptr<GE::Scene> scene,
                                  GE::TimeStep ts,
                                  const std::vector<Particle>& particles)
{
  constexpr auto FRACTION = 0.125;
  m_size -= static_cast<f32>(ts.Secs() * FRACTION);
  m_delta += static_cast<f32>(ts.Secs());

  constexpr auto SIZE_MIN_LIMIT = 0.01f;
  if (m_size < SIZE_MIN_LIMIT)
  {
    m_done = true;
    for (auto& [ent, acc, vel, pos] : particles)
    {
      scene->EnqueueToDestroy(ent);
    }
    return;
  }

  for (auto& [ent, acc, vel, pos] : particles)
  {
    auto& tc = scene->GetComponent<GE::TransformComponent>(ent);
    tc.Scale().x = m_size;
    tc.Scale().y = m_size;
    tc.Scale().z = m_size;
    tc.Position() = tc.Position() + (vel * static_cast<f32>(m_delta)) +
                    (acc * static_cast<f32>(m_delta * m_delta)) / 2.0f;
  }
}

bool ExplosionParticles::HasDone()
{
  return m_done;
}

MainLayer::MainLayer() :
    GE::Layer("MainLayer"),
    m_scene(GE::Scene::Make("MainLayer")),
    m_box(0),
    m_camera(0),
    m_editor_camera(START_EYE, START_TARGET),
    m_aim(0),
    m_axis_x(0),
    m_axis_y(0)
{
  // GE::SceneSerializer{m_scene}.DeserializeFromFile("Assets/scenes/Scene_Learn.yaml");
}

MainLayer::~MainLayer() = default;

void MainLayer::OnAttach()
{
  m_scene->OnAttach();

  m_camera = m_scene->CreateEntity("Camera");
  m_scene->AddComponent<GE::CameraComponent>(m_camera, START_EYE, START_TARGET, true, false);
  auto& cam_comp = m_scene->GetComponent<GE::CameraComponent>(m_camera);
  cam_comp.GetCamera().SetFov(80);
  m_scene->SetActiveCamera(m_camera);

  auto amb_light = m_scene->CreateEntity("AmbientLight");
  m_scene->AddComponent<GE::AmbientLightComponent>(amb_light, GE::Colors::WHITE, 0.5f);

  auto light_src = m_scene->CreateEntity("Light");
  m_scene->AddComponent<GE::LightSourceComponent>(light_src,
                                                  GE::Colors::WHITE,
                                                  START_EYE,
                                                  10.0f,
                                                  true);

  m_aim = m_scene->CreateEntity("aim");
  m_scene->AddComponent<GE::PrimitiveComponent>(m_aim, GE::Cube{}.GetDrawable(), GE::Colors::WHITE);
  m_scene->AddComponent<GE::TransformComponent>(m_aim,
                                                GE::Vec3{},
                                                GE::Vec3{ 0.25f, 0.25f, 0.1f },
                                                GE::Vec3{ 0, 0, 45 });

  constexpr auto CUBES_COUNT = 10;
  m_cubes.reserve(CUBES_COUNT);
  for (i32 i = 0; i < CUBES_COUNT; i++)
  {
    GE::Entity cube = m_scene->CreateEntity("Cube_" + std::to_string(i));
    m_scene->AddComponent<GE::PrimitiveComponent>(
      cube,
      GE::Mesh{ ASTEROIDS_MODELS.at(GE::Random::GenInt(0, static_cast<u32>(ASTEROIDS_MODELS.size()) - 1)) }
        .GetDrawable(),
      GE::Colors::GRAY);
    const f32 cube_size = GE::Random::GenFloat(0.25f, 2.0f);
    m_scene->AddComponent<GE::TransformComponent>(cube,
                                                  GE::Vec3{},
                                                  GE::Vec3{ cube_size, cube_size, cube_size },
                                                  GE::Vec3{ GE::Random::GenFloat(0, 90),
                                                            GE::Random::GenFloat(0, 90),
                                                            GE::Random::GenFloat(0, 90) });
    auto dir = GE::Vec3{ GE::Random::GenFloat(-100.0f, 100.0f),
                         GE::Random::GenFloat(-100.0f, 100.0f),
                         /*GE::Random::GenFloat(-100.0f, 100.0f)*/ 0 };
    m_directions.emplace(cube, dir.Normalize());
    const auto& prim = m_scene->GetComponent<GE::PrimitiveComponent>(cube);
    m_colors.emplace(cube, prim.GetColor());
    m_cubes.push_back(cube);
  }

  const float world_size = GetWorldSize(START_EYE.z, cam_comp.GetCamera().GetFov());
  m_world_start = -world_size / 2.0f;
  m_world_end = world_size / 2.0f;
  m_world_top = world_size / 2.0f;
  m_world_bot = -world_size / 2.0f;
  m_axis_y = m_scene->CreateEntity("AxisY");
  m_scene->AddComponent<GE::PrimitiveComponent>(m_axis_y,
                                                GE::Cube{}.GetDrawable(),
                                                GE::Colors::WHITE);
  m_scene->AddComponent<GE::TransformComponent>(m_axis_y,
                                                GE::Vec3{ 0, 0, 0 },
                                                GE::Vec3{ 0.05f, world_size, 0.1f });
  m_axis_x = m_scene->CreateEntity("AxisX");
  m_scene->AddComponent<GE::PrimitiveComponent>(m_axis_x,
                                                GE::Cube{}.GetDrawable(),
                                                GE::Colors::WHITE);
  m_scene->AddComponent<GE::TransformComponent>(m_axis_x,
                                                GE::Vec3{ 0, 0, 0 },
                                                GE::Vec3{ world_size, 0.05f, 0.1f });
}

void MainLayer::OnUpdate(GE::TimeStep step)
{
  if (m_pause)
    return;

  Layer::OnUpdate(step);

  GE::Renderer::SetClearColor(GE::Color{ 0x111111 }.ToVec4());
  GE::Renderer::Clear();

  const GE::Vec2 mouse_pos = GE::Input::GetMouseXY();
  GE::TransformComponent& aim_tc = m_scene->GetComponent<GE::TransformComponent>(m_aim);
  if (mouse_pos.x >= 0 && mouse_pos.x <= 480 && mouse_pos.y >= 0 && mouse_pos.y <= 480)
  {
    const float& screen_x = mouse_pos.x;
    const float& screen_y = 480 - mouse_pos.y;
    const float& world_width = (m_world_end - m_world_start);
    const float& world_height = (m_world_top - m_world_bot);
    const float& world_x = m_world_start + ((screen_x / 480) * world_width);
    const float& world_y = m_world_bot + ((screen_y / 480) * world_height);
    aim_tc.Position().x = world_x;
    aim_tc.Position().y = world_y;
    m_scene->GetComponent<GE::TransformComponent>(m_axis_x).Position().y = world_y;
    m_scene->GetComponent<GE::TransformComponent>(m_axis_y).Position().x = world_x;
  }

  for (GE::Entity& ent : m_cubes)
  {
    GE::TransformComponent& tc = m_scene->GetComponent<GE::TransformComponent>(ent);
    GE::PrimitiveComponent& pri = m_scene->GetComponent<GE::PrimitiveComponent>(ent);
    auto& pos = tc.Position();
    pos += m_directions.at(ent) * static_cast<f32>(step.Secs()) * 2.0f;

    {
      const float dist = pos.Distance(aim_tc.Position());
      const bool hit = dist < (tc.Scale().x / 2.0f);
      pri.SetColor(hit ? GE::Colors::RED : m_colors.at(ent));
      if (GE::Input::IsMouseButtonPressed(GE::KeyCode::MOUSE_BT_LEFT) && hit)
      {
        GE_INFO("Cube hit: {}", ent.handle)
        if (!m_particle_systems.contains(ent))
        {
          // m_particle_systems.emplace(ent, ParticlesSystem{ m_scene, pos, tc.Scale().x / 3.0f });
          m_particle_systems.emplace(
            ent,
            ParticlesSystem{ m_scene, 1000, GE::MakeRef<ExplosionParticles>(tc.Scale().x / 3.0f, pos) });
          m_scene->EnqueueToDestroy(ent);
          std::erase(m_cubes, ent);
          break;
        }
      }
    }

    constexpr auto box_size = 0.f;
    if (pos.x >= (m_world_end - box_size) || pos.x <= (m_world_start + box_size))
    {
      GE::Vec3& dir = m_directions.at(ent);
      dir.x = -dir.x;
      if (pos.x >= (m_world_end - box_size))
        pos.x = m_world_end;
      if (pos.x <= (m_world_start + box_size))
        pos.x = m_world_start;
    }
    if (pos.y >= (m_world_top - box_size) || pos.y <= (m_world_bot + box_size))
    {
      GE::Vec3& dir = m_directions.at(ent);
      dir.y = -dir.y;
      if (pos.y >= (m_world_top - box_size))
        pos.y = m_world_top;
      if (pos.y <= (m_world_bot + box_size))
        pos.y = m_world_bot;
    }
  }

  for (auto& [_, par] : m_particle_systems)
    par.OnUpdate(step);

  if (m_in_editor)
  {
    m_editor_camera.OnUpdate(step);
    m_scene->OnUpdateEditor(step, m_editor_camera);
  }
  else
  {
    m_scene->OnUpdate(step);
  }

  std::erase_if(m_particle_systems, [](auto&& par) { return par.second.Ended(); });
}

void MainLayer::OnEvent(GE::Event& event)
{
  Layer::OnEvent(event);
  event                              //
    .When(GE::EvType::WINDOW_RESIZE) //
    .Then(
      [&](const GE::EvData& data)
      {
        const auto& resize_data = *std::get_if<GE::WindowResizeData>(&data);
        GE::Dimensions vp{ resize_data.first, resize_data.second };
        m_scene->OnViewportResize(vp);
        GE::Renderer::SetViewport(0, 0, vp);
      });

  if (m_in_editor)
    m_editor_camera.OnEvent(event);

  event.When(GE::EvType::KEY_PRESS)
    .Then(
      [&](const GE::EvData& data)
      {
        const auto& key_data = *std::get_if<GE::KeyCode>(&data);
        if (key_data == GE::KeyCode::TAB)
          m_in_editor = !m_in_editor;
        else if (key_data == GE::KeyCode::P)
          m_pause = !m_pause;
      });
}
