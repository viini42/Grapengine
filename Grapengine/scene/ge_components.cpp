#include "ge_components.hpp"

#include "math/ge_arithmetic.hpp"

using namespace GE;

//----------------------------------------------------------------------------------------------
TagComponent::TagComponent(std::string&& t) : m_tag(std::move(t)) {}

//----------------------------------------------------------------------------------------------
PrimitiveComponent::PrimitiveComponent(const Drawable& dra, Color c, u32 texSlot) :
    m_drawable(dra), m_color(c), m_texture_slot(texSlot)
{
}
const Drawable& PrimitiveComponent::GetDrawable() const
{
  return m_drawable;
}
Drawable& PrimitiveComponent::GetDrawable()
{
  return m_drawable;
}
const Color& PrimitiveComponent::GetColor() const
{
  return m_color;
}

u32 PrimitiveComponent::GetTexSlot() const
{
  return m_texture_slot;
}

void PrimitiveComponent::SetColor(Color c)
{
  m_color = c;
}

void PrimitiveComponent::SetTexSlot(u32 slot)
{
  m_texture_slot = slot;
}

//----------------------------------------------------------------------------------------------
CameraComponent::CameraComponent(const SceneCamera& cam, bool isActive, bool isFixedRatio) :
    m_camera(cam), m_active(isActive), m_fixed_ratio(isFixedRatio)
{
}

//----------------------------------------------------------------------------------------------
CameraComponent::CameraComponent(const Vec3& eye, const Vec3& target, bool act, bool fixedRatio) :
    m_active(act), m_fixed_ratio(fixedRatio)
{
  m_camera.SetView(eye, target);
}

//----------------------------------------------------------------------------------------------
NativeScriptComponent::NativeScriptComponent() :
    m_instance(nullptr), m_instantiate_fun({}), m_destroy_fun({})
{
}

//----------------------------------------------------------------------------------------------
AmbientLightComponent::AmbientLightComponent(Color c, f32 str) : AmbientLightComponent(c, str, true)
{
}

AmbientLightComponent::AmbientLightComponent(Color c, f32 str, bool active) :
    m_color(c), m_strenght(str), m_active(active)
{
}

bool AmbientLightComponent::operator==(const AmbientLightComponent& lhs) const
{
  return Arithmetic::IsEqual(m_strenght, lhs.m_strenght) && //
         m_active == lhs.m_active &&                        //
         m_color == lhs.m_color;
}
LightSourceComponent::LightSourceComponent(LightSource ls, bool active) :
    m_light_source(ls), m_active(active), m_drawable(Cube().GetDrawable())
{
}
//----------------------------------------------------------------------------------------------
LightSourceComponent::LightSourceComponent(Color c, Vec3 pos, f32 str, bool act) :
    LightSourceComponent({ pos, c, str, 0.5f, 32 }, act)
{
}

bool LightSourceComponent::operator==(const LightSourceComponent& rhs) const
{
  return m_light_source == rhs.m_light_source && //
         m_active == rhs.m_active &&             //
         m_drawable == rhs.m_drawable;
}

bool operator==(const GE::VarComponent& lhs, const GE::VarComponent& rhs)
{
  if (lhs.index() != rhs.index())
    return false;

  return std::visit([](const auto& l, const auto& r) { return l == r; }, lhs, rhs);
}
