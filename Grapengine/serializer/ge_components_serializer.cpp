#include "ge_components_serializer.hpp"

#include "ge_serializer_constants.hpp"

using namespace GE;

namespace
{
  YAML::Emitter& operator<<(YAML::Emitter& e, const Vec2& v)
  {
    e << YAML::Flow;
    e << YAML::BeginSeq;
    e << v.x << v.y;
    e << YAML::EndSeq;
    GE_ASSERT_NO_MSG(e.good());
    return e;
  }
  YAML::Emitter& operator<<(YAML::Emitter& e, const Vec3& v)
  {
    e << YAML::Flow;
    e << YAML::BeginSeq;
    e << v.x << v.y << v.z;
    e << YAML::EndSeq;
    GE_ASSERT_NO_MSG(e.good());
    return e;
  }
  YAML::Emitter& operator<<(YAML::Emitter& e, const Vec4& v)
  {
    e << YAML::Flow;
    e << YAML::BeginSeq;
    e << v.x0 << v.x1 << v.x2 << v.x3;
    e << YAML::EndSeq;
    GE_ASSERT_NO_MSG(e.good());
    return e;
  }
  YAML::Emitter& operator<<(YAML::Emitter& e, const Color& c)
  {
    e << YAML::BeginMap;
    e << YAML::Key << "RGB" << YAML::Value << YAML::Hex << c.HexRGB();
    e << YAML::Key << "Alpha" << YAML::Value << YAML::Hex << u32(c.A());
    e << YAML::EndMap;
    GE_ASSERT_NO_MSG(e.good());
    return e;
  }
  YAML::Emitter& operator<<(YAML::Emitter& e, const Mat4& m)
  {
    e << YAML::Flow;
    e << YAML::BeginSeq;
    e << m(0, 0) << m(0, 1) << m(0, 2) << m(0, 3); //
    e << m(1, 0) << m(1, 1) << m(1, 2) << m(1, 3); //
    e << m(2, 0) << m(2, 1) << m(2, 2) << m(2, 3); //
    e << m(3, 0) << m(3, 1) << m(3, 2) << m(3, 3); //
    e << YAML::EndSeq;
    GE_ASSERT_NO_MSG(e.good());
    return e;
  }
  YAML::Emitter& operator<<(YAML::Emitter& e, const Dimensions& d)
  {
    e << YAML::Flow;
    e << YAML::BeginSeq;
    e << d.width << d.height;
    e << YAML::EndSeq;
    GE_ASSERT_NO_MSG(e.good());
    return e;
  }
  YAML::Emitter& operator<<(YAML::Emitter& e, const Drawable& d)
  {
    e << YAML::BeginMap; // drawable
    const VerticesData& vertices_data = d.GetVerticesData();
    const u64& v_count = vertices_data.GetCount();
    e << YAML::Key << Fields::VERTICES_COUNT << YAML::Value << v_count;
    e << YAML::Key << Fields::VERTICES_DATA << YAML::Value << YAML::BeginSeq; // vertices
    for (const VertexStruct& vd : vertices_data.GetData())
    {
      e << YAML::BeginMap; // vertex
      e << YAML::Key << Fields::POSITION << YAML::Value << vd.position;
      e << YAML::Key << Fields::TEX_COORDINATES << YAML::Value << vd.texture_coord;
      e << YAML::Key << Fields::COLOR_DECIMAL << YAML::Value << vd.color;
      e << YAML::Key << Fields::NORMAL << YAML::Value << vd.normal;
      e << YAML::Key << Fields::TEXTURE_SLOT << YAML::Value << vd.texture_slot;
      e << YAML::EndMap; // vertex
    }
    e << YAML::EndSeq; // vertices
    const std::vector<u32>& i_data = d.GetIndicesData();
    e << YAML::Key << Fields::INDICES_DATA << YAML::Value << YAML::Flow << i_data;
    e << YAML::EndMap; // drawable
    GE_ASSERT_NO_MSG(e.good());
    return e;
  }
  YAML::Emitter& operator<<(YAML::Emitter& e, const Camera& c)
  {
    e << YAML::BeginMap; // camera
    e << YAML::Key << Fields::PROJECTION << YAML::Value << c.GetProjection();
    e << YAML::EndMap; // camera
    GE_ASSERT_NO_MSG(e.good());
    return e;
  }
  YAML::Emitter& operator<<(YAML::Emitter& e, const SceneCamera& sc)
  {
    e << YAML::BeginMap; // scene_camera
    e << YAML::Key << Fields::CAMERA << YAML::Value << static_cast<const Camera&>(sc);
    e << YAML::Key << Fields::POSITION << YAML::Value << sc.GetPosition();
    e << YAML::Key << Fields::TARGET << YAML::Value << sc.GetTarget();
    e << YAML::Key << Fields::VIEWPORT << YAML::Value << sc.GetViewport();
    e << YAML::Key << Fields::FOV << YAML::Value << sc.GetFov();
    e << YAML::Key << Fields::ORTHO_SIZE << YAML::Value << sc.GetOrthographicSize();
    e << YAML::Key << Fields::PROJECTION_MODE << YAML::Value << i32(sc.GetProjectionMode());
    e << YAML::EndMap; // scene_camera
    GE_ASSERT_NO_MSG(e.good());
    return e;
  }
  YAML::Emitter& operator<<(YAML::Emitter& e, const LightSource& ls)
  {
    e << YAML::BeginMap; // lightsource
    e << YAML::Key << Fields::POSITION << YAML::Value << ls.position;
    e << YAML::Key << Fields::COLOR_RGBA << YAML::Value << ls.color;
    e << YAML::Key << Fields::STRENGHT << YAML::Value << ls.light_str;
    e << YAML::Key << Fields::SPEC_STRENGHT << YAML::Value << ls.specular_str;
    e << YAML::Key << Fields::SPEC_SHINE << YAML::Value << ls.shininess;
    e << YAML::EndMap; // lightsource
    GE_ASSERT_NO_MSG(e.good());
    return e;
  }
}

namespace YAML
{
  template <>
  struct convert<Color>
  {
    static bool decode(const Node& node, Color& d)
    {
      GE_ASSERT_OR_RETURN(node.IsMap(), false, "Color not found");

      const auto& rgb = node[Fields::RGB].as<u32>();
      const auto& alpha = node[Fields::ALPHA].as<u32>();
      d = Color{ rgb, u8(std::clamp(alpha, 0x0u, 0xffu)) };
      return true;
    }
  };

  template <>
  struct convert<Dimensions>
  {
    static bool decode(const Node& node, Dimensions& d)
    {
      GE_ASSERT_OR_RETURN(node.IsSequence() && node.size() == 2, false, "Dimension not found");

      const auto& w = node[0].as<u32>();
      const auto& h = node[1].as<u32>();
      d.width = w;
      d.height = h;
      return true;
    }
  };

  template <>
  struct convert<GE::Vec2>
  {
    static bool decode(const Node& node, GE::Vec2& v)
    {
      GE_ASSERT_OR_RETURN(node.IsSequence() && node.size() == 2, false, "Vec2 not found");

      v.x = node[0].as<float>();
      v.y = node[1].as<float>();
      return true;
    }
  };

  template <>
  struct convert<GE::Vec3>
  {
    static bool decode(const Node& node, GE::Vec3& v)
    {
      GE_ASSERT_OR_RETURN(node.IsSequence() && node.size() == 3, false, "Vec3 not found");

      v.x = node[0].as<float>();
      v.y = node[1].as<float>();
      v.z = node[2].as<float>();
      return true;
    }
  };

  template <>
  struct convert<GE::Vec4>
  {
    static bool decode(const Node& node, GE::Vec4& v)
    {
      GE_ASSERT_OR_RETURN(node.IsSequence() && node.size() == 4, false, "Vec4 not found");

      v.x0 = node[0].as<float>();
      v.x1 = node[1].as<float>();
      v.x2 = node[2].as<float>();
      v.x3 = node[3].as<float>();
      return true;
    }
  };

  template <>
  struct convert<Mat4>
  {
    static bool decode(const Node& node, Mat4& sc)
    {
      GE_ASSERT_OR_RETURN(node.IsSequence() && node.size() == 16, false, "Mat4 not found");

      auto data = node.as<std::array<f32, 16>>();
      u32 counter = 0;
      for (u32 row = 0; row < 4; ++row)
        for (u32 col = 0; col < 4; ++col)
          sc(row, col) = data.at(counter++);
      return true;
    }
  };

  template <>
  struct convert<Camera>
  {
    static bool decode(const Node& node, Camera& sc)
    {
      GE_ASSERT_OR_RETURN(!node.IsSequence() && node.IsMap(), false, "Camera not found");

      const auto& data = node[Fields::PROJECTION].as<Mat4>();
      sc.SetProjection(data);
      return true;
    }
  };

  template <>
  struct convert<SceneCamera>
  {
    static bool decode(const Node& node, SceneCamera& sc)
    {
      GE_ASSERT_OR_RETURN(!node.IsSequence() && node.IsMap(), false, "SceneCamera not found");

      const auto& camera = node[Fields::CAMERA].as<Camera>();
      const auto& pos = node[Fields::POSITION].as<Vec3>();
      const auto& target = node[Fields::TARGET].as<Vec3>();
      const auto& viewport = node[Fields::VIEWPORT].as<Dimensions>();
      const auto& fov = node[Fields::FOV].as<f32>();
      const auto& ortho = node[Fields::ORTHO_SIZE].as<f32>();
      const auto& proj_mode = node[Fields::PROJECTION_MODE].as<i32>();

      sc.SetProjection(camera.GetProjection());
      sc.SetView(pos, target);
      sc.SetViewport(viewport);
      sc.SetFov(fov);
      sc.SetOrthographicSize(ortho);
      sc.SetProjectionMode(static_cast<ProjectionMode>(proj_mode));

      return true;
    }
  };

  template <>
  struct convert<VertexStruct>
  {
    static bool decode(const Node& node, VertexStruct& d)
    {
      GE_ASSERT_OR_RETURN(!node.IsSequence() && node.IsMap(), false, "VertexStruct not found");

      d.position = node[Fields::POSITION].as<decltype(d.position)>();
      d.texture_coord = node[Fields::TEX_COORDINATES].as<decltype(d.texture_coord)>();
      d.color = node[Fields::COLOR_DECIMAL].as<decltype(d.color)>();
      d.normal = node[Fields::NORMAL].as<decltype(d.normal)>();
      d.texture_slot = node[Fields::TEXTURE_SLOT].as<decltype(d.texture_slot)>();

      return true;
    }
  };

  template <>
  struct convert<Drawable>
  {
    static bool decode(const Node& node, Drawable& d)
    {
      GE_ASSERT_OR_RETURN(!node.IsSequence() && node.IsMap(), false, "Drawable not found");

      const auto& v_count = node[Fields::VERTICES_COUNT].as<u32>();
      const auto& v_data = node[Fields::VERTICES_DATA].as<std::vector<VertexStruct>>();
      const auto& i_data = node[Fields::INDICES_DATA].as<std::vector<u32>>();

      GE_ASSERT(v_data.size() == v_count, "Wrong number of vertices count");

      d = Drawable(VerticesData{ v_data }, i_data);
      return true;
    }
  };

  template <>
  struct convert<LightSource>
  {
    static bool decode(const Node& node, LightSource& ls)
    {
      GE_ASSERT_OR_RETURN(!node.IsSequence() && node.IsMap(), false, "LightSource not found");

      ls.position = node[Fields::POSITION].as<Vec3>();
      ls.color = node[Fields::COLOR_RGBA].as<Color>();
      ls.light_str = node[Fields::STRENGHT].as<f32>();
      ls.specular_str = node[Fields::SPEC_STRENGHT].as<f32>();
      ls.shininess = node[Fields::SPEC_SHINE].as<u32>();
      return true;
    }
  };
}

ComponentSerializer::ComponentSerializer(YAML::Emitter& emmiter) : m_emitter(emmiter) {}

void ComponentSerializer::operator()(const TagComponent& c) const
{
  m_emitter << YAML::Key << Titles::TAG_COMP << YAML::Value;
  m_emitter << YAML::BeginMap; // component
  m_emitter << YAML::Key << Fields::TAG << YAML::Value << c.GetTag();
  m_emitter << YAML::EndMap; // component
  GE_ASSERT_NO_MSG(m_emitter.good());
}

void ComponentSerializer::operator()(const TransformComponent& c) const
{
  m_emitter << YAML::Key << Titles::TRANFORM_COMP << YAML::Value;
  m_emitter << YAML::BeginMap; // component
  m_emitter << YAML::Key << Fields::POSITION << YAML::Value << c.Position();
  m_emitter << YAML::Key << Fields::ROTATION << YAML::Value << c.Rotation();
  m_emitter << YAML::Key << Fields::SCALE << YAML::Value << c.Scale();
  m_emitter << YAML::EndMap; // component
  GE_ASSERT_NO_MSG(m_emitter.good());
}

void ComponentSerializer::operator()(const PrimitiveComponent& c) const
{
  m_emitter << YAML::Key << Titles::PRIMITIVE_COMP << YAML::Value;
  m_emitter << YAML::BeginMap; // component
  m_emitter << YAML::Key << Fields::DRAWABLE << YAML::Value << c.GetDrawable();
  m_emitter << YAML::Key << Fields::COLOR_RGBA << YAML::Value << c.GetColor();
  m_emitter << YAML::Key << Fields::TEXTURE_SLOT << YAML::Value << c.GetTexSlot();
  m_emitter << YAML::EndMap; // component
  GE_ASSERT_NO_MSG(m_emitter.good());
}

void ComponentSerializer::operator()(const CameraComponent& c) const
{
  m_emitter << YAML::Key << Titles::CAMERA_COMP << YAML::Value;
  m_emitter << YAML::BeginMap; // component
  m_emitter << YAML::Key << Fields::SCENE_CAMERA << YAML::Value << c.GetCamera();
  m_emitter << YAML::Key << Fields::ACTIVE << YAML::Value << c.IsActive();
  m_emitter << YAML::Key << Fields::FIXED_RATIO << YAML::Value << c.IsFixedRatio();
  m_emitter << YAML::EndMap; // component
  GE_ASSERT_NO_MSG(m_emitter.good());
}

void ComponentSerializer::operator()(const NativeScriptComponent&) const
{
  GE_ERROR("Not writing NativeScriptComponent")
  // GE_ASSERT_NO_MSG(false);
}

void ComponentSerializer::operator()(const AmbientLightComponent& c) const
{
  m_emitter << YAML::Key << Titles::AMBIENT_LIGHT_COMP << YAML::Value;
  m_emitter << YAML::BeginMap; // component
  m_emitter << YAML::Key << Fields::COLOR_RGBA << YAML::Value << c.GetColor();
  m_emitter << YAML::Key << Fields::STRENGHT << YAML::Value << c.GetStr();
  m_emitter << YAML::Key << Fields::ACTIVE << YAML::Value << c.IsActive();
  m_emitter << YAML::EndMap; // component
  GE_ASSERT_NO_MSG(m_emitter.good());
}

void ComponentSerializer::operator()(const LightSourceComponent& c) const
{
  m_emitter << YAML::Key << Titles::LIGHT_SOURCE_COMP << YAML::Value;
  m_emitter << YAML::BeginMap; // component
  m_emitter << YAML::Key << Fields::LIGHT_SOURCE << YAML::Value << c.GetLightSource();
  m_emitter << YAML::Key << Fields::ACTIVE << YAML::Value << c.IsActive();
  // m_emitter << YAML::Key << Fields::DRAWABLE << YAML::Value << c.GetDrawable(); // not needed
  m_emitter << YAML::EndMap; // component
  GE_ASSERT_NO_MSG(m_emitter.good());
}

ComponentDeserializer::ComponentDeserializer(const YAML::Node& n) : m_node(n) {}

Opt<TagComponent> ComponentDeserializer::GetTag() const
{
  auto comp = m_node[Titles::TAG_COMP];
  if (!comp)
    return {};

  auto tag = comp[Fields::TAG].as<std::string>();
  return { std::move(tag) };
}

Opt<TransformComponent> ComponentDeserializer::GetTransform() const
{
  auto comp = m_node[Titles::TRANFORM_COMP];
  if (!comp)
    return {};

  auto pos_array = comp[Fields::POSITION].as<std::array<f32, 3>>();
  auto rot_array = comp[Fields::ROTATION].as<std::array<f32, 3>>();
  auto scale_array = comp[Fields::SCALE].as<std::array<f32, 3>>();
  Vec3 translation{ pos_array[0], pos_array[1], pos_array[2] };
  Vec3 rotation{ rot_array[0], rot_array[1], rot_array[2] };
  Vec3 scale{ scale_array[0], scale_array[1], scale_array[2] };
  return TransformComponent{ translation, scale, rotation };
}

Opt<PrimitiveComponent> ComponentDeserializer::GetPrimitive() const
{
  auto comp = m_node[Titles::PRIMITIVE_COMP];
  if (!comp)
    return {};

  auto drawable_node = comp[Fields::DRAWABLE];
  auto vertices_count = drawable_node[Fields::VERTICES_COUNT].as<u32>();
  std::vector<VertexStruct> vertices;
  vertices.reserve(vertices_count);
  auto vertices_data_node = drawable_node[Fields::VERTICES_DATA];
  for (const auto& _ : vertices_data_node)
  {
    auto pos_array = _[Fields::POSITION].as<std::array<f32, 3>>();
    auto tex_array = _[Fields::TEX_COORDINATES].as<std::array<f32, 2>>();
    auto color_array = _[Fields::COLOR_DECIMAL].as<std::array<f32, 4>>();
    auto normal = _[Fields::NORMAL].as<std::array<f32, 3>>();
    u32 tex_slot = Texture2D::EMPTY_TEX_SLOT;
    if (_[Fields::TEXTURE_SLOT].IsDefined())
      tex_slot = _[Fields::TEXTURE_SLOT].as<u32>();

    VertexStruct vertex{ Vec3{ pos_array[0], pos_array[1], pos_array[2] },
                         Vec2{ tex_array[0], tex_array[1] },
                         Vec4{ color_array[0], color_array[1], color_array[2], color_array[3] },
                         Vec3{ normal[0], normal[1], normal[2] },
                         tex_slot };

    vertices.push_back(vertex);
  }
  VerticesData vertices_data{ vertices };
  auto indices_data = drawable_node[Fields::INDICES_DATA].as<std::vector<u32>>();
  Drawable drawable{ vertices_data, indices_data };
  Color color = comp[Fields::COLOR_RGBA].as<Color>();
  u32 tex_slot = Texture2D::EMPTY_TEX_SLOT;
  if (comp[Fields::TEXTURE_SLOT].IsDefined())
    tex_slot = comp[Fields::TEXTURE_SLOT].as<u32>();

  return PrimitiveComponent{ drawable, color, tex_slot };
}

Opt<CameraComponent> ComponentDeserializer::GetCamera() const
{
  auto comp = m_node[Titles::CAMERA_COMP];
  if (!comp)
    return {};

  auto scene_camera = comp[Fields::SCENE_CAMERA].as<SceneCamera>();
  auto is_active = comp[Fields::ACTIVE].as<bool>();
  auto is_fixed_ratio = comp[Fields::FIXED_RATIO].as<bool>();
  return CameraComponent{ scene_camera, is_active, is_fixed_ratio };
}

Opt<NativeScriptComponent> ComponentDeserializer::GetNativeScript() const
{
  GE_ASSERT_OR_RETURN(false, {}, "No deserializing scripts yet");
  return {};
}

Opt<AmbientLightComponent> ComponentDeserializer::GetAmbientLight() const
{
  auto comp = m_node[Titles::AMBIENT_LIGHT_COMP];
  if (!comp)
    return {};

  auto color = comp[Fields::COLOR_RGBA].as<Color>();
  auto strenght = comp[Fields::STRENGHT].as<f32>();
  auto active = comp[Fields::ACTIVE].as<bool>();
  return AmbientLightComponent{ color, strenght, active };
}

Opt<LightSourceComponent> ComponentDeserializer::GetLightSource() const
{
  auto comp = m_node[Titles::LIGHT_SOURCE_COMP];
  if (!comp)
    return {};

  auto ls = comp[Fields::LIGHT_SOURCE].as<LightSource>();
  auto active = comp[Fields::ACTIVE].as<bool>();
  return LightSourceComponent{ ls, active };
}
