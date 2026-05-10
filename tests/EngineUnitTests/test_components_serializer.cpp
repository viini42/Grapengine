#include "grapengine.hpp"
#include "serializer/ge_components_serializer.hpp"

#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>

#if defined(GE_CLANG_COMPILER)
  #pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

namespace
{
  void InitTestEmmiter(YAML::Emitter& e)
  {
    e << YAML::BeginMap;
    e << YAML::Key << "Test" << YAML::Value << "";
  }

  std::string GetSerialized(const GE::VarComponent& c)
  {
    YAML::Emitter e;
    InitTestEmmiter(e);
    std::visit<void>(GE::ComponentSerializer(e), c);
    e << YAML::EndMap;
    return e.c_str();
  }
}

TEST(Components, Tag)
{
  // Given component
  GE::VarComponent comp = GE::TagComponent{ "Lorem Ipsum" };
  const GE::TagComponent& tag = *std::get_if<GE::TagComponent>(&comp);

  // When serialized
  auto serialized = GetSerialized(comp);

  // Then deserialized
  YAML::Node node = YAML::Load(serialized);
  GE::ComponentDeserializer des{ node };
  auto comp_des = des.GetTag();
  ASSERT_TRUE(comp_des);
  ASSERT_EQ(tag, comp_des);
}

TEST(Components, Transform)
{
  // Given component
  GE::VarComponent comp = GE::TransformComponent{ { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
  const auto& transform = *std::get_if<GE::TransformComponent>(&comp);

  // When serialized
  auto serialized = GetSerialized(comp);

  // Then deserialized
  YAML::Node node = YAML::Load(serialized);
  GE::ComponentDeserializer des{ node };
  auto comp_des = des.GetTransform();
  ASSERT_TRUE(comp_des);
  ASSERT_EQ(transform, comp_des);
}

TEST(Components, Primitive)
{
  // Given component
  GE::VertexStruct vs{ GE::Vec3{ 1, 2, 3 },
                       GE::Vec2{ 4, 5 },
                       GE::Vec4{ 6, 7, 8, 9 },
                       GE::Vec3{ 9, 10, 11 },
                       0 };
  GE::VerticesData vd{ { vs } };
  GE::Drawable d{ vd, { 0 } };
  GE::VarComponent comp = GE::PrimitiveComponent{ d, GE::Colors::RED, 42 };
  const auto& primitive = *std::get_if<GE::PrimitiveComponent>(&comp);

  // When serialized
  auto serialized = GetSerialized(comp);

  // Then deserialized
  YAML::Node node = YAML::Load(serialized);
  GE::ComponentDeserializer des{ node };
  auto comp_des = des.GetPrimitive();
  ASSERT_TRUE(comp_des);
  ASSERT_EQ(primitive, comp_des);
}

TEST(Components, Camera)
{
  // Given component
  GE::VarComponent comp = GE::CameraComponent{ { 0, 1, 2 }, { 3, 4, 5 }, true, false };
  const auto& camera = *std::get_if<GE::CameraComponent>(&comp);

  // When serialized
  auto serialized = GetSerialized(comp);

  // Then deserialized
  YAML::Node node = YAML::Load(serialized);
  GE::ComponentDeserializer des{ node };
  auto comp_des = des.GetCamera();
  ASSERT_TRUE(comp_des);
  ASSERT_EQ(camera, comp_des);
}

TEST(Components, AmbientLight)
{
  // Given component
  GE::VarComponent comp = GE::AmbientLightComponent{ GE::Colors::BLUE, 0.3f, false };
  const auto& amb_light = *std::get_if<GE::AmbientLightComponent>(&comp);

  // When serialized
  auto serialized = GetSerialized(comp);

  // Then deserialized
  YAML::Node node = YAML::Load(serialized);
  GE::ComponentDeserializer des{ node };
  auto comp_des = des.GetAmbientLight();
  ASSERT_TRUE(comp_des);
  ASSERT_EQ(amb_light, comp_des);
}

TEST(Components, LightSource)
{
  // Given component
  GE::VarComponent comp =
    GE::LightSourceComponent{ GE::Colors::BLUE, GE::Vec3{ 0, 1, 2 }, 0.35f, true };
  const auto& light_src = *std::get_if<GE::LightSourceComponent>(&comp);

  // When serialized
  auto serialized = GetSerialized(comp);

  // Then deserialized
  YAML::Node node = YAML::Load(serialized);
  GE::ComponentDeserializer des{ node };
  auto comp_des = des.GetLightSource();
  ASSERT_TRUE(comp_des);
  ASSERT_EQ(light_src, comp_des);
}
