#ifndef GRAPENGINE_MAINLAYER_HPP
#define GRAPENGINE_MAINLAYER_HPP

#include "ParticlesSystem.hpp"

#include <grapengine.hpp>

class ExplosionParticles final : public IParticlesController
{
public:
  ExplosionParticles(f32 size, const GE::Vec3& pos);

  ~ExplosionParticles() override = default;
  Particle CreateParticle(GE::Ptr<GE::Scene> scene, u32 id) override;
  void OnUpdate(GE::Ptr<GE::Scene> scene,
                GE::TimeStep ts,
                const std::vector<Particle>& particles) override;
  bool HasDone() override;

private:
  f32 m_size;
  GE::Vec3 m_position;
  f32 m_delta;
  bool m_done = false;
};

class MainLayer final : public GE::Layer
{
public:
  MainLayer();

  ~MainLayer() override;
  void OnAttach() override;
  void OnUpdate(GE::TimeStep step) override;
  void OnEvent(GE::Event& event) override;

private:
  Particle CreateParticle(GE::Ptr<GE::Scene> scene, u32 id, const GE::Vec3& pos, f32 size);

  void OnParticleUpdate(GE::Ptr<GE::Scene> scene, GE::TimeStep step);

  GE::Ptr<GE::Scene> m_scene;
  GE::Entity m_box;
  std::vector<GE::Entity> m_cubes;
  std::map<GE::Entity, GE::Vec3> m_directions;
  std::map<GE::Entity, GE::Color> m_colors;
  GE::Entity m_camera;
  GE::EditorCamera m_editor_camera;
  bool m_in_editor = false;
  bool m_pause = false;
  GE::Entity m_aim;
  GE::Entity m_axis_x;
  GE::Entity m_axis_y;
  float m_world_start{};
  float m_world_end{};
  float m_world_top{};
  float m_world_bot{};
  std::map<GE::Entity, ParticlesSystem> m_particle_systems;
};

#endif // GRAPENGINE_MAINLAYER_HPP
