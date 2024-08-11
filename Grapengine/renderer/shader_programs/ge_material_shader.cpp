#include "renderer/shader_programs/ge_material_shader.hpp"

#include "profiling/ge_profiler.hpp"
#include "renderer/ge_buffer_layout.hpp"
#include "renderer/ge_shader_data_types.hpp"

#include <drawables/ge_color.hpp>
#include <renderer/ge_shader.hpp>

using namespace GE;

namespace
{
  constexpr auto MAX_LIGHTS_SOURCES = 100;
}

MaterialShader::MaterialShader()
{
  GE_PROFILE;
  m_shader =
    Shader::Make("assets/shaders/Material.vshader.glsl", "assets/shaders/Material.fshader.glsl");
}

MaterialShader::~MaterialShader() = default;

void MaterialShader::Activate()
{
  m_shader->Bind();
}

void MaterialShader::Deactivate()
{
  m_shader->Unbind();
}

void MaterialShader::UpdateViewProjectionMatrix(const Mat4& viewProj, const Vec3& viewPosition)
{
  Activate();
  m_shader->UploadMat4F("u_VP", viewProj);
  m_shader->UploadVec3("u_viewPos", viewPosition);
}

void MaterialShader::UpdateTexture(int id)
{
  Activate();
  m_shader->UploadInt("u_texture", id);
}

void GE::MaterialShader::UpdateAmbientColor(Color color)
{
  m_shader->UploadVec3("u_ambientColor", color.ToVec3());
}

void GE::MaterialShader::UpdateAmbientStrength(f32 strength)
{
  m_shader->UploadFloat("u_ambientStrength", strength);
}

void GE::MaterialShader::UpdateLightPosition(const std::vector<Vec3>& pos)
{
  m_shader->UploadVec3Array("u_lightPos", pos);
}

void GE::MaterialShader::UpdateLightColor(const std::vector<Vec3>& colors)
{
  m_shader->UploadVec3Array("u_lightColor", colors);
}

void GE::MaterialShader::UpdateLightStrength(const std::vector<f32>& strength)
{
  m_shader->UploadFloatArray("u_lightStrength", strength);
}

Ptr<MaterialShader> MaterialShader::Make()
{
  return MakeRef<MaterialShader>();
}

void MaterialShader::UpdateAmbientLight(Color color, f32 strength)
{
  UpdateAmbientColor(color);
  UpdateAmbientStrength(strength);
}

void MaterialShader::UpdateLightSources(const std::vector<LightSource>& lightSources)
{
  GE_ASSERT_NO_MSG(lightSources.size() <= MAX_LIGHTS_SOURCES);

  //  m_shader->UploadVec3("u_LightSource", std::ref(lightSources.front().position));
  std::vector<Vec3> positions;
  positions.reserve(lightSources.size());
  std::vector<Vec3> colors;
  colors.reserve(lightSources.size());
  std::vector<f32> strenghts;
  strenghts.reserve(lightSources.size());
  std::vector<f32> specular_strenghts;
  specular_strenghts.reserve(lightSources.size());
  std::vector<f32> specular_shininess;
  specular_shininess.reserve(lightSources.size());
  for (const auto& [pos, color, str, spec, shine] : lightSources)
  {
    positions.push_back(pos);
    colors.push_back(color.ToVec3());
    strenghts.push_back(str);
    specular_strenghts.push_back(spec);
    specular_shininess.push_back(f32(shine));
  }

  UpdateLightPosition(positions);
  UpdateLightColor(colors);
  UpdateLightStrength(strenghts);
  m_shader->UploadFloatArray("u_specularStrenght", specular_strenghts);
  m_shader->UploadFloatArray("u_specularShininess", specular_shininess);
  m_shader->UploadInt("u_lights_count", i32(lightSources.size()));
}

//void MaterialShader::ClearAmbientLight()
//{
//  UpdateAmbientLight(Colors::WHITE, 1.0f);
//}
//
//void MaterialShader::ClearLightSources()
//{
//  UpdateLightSources({});
//}
