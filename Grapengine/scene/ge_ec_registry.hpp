#ifndef GRAPENGINE_GE_EC_REGISTRY_HPP
#define GRAPENGINE_GE_EC_REGISTRY_HPP

#include "ge_components.hpp"
#include "ge_entity.hpp"
#include "profiling/ge_profiler.hpp"

namespace GE
{
  class ECRegistry
  {
  public:
    /**
     * Create and return an entity with a unique id
     * @return empty entity
     */
    Entity Create();

    void Push(Entity ent);

    void Destroy(Opt<Entity> ent);

    /**
     * Add a component to registry and associate to its entity
     * @tparam Component type of component
     * @tparam Args argument list to construct the component
     * @param ent entity that is associated of component
     * @param args argument list to construct the component
     */
    template <typename Component, typename... Args>
    void AddComponent(const Entity& ent, Args... args)
    {
      GE_PROFILE;
      GE_ASSERT(!Has<Component>(ent), "Entity already has this component!");

      /*VarComponent& added =*/
      m_components[ent].emplace_back(std::in_place_type<Component>, std::forward<Args>(args)...);
      m_entities_with_components[ent].insert(typeid(Component).hash_code());

    }

    template <typename Component>
    void PushComponent(const Entity& ent, Component&& component)
    {
      GE_PROFILE;
      GE_ASSERT(!Has<Component>(ent), "Entity already has this component!");

      m_components[ent].emplace_back(component);
      m_entities_with_components[ent].insert(typeid(Component).hash_code());
    }

    template <typename Component>
    void RemoveComponent(Entity entity)
    {
      m_entities_with_components[entity].erase(typeid(Component).hash_code());
      auto& ent_components = m_components[entity];
      std::erase_if(ent_components,
                    [&](const VarComponent& c) { return std::holds_alternative<Component>(c); });
    }

    /**
     * Retrieve the component of type Comp associated with entity
     * @tparam Component component type to be got
     * @param ent entity that is associated with component
     * @return associated component
     */
    template <typename Component>
    [[nodiscard]] Component& GetComponent(const Entity& ent)
    {
      GE_PROFILE;
      GE_ASSERT(Has<Component>(ent), "Entity does not have this component!");

      auto found = std::ranges::find_if(m_components.at(ent),
                                        [&](auto&& anyComp)
                                        { return std::holds_alternative<Component>(anyComp); });
      VarComponent& var_component = *found;
      Component* alc = std::get_if<Component>(&var_component);
      return *alc;
    }

    /**
     * Retrieve the component of type Comp associated with entity
     * @tparam Component component type to be got
     * @param ent entity that is associated with component
     * @return associated component
     */
    template <typename Component>
    [[nodiscard]] const Component& GetComponent(const Entity& ent) const
    {
      GE_PROFILE;
      GE_ASSERT(Has<Component>(ent), "Entity does not have this component!");

      auto found = std::ranges::find_if(m_components.at(ent),
                                        [&](auto&& anyComp)
                                        { return std::holds_alternative<Component>(anyComp); });
      const VarComponent& var_component = *found;
      const Component* alc = std::get_if<Component>(&var_component);
      return *alc;
    }

    [[nodiscard]] const std::vector<VarComponent>& GetComponents(const Entity& ent) const;

    [[nodiscard]] const std::list<Entity>& GetEntitiesList() const;
    [[nodiscard]] const std::set<Entity>& GetEntitiesSet() const;

    /**
     * Verify if associated component exist with given entity
     * @tparam Comp component type to be verified
     * @param ent entity associated
     * @return true if entity has the component
     */
    template <typename Comp>
    [[nodiscard]] bool Has(const Entity& ent) const
    {
      GE_PROFILE;
      if (!m_entities.contains(ent))
        return false;

      if (!m_components.contains(ent))
        return false;

      if (m_components.at(ent).empty())
        return false;

      auto found = std::ranges::find_if(m_entities_with_components.at(ent),
                                        [&](u64 compHashCode)
                                        { return typeid(Comp).hash_code() == compHashCode; });
      return found != m_entities_with_components.at(ent).end();
    }

    /**
     * Retrieve list of entities that has all passed components
     * @tparam Comps list of components used to query the entities
     * @return set of entities that has all given components in common
     */
    template <typename... Comps>
    [[nodiscard]] std::vector<Entity> Group() const
    {
      GE_PROFILE;

      // Get all component types id required
      std::set<u64> comps;
      (..., [&] { comps.insert(typeid(Comps).hash_code()); }());

      std::vector<Entity> entities;
      for (const auto& [ent, its_comps] : m_entities_with_components)
      {
        if (m_entities_disabled.contains(ent))
          continue;

        if (std::ranges::includes(its_comps, comps))
          entities.push_back(ent);
      }
      return entities;
    }

    void OnEach(const std::function<void(Entity)>& action);
    void OnEach(const std::function<void(Entity)>& action) const;

    void DisableEntity(Entity ent);
    void EnableEntity(Entity ent);

    [[nodiscard]] bool operator==(const ECRegistry& other) const;

  private:
    std::set<Entity> m_entities;
    std::set<Entity> m_entities_disabled;
    std::list<Entity> m_entities_sorted_list;
    std::map<Entity, std::vector<VarComponent>> m_components;
    std::map<Entity, std::set<u64>> m_entities_with_components;
    std::set<Entity> m_destroy_queue;
  };
} // GE

#endif // GRAPENGINE_GE_EC_REGISTRY_HPP
