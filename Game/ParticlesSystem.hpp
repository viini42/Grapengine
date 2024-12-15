#ifndef PARTICLESSYSTEM_HPP
#define PARTICLESSYSTEM_HPP

#include <grapengine.hpp>

struct Particle
{
  GE::Entity entity;
  GE::Vec3 acceleration;
  GE::Vec3 velocity;
  GE::Vec3 position;

  Particle(const GE::Entity& e,
           const GE::Vec3& acc,
           const GE::Vec3& vel,
           const GE::Vec3& pos) :
      entity(e), acceleration(acc), velocity(vel), position(pos)
  {
  }
};

using ParticleCreator = std::function<Particle(GE::Ptr<GE::Scene>, u32)>;

class IParticlesController
{
public:
  virtual ~IParticlesController() = default;

  virtual Particle CreateParticle(GE::Ptr<GE::Scene> scene, u32 id) = 0;

  virtual void OnUpdate(GE::Ptr<GE::Scene> scene, GE::TimeStep ts, const std::vector<Particle>& particles) = 0;

  virtual bool HasDone() = 0;
};

class ParticlesSystem
{
public:
  ParticlesSystem(GE::Ptr<GE::Scene> scene, u32 count, GE::Ptr<IParticlesController> controller);
  // ParticlesSystem(GE::Ptr<GE::Scene> scene, const GE::Vec3& pos, const f32 size);

  void OnUpdate(GE::TimeStep delta);

  bool Ended() const;

  // void Start();

private:
  GE::Ptr<GE::Scene> m_scene;
  std::vector<Particle> m_particles;
  GE::Ptr<IParticlesController> m_controller;
};

#endif // PARTICLESSYSTEM_HPP
