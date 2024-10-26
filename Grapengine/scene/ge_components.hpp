#ifndef GRAPENGINE_GE_COMPONENTS_HPP
#define GRAPENGINE_GE_COMPONENTS_HPP

#include "drawables/ge_color.hpp"
#include "drawables/ge_cube.hpp"
#include "ge_entity.hpp"
#include "ge_scene_camera.hpp"
#include "math/ge_transformations.hpp"
#include "math/ge_vector.hpp"
#include "renderer/ge_light_source.hpp"
#include "renderer/ge_texture_2d.hpp"

namespace GE
{
  //----------------------------------------------------------------------------------------------
  class TagComponent
  {
  public:
    TagComponent(std::string&& t);

    [[nodiscard]] std::string& Tag() { return m_tag; }

    [[nodiscard]] const char* GetTag() const { return m_tag.c_str(); }

    bool operator==(const TagComponent&) const = default;

  private:
    std::string m_tag;
  };

  //----------------------------------------------------------------------------------------------
  class TransformComponent
  {
  public:
    TransformComponent(const Vec3& pos = Vec3{ 0, 0, 0 },
                       const Vec3& scale = Vec3{ 1, 1, 1 },
                       const Vec3& rotation = Vec3{ 0, 0, 0 }) :
        m_position_values(pos), m_scale_values(scale), m_rotation_values(rotation)
    {
    }
    TransformComponent(const TransformComponent& other) = default;
    TransformComponent& operator=(const TransformComponent& other) = default;
    [[nodiscard]] Mat4 GetModelMat() const
    {
      auto rotate = Transform::Rotate(m_rotation_values.x, { 1, 0, 0 }) *
                    Transform::Rotate(m_rotation_values.y, { 0, 1, 0 }) *
                    Transform::Rotate(m_rotation_values.z, { 0, 0, 1 });

      return Transform::Translate(m_position_values) * rotate * Transform::Scale(m_scale_values);
    }

    [[nodiscard]] Vec3& Position() { return m_position_values; }
    [[nodiscard]] Vec3& Scale() { return m_scale_values; }
    [[nodiscard]] Vec3& Rotation() { return m_rotation_values; }
    [[nodiscard]] const Vec3& Position() const { return m_position_values; }
    [[nodiscard]] const Vec3& Scale() const { return m_scale_values; }
    [[nodiscard]] const Vec3& Rotation() const { return m_rotation_values; }

    bool operator==(const TransformComponent&) const = default;

  private:
    Vec3 m_position_values{ 0, 0, 0 };
    Vec3 m_scale_values{ 1, 1, 1 };
    Vec3 m_rotation_values{ 0, 0, 0 };
  };

  //----------------------------------------------------------------------------------------------
  class DrawingObject;
  class PrimitiveComponent
  {
  public:
    PrimitiveComponent(const Drawable& drawable,
                       Color color,
                       u32 texSlot = Texture2D::EMPTY_TEX_SLOT);

    [[nodiscard]] const Drawable& GetDrawable() const;
    [[nodiscard]] Drawable& GetDrawable();
    [[nodiscard]] const Color& GetColor() const;
    [[nodiscard]] u32 GetTexSlot() const;

    void SetColor(Color c);
    void SetTexSlot(u32 slot);

    bool operator==(const PrimitiveComponent&) const = default;

  private:
    Drawable m_drawable;
    Color m_color;
    u32 m_texture_slot;
  };

  //----------------------------------------------------------------------------------------------
  class CameraComponent
  {
  public:
    //    explicit CameraComponent();
    CameraComponent(const SceneCamera& cam, bool isActive, bool isFixedRatio);
    CameraComponent(const Vec3& eye, const Vec3& target, bool act, bool fixedRatio);

    [[nodiscard]] const SceneCamera& GetCamera() const { return m_camera; }
    [[nodiscard]] SceneCamera& GetCamera() { return m_camera; }
    [[nodiscard]] bool IsActive() const { return m_active; }
    [[nodiscard]] bool IsFixedRatio() const { return m_fixed_ratio; }

    void SetActive(bool active) { m_active = active; }

    bool operator==(const CameraComponent&) const = default;

  private:
    SceneCamera m_camera;
    bool m_active;
    bool m_fixed_ratio;
  };

  //----------------------------------------------------------------------------------------------
  class ScriptableEntity;
  class Scene;
  class NativeScriptComponent
  {
  public:
    explicit NativeScriptComponent();

    template <typename T, typename... Args>
    void Bind(Args... args)
    {
      m_instantiate_fun = [&](Entity e, Scene& s) { m_instance = new T(e, std::ref(s), std::forward<Args>(args)...); };
      m_destroy_fun = [this]()
      {
        delete static_cast<T*>(m_instance);
        m_instance = nullptr;
      };
    }

    [[nodiscard]] bool IsValid() const { return m_instance != nullptr; }
    void Instantiate(Entity ent, Scene& scene) const { m_instantiate_fun(ent, scene); }
    [[nodiscard]] ScriptableEntity* GetInstance() const { return m_instance; }

    bool operator==(const NativeScriptComponent&) const { return false; }

  private:
    ScriptableEntity* m_instance;
    std::function<void(Entity, Scene&)> m_instantiate_fun;
    std::function<void()> m_destroy_fun;
  };

  //----------------------------------------------------------------------------------------------
  class AmbientLightComponent
  {
  public:
    AmbientLightComponent(Color c, f32 str);
    AmbientLightComponent(Color c, f32 str, bool active);

    [[nodiscard]] bool IsActive() const { return m_active; }
    [[nodiscard]] const Color& GetColor() const { return m_color; }
    [[nodiscard]] f32 GetStr() const { return m_strenght; }

    void SetActive(bool active) { m_active = active; }
    void SetColor(Color c) { m_color = c; }
    void SetStr(f32 s) { m_strenght = s; }

    bool operator==(const AmbientLightComponent&) const;

  private:
    Color m_color;
    f32 m_strenght;
    bool m_active;
  };

  //----------------------------------------------------------------------------------------------
  class LightSourceComponent
  {
  public:
    LightSourceComponent(LightSource ls, bool active);
    LightSourceComponent(Color c, Vec3 pos, f32 str, bool active);

    [[nodiscard]] Color& ColorRef() { return m_light_source.color; }
    [[nodiscard]] Vec3& Position() { return m_light_source.position; }
    [[nodiscard]] f32& Strenght() { return m_light_source.light_str; }
    [[nodiscard]] bool& Active() { return m_active; }
    [[nodiscard]] f32& SpecStr() { return m_light_source.specular_str; }
    [[nodiscard]] u32& SpecShine() { return m_light_source.shininess; }

    [[nodiscard]] const Color& GetColor() const { return m_light_source.color; }
    [[nodiscard]] const Vec3& GetPos() const { return m_light_source.position; }
    [[nodiscard]] bool IsActive() const { return m_active; }
    [[nodiscard]] const Drawable& GetDrawable() const { return m_drawable; }
    [[nodiscard]] Drawable& GetDrawable() { return m_drawable; }
    [[nodiscard]] const LightSource& GetLightSource() const { return m_light_source; }

    bool operator==(const LightSourceComponent&) const;

  private:
    LightSource m_light_source;
    bool m_active;
    Drawable m_drawable;
  };

  //----------------------------------------------------------------------------------------------
  using VarComponent = std::variant< //
    TagComponent,
    TransformComponent,
    PrimitiveComponent,
    CameraComponent,
    NativeScriptComponent,
    AmbientLightComponent,
    LightSourceComponent //
    >;
}

bool operator==(const GE::VarComponent& lhs, const GE::VarComponent& rhs);

#endif // GRAPENGINE_GE_COMPONENTS_HPP
