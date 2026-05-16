#ifndef GRAPENGINE_MATERIAL_SHADER_HPP
#define GRAPENGINE_MATERIAL_SHADER_HPP

#include "drawables/ge_color.hpp"
#include "math/ge_vector.hpp"
#include "renderer/ge_ishader_program.hpp"
#include "renderer/ge_light_source.hpp"
#include "renderer/ge_shader.hpp"

namespace GE
{
  class MaterialShader final : public IShaderProgram
  {
  public:
    static Ptr<MaterialShader> Make();

    MaterialShader();
    ~MaterialShader() override;

    void Activate() override;
    void Deactivate() override;

    void UpdateViewProjectionMatrix(const Mat4& viewProj, const Vec3& viewPosition) override;
    void UpdateTextures(const std::vector<i32>& textures);

    /**
     * Update ambient light properties
     * @param color light color
     * @param strength light strenght limited from 0 to 10
     */
    void UpdateAmbientLight(Color color, f32 strength);
    //    void ClearAmbientLight();

    /**
     *
     * @param lightSources lights properties
     */
    void UpdateLightSources(const std::vector<LightSource>& lightSources);
    //    void ClearLightSources();

  private:
    void UpdateAmbientColor(Color color);
    void UpdateAmbientStrength(f32 strength);

    void UpdateLightPosition(const std::vector<Vec3>& pos);
    void UpdateLightColor(const std::vector<Vec3>& color);
    void UpdateLightStrength(const std::vector<f32>& strength);

    void UpdateViewPosition(const Vec3& pos);

    Ptr<Shader> m_shader;
  };
}
#endif // GRAPENGINE_MATERIAL_SHADER_HPP
