//
// Created by vinicius on 26-Oct-24.
//

#include "ParticlesSystem.hpp"

ParticlesSystem::ParticlesSystem(GE::Ptr<GE::Scene> scene,
                                 u32 count,
                                 GE::Ptr<IParticlesController> controller) :
    m_scene(scene), m_particles(), m_controller(controller)
{
  m_particles.reserve(count);
  for (u32 i = 0; i < count; ++i)
  {
    m_particles.emplace_back(controller->CreateParticle(m_scene, i));
  }
}

/*ParticlesSystem::ParticlesSystem(GE::Ptr<GE::Scene> scene, const GE::Vec3& pos, const f32 size) :
    m_scene(scene), m_particles({}), m_position(pos)
{
  constexpr auto PARTICLES_COUNT = 100;
  m_particles.reserve(PARTICLES_COUNT);
  for (i32 i = 0; i < PARTICLES_COUNT; i++)
  {
    GE::Entity par = m_scene->CreateEntity("par_" + std::to_string(i));
    /*m_scene->AddComponent<GE::PrimitiveComponent>(par,
                                                  GE::Cube{}.GetDrawable(),
                                                  PARTICLE_COLORS.at(GE::Random::GenInt(0, 3)));#1#
    m_scene->AddComponent<GE::TransformComponent>(par);
    auto& tc = m_scene->GetComponent<GE::TransformComponent>(par);
    tc.Rotation().x = GE::Random::GenFloat(-90.0f, 90.0f);
    tc.Rotation().y = GE::Random::GenFloat(-90.0f, 90.0f);
    tc.Rotation().z = GE::Random::GenFloat(-90.0f, 90.0f);
    tc.Scale().x = size;
    tc.Scale().y = size;
    tc.Scale().z = size;

    const float min_velocity = -0.5f;
    const float max_velocity = +0.5f;
    const float min_acc = -0.005f;
    const float max_acc = 0.005f;

    const float vx = GE::Random::GenFloat(min_velocity, max_velocity);
    const float vy = GE::Random::GenFloat(min_velocity, max_velocity);
    const float vz = GE::Random::GenFloat(min_velocity, max_velocity);
    const float ax = 5 * vx; // GE::Random::GenFloat(min_acc, max_acc);
    const float ay = 5 * vy; // GE::Random::GenFloat(min_acc, max_acc);
    const float az = 5 * vz; // GE::Random::GenFloat(min_acc, max_acc);
    m_particles.emplace_back(par, GE::Vec3{ ax, ay, az }, GE::Vec3{ vx, vy, vz }, GE::Vec3{});
  }

  Start(size);
}*/

void ParticlesSystem::OnUpdate(GE::TimeStep delta)
{
  if (m_controller->HasDone())
    return;

  m_controller->OnUpdate(m_scene, delta, m_particles);
  // for (u32 i = 0; i < m_particles.size(); ++i)
  // {
  //   m_controller->OnUpdate(m_scene, delta, m_particles.at(i));
  // }
}

/*void ParticlesSystem::OnUpdate(GE::TimeStep deltaTime)
{

  constexpr auto FRACTION = 0.125;
  m_size -= static_cast<f32>(deltaTime.Secs() * FRACTION);
  m_delta += deltaTime.Secs();

  constexpr auto SIZE_MIN_LIMIT = 0.01f;
  if (m_size < SIZE_MIN_LIMIT)
  {
    m_started = false;
    for (auto& [ent, acc, vel, pos] : m_particles)
    {
      m_scene->EnqueueToDestroy(ent);
    }
    return;
  }

  for (auto& [ent, acc, vel, pos] : m_particles)
  {
    auto& tc = m_scene->GetComponent<GE::TransformComponent>(ent);
    tc.Scale().x = m_size;
    tc.Scale().y = m_size;
    tc.Scale().z = m_size;
    tc.Position() = tc.Position() + (vel * static_cast<f32>(m_delta)) +
                    (acc * static_cast<f32>(m_delta * m_delta)) / 2.0f;
  }
}*/

bool ParticlesSystem::Ended() const
{
  return m_controller->HasDone();
}

// void ParticlesSystem::Start()
// {
//   if (m_started)
//     return;
//
//   m_started = true;
// }

/*void ParticlesSystem::Start(const f32 size)
{
  if (m_started)
    return;
  for (auto& [ent, acc, vel, pos] : m_particles)
  {
    m_scene->EnableEntity(ent);
    auto& tc = m_scene->GetComponent<GE::TransformComponent>(ent);
    tc.Scale() = GE::Vec3{ 0.5f, 0.5f, 0.5f };
    tc.Position() = m_position;
  }
  m_started = true;
  m_size = size;
  m_delta = 0;
}*/