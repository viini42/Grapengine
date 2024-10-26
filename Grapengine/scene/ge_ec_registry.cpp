#include "scene/ge_ec_registry.hpp"

#include "profiling/ge_profiler.hpp"
#include "utils/ge_random.hpp"

using namespace GE;

Entity ECRegistry::Create()
{
  GE_PROFILE;
  auto id = Random::GenInt();
  Entity e{ id };
  auto [_, ok] = m_entities.insert(e);
  GE_ASSERT(ok, "Random id collision");
  m_entities_sorted_list.push_back(e);
  return e;
}

void ECRegistry::Push(Entity ent)
{
  auto [it, inserted] = m_entities.insert(ent);
  GE_ASSERT_OR_RETURN_VOID(inserted, "Entity already exists");
  m_entities_sorted_list.push_back(ent);
}

void ECRegistry::OnEach(const std::function<void(Entity)>& action) const
{
  std::ranges::for_each(m_entities_sorted_list, action);
}

void ECRegistry::DisableEntity(Entity ent)
{
  GE_PROFILE;
  if (!m_entities.contains(ent))
    return;

  if (m_entities_disabled.contains(ent))
    return;

  m_entities_disabled.insert(m_entities.extract(ent));
}

void ECRegistry::EnableEntity(Entity ent)
{
  GE_PROFILE;
  if (m_entities.contains(ent))
    return;

  if (!m_entities_disabled.contains(ent))
    return;

  m_entities.insert(m_entities_disabled.extract(ent));
}

bool ECRegistry::operator==(const ECRegistry& other) const
{
  const bool entities_equal = m_entities == other.m_entities;
  const bool components_equal = m_entities_with_components == other.m_entities_with_components;
  return entities_equal && components_equal;
}

void ECRegistry::OnEach(const std::function<void(Entity)>& action)
{
  std::ranges::for_each(m_entities_sorted_list, action);
}

void ECRegistry::Destroy(Opt<Entity> ent)
{
  if (!ent)
    return;

  m_entities.erase(ent.value());
  std::erase(m_entities_sorted_list, ent.value());
  m_components.erase(ent.value());
  m_entities_with_components.erase(ent.value());
}

const std::vector<VarComponent>& ECRegistry::GetComponents(const Entity& ent) const
{
  GE_ASSERT(m_components.contains(ent), "Entity not found!");

  return m_components.at(ent);
}

const std::list<Entity>& ECRegistry::GetEntitiesList() const
{
  return m_entities_sorted_list;
}

const std::set<Entity>& ECRegistry::GetEntitiesSet() const
{
  return m_entities;
}
