#include "ge_scene_serializer.hpp"

#include "drawables/ge_mesh.hpp"
#include "ge_components_serializer.hpp"
#include "ge_serializer_constants.hpp"
#include "ge_textures_registry_serializer.hpp"
#include "utils/ge_io.hpp"

#include <yaml-cpp/yaml.h>

using namespace GE;

SceneSerializer::SceneSerializer(const Ptr<Scene>& scene) : m_scene(scene) {}

std::string SceneSerializer::Serialize() const
{
  GE_ASSERT(m_scene != nullptr, "Invalid scene");

  YAML::Emitter out;
  out.SetIndent(0);
  out << YAML::BeginMap; // Main node
  out << YAML::Key << Constants::SERIALIZE_TITLE;
  out << YAML::BeginMap; // Scene node
  out << YAML::Key << Fields::NAME;
  out << YAML::Value << m_scene->GetName();
  out << YAML::Key << Fields::ENTITIES;
  out << YAML::Value << YAML::BeginSeq; // Entities

  m_scene->OnEachEntity(
    [&](Entity ent)
    {
      out << YAML::BeginMap; // Entity node
      out << YAML::Key << Fields::ENTITY_ID << YAML::Value << ent.handle;
      const std::vector<VarComponent>& comps = m_scene->GetComponents(ent);
      for (const VarComponent& c : comps)
      {
        std::visit<void>(ComponentSerializer(out), c);
        GE_ASSERT_NO_MSG(out.good());
      }
      out << YAML::EndMap; // Entity node
    });

  out << YAML::EndSeq; // Entities

  out << YAML::Key << Fields::TEXTURES; // Textures
  out << YAML::Value;

  TexturesRegistrySerializer{ m_scene->GetTextureRegistry() }.Serialize(out);

  out << YAML::EndMap; // Textures

  out << YAML::EndMap; // Scene node
  out << YAML::EndMap; // Main node

  return out.c_str();
}

void SceneSerializer::SerializeToFile(const std::filesystem::path& path) const
{
  GE_PROFILE;
  GE_ASSERT(std::filesystem::exists(path.parent_path()), "Directory does not exist");
  GE_INFO("Serializing scene to '{}'", path.string())

  std::string scene_serialized = Serialize();
  std::ofstream out(path);
  out << scene_serialized;
}

void SceneSerializer::Deserialize(const std::string& sceneString)
{
  GE_ASSERT(m_scene != nullptr, "Invalid scene");
  YAML::Node config = YAML::Load(sceneString);

  YAML::Node root_node = config[Constants::SERIALIZE_TITLE];
  if (!root_node)
    return;

  YAML::Node scene_name_node = root_node[Fields::NAME];
  const std::string scene_name = scene_name_node.as<std::string>();
  m_scene->SetName(scene_name);

  YAML::Node entities_node = root_node[Fields::ENTITIES];
  for (const auto& ent_node : entities_node)
  {
    u32 ent_handle = ent_node[Fields::ENTITY_ID].as<u32>();
    Entity ent{ ent_handle };
    m_scene->PushEntity(ent);

    auto deserializer = ComponentDeserializer(ent_node);
    m_scene->PushComponent<TagComponent>(ent, deserializer.GetTag());
    m_scene->PushComponent<PrimitiveComponent>(ent, deserializer.GetPrimitive());
    m_scene->PushComponent<TransformComponent>(ent, deserializer.GetTransform());
    auto camera = deserializer.GetCamera();
    if (camera)
    {
      m_scene->PushComponent<CameraComponent>(ent, camera);
      if (camera->IsActive())
        m_scene->SetActiveCamera(ent);
    }
    m_scene->PushComponent<AmbientLightComponent>(ent, deserializer.GetAmbientLight());
    m_scene->PushComponent<LightSourceComponent>(ent, deserializer.GetLightSource());
  }

  YAML::Node texture_node = root_node[Fields::TEXTURES];
  if (texture_node.IsDefined())
    TexturesRegistryDeserializer{m_scene->GetTextureRegistry()}.Deserialize(texture_node);
}

void SceneSerializer::DeserializeFromFile(const std::filesystem::path& path)
{
  GE_PROFILE;
  GE_ASSERT(m_scene != nullptr, "Invalid scene");
  GE_ASSERT(std::filesystem::exists(path), "File not found: {}", path.string());
  GE_INFO("Deserializing scene from '{}'", path.string())

  std::ifstream file(path);
  if (!file.is_open())
    return;

  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string str = buffer.str();
  Deserialize(str);
}
