#include "grapengine.hpp"

#include <gtest/gtest.h>

#if defined(GE_CLANG_COMPILER)
  #pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

TEST(SceneSerializer, Serialize)
{
  std::filesystem::path file_path = std::filesystem::temp_directory_path() / "test.yaml";
  std::cout << file_path << std::endl;
  GE::Ptr<GE::Scene> scene = GE::Scene::Make("Untitled");
  [[maybe_unused]] const GE::Entity& empty_ent = scene->CreateEntity("My tag");
  const GE::Entity& full_ent = scene->CreateEntity("My model");
  scene->AddComponent<GE::TransformComponent>(full_ent);
  [[maybe_unused]] const GE::TransformComponent& transf_comp =
    scene->GetComponent<GE::TransformComponent>(full_ent);
  auto tex_slot = scene->RegisterTexture("path-to-texture");
  scene->AddComponent<GE::PrimitiveComponent>(full_ent,
                                              GE::Cube().GetDrawable(),
                                              GE::Colors::ORANGE,
                                              tex_slot);
  [[maybe_unused]] const GE::PrimitiveComponent& primitive_comp =
    scene->GetComponent<GE::PrimitiveComponent>(full_ent);
  scene->AddComponent<GE::CameraComponent>(full_ent, GE::Vec3{}, GE::Vec3{}, true, true);
  [[maybe_unused]] const GE::CameraComponent& camera_comp =
    scene->GetComponent<GE::CameraComponent>(full_ent);
  scene->SetActiveCamera(full_ent);
  scene->AddComponent<GE::AmbientLightComponent>(full_ent, GE::Colors::MAGENTA, 0.75f);
  [[maybe_unused]] const GE::AmbientLightComponent& ambient_comp =
    scene->GetComponent<GE::AmbientLightComponent>(full_ent);
  scene->AddComponent<GE::LightSourceComponent>(full_ent,
                                                GE::Colors::GREEN,
                                                GE::Vec3{ 1, 2, 3 },
                                                5.5f,
                                                true);
  [[maybe_unused]] const GE::LightSourceComponent& light_comp =
    scene->GetComponent<GE::LightSourceComponent>(full_ent);
  const std::set<GE::Entity>& entities = scene->GetEntitiesSet();
  GE::SceneSerializer serializer{ scene };
  serializer.SerializeToFile(file_path);

  ASSERT_TRUE(std::filesystem::exists(file_path));

  auto scene_2_read = GE::Scene::Make("Untitled-2");
  GE::SceneSerializer serializer2{ scene_2_read };
  serializer2.DeserializeFromFile(file_path);

  ASSERT_EQ(*scene, *scene_2_read);

  scene_2_read->OnEachEntity(
    [&](GE::Entity entity)
    {
      ASSERT_TRUE(entities.contains(entity));

      if (entity == full_ent)
      {
        ASSERT_TRUE(scene_2_read->HasComponent<GE::PrimitiveComponent>(entity));
        ASSERT_TRUE(scene_2_read->HasComponent<GE::TransformComponent>(entity));
        ASSERT_TRUE(scene_2_read->HasComponent<GE::CameraComponent>(entity));
        ASSERT_TRUE(scene_2_read->HasComponent<GE::AmbientLightComponent>(entity));
        ASSERT_TRUE(scene_2_read->HasComponent<GE::LightSourceComponent>(entity));
      }
    });
}

// TEST(SceneWriter, Read)
// {
//
// }
