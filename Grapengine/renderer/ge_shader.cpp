#include "renderer/ge_shader.hpp"

#include "core/ge_assert.hpp"
#include "profiling/ge_profiler.hpp"
#include "utils/ge_io.hpp"

#include <core/ge_platform.hpp>
#include <glad/glad.h>

using namespace GE;

namespace
{
  enum class ShaderType : u8
  {
    VERTEX,
    FRAGMENT
  };

  u32 GetGLShaderType(ShaderType type)
  {
    switch (type)
    {
    case ShaderType::VERTEX:
      return GL_VERTEX_SHADER;
    case ShaderType::FRAGMENT:
      return GL_FRAGMENT_SHADER;
    }
    Platform::Unreachable();
  }

  std::string GetGLShaderName(ShaderType type)
  {
    switch (type)
    {
    case ShaderType::VERTEX:
      return "Vertex";
    case ShaderType::FRAGMENT:
      return "Fragment";
    }
    Platform::Unreachable();
  }

  std::tuple<u32, bool> Compile(const std::string& src, ShaderType type)
  {
    const u32 shader = glCreateShader(GetGLShaderType(type));
    const char* chars = src.c_str();
    glShaderSource(shader, 1, &chars, nullptr);

    glCompileShader(shader);

    i32 success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == 0)
    {
      i32 max_length = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

      std::vector<char> info_log;
      info_log.reserve(static_cast<u32>(max_length));
      glGetShaderInfoLog(shader, max_length, &max_length, info_log.data());

      const std::string shader_name = GetGLShaderName(type);
      GE_ASSERT(false, "Failure at compiling the {} shader\n  {}", shader_name, info_log.data());

      glDeleteShader(shader);
    }
    return std::tuple<u32, bool>{ shader, success == 1 };
  }

  RendererID CreateProgram(const std::string& vertexSrc, const std::string& fragmentSrc)
  {
    GE_PROFILE;
    auto [vertex_shader, vertex_ok] = Compile(vertexSrc, ShaderType::VERTEX);
    if (!vertex_ok)
      return 0;

    auto [fragment_shader, fragment_ok] = Compile(fragmentSrc, ShaderType::FRAGMENT);
    if (!fragment_ok)
    {
      glDeleteShader(vertex_shader);
      return 0;
    }

    auto renderer_id = glCreateProgram();

    glAttachShader(renderer_id, vertex_shader);
    glAttachShader(renderer_id, fragment_shader);

    glLinkProgram(renderer_id);

    i32 is_linked = 0;
    glGetProgramiv(renderer_id, GL_LINK_STATUS, &is_linked);
    if (is_linked == 0)
    {
      i32 max_length = 0;
      glGetProgramiv(renderer_id, GL_INFO_LOG_LENGTH, &max_length);

      std::vector<char> info_log;
      info_log.resize(static_cast<u32>(max_length));
      glGetProgramInfoLog(renderer_id, max_length, &max_length, info_log.data());

      GE_ASSERT(false, "Failure at linking program!\n  {}", info_log.data());

      glDeleteProgram(renderer_id);

      glDeleteShader(vertex_shader);
      glDeleteShader(fragment_shader);
      return 0;
    }

    glDetachShader(renderer_id, vertex_shader);
    glDetachShader(renderer_id, fragment_shader);

    return renderer_id;
  }
}

Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) :
    m_renderer_id(CreateProgram(vertexSrc, fragmentSrc))
{
  GE_PROFILE;
}

Shader::Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragPath) :
    m_renderer_id(0)
{
  GE_PROFILE;
  auto vertex_src = IO::ReadFileToString(vertexPath);
  auto frag_src = IO::ReadFileToString(fragPath);
  m_renderer_id = CreateProgram(vertex_src, frag_src);
}

void Shader::Bind() const
{
  glUseProgram(u32(m_renderer_id));
}

Shader::~Shader() = default;

void Shader::UploadMat4F(const std::string& name, const Mat4& mat)
{
  UploadMat4F(name, mat.ValuePtr());
}

void Shader::UploadMat4F(const std::string& name, const f32* data)
{
  auto location = RetrieveUniform(name);
  glUniformMatrix4fv(location, 1, GL_FALSE, data);
}

void Shader::UploadInt(const std::string& name, i32 i)
{
  auto location = RetrieveUniform(name);
  glUniform1i(location, i);
}

void Shader::UploadFloat(const std::string& name, f32 i)
{
  auto location = RetrieveUniform(name);
  glUniform1f(location, i);
}

void Shader::UploadVec3(const std::string& name, const Vec3& vec3)
{
  auto location = RetrieveUniform(name);
  glUniform3f(location, vec3.x, vec3.y, vec3.z);
}

void GE::Shader::UploadVec3Array(const std::string& name, const std::vector<Vec3>& vec3)
{
  auto location = RetrieveUniform(name);
  glUniform3fv(location, static_cast<GLsizei>(vec3.size()), &vec3.data()->x);
}

void GE::Shader::UploadFloatArray(const std::string& name, const std::vector<f32>& vec3)
{
  auto location = RetrieveUniform(name);
  glUniform1fv(location, static_cast<GLsizei>(vec3.size()), vec3.data());
}

void Shader::UploadIntArray(const std::string& name, const std::vector<i32>& arr)
{
  auto location = RetrieveUniform(name);
  glUniform1iv(location, static_cast<i32>(arr.size()), static_cast<const i32*>(arr.data()));
}

[[maybe_unused]] bool Shader::IsValid() const
{
  return bool(glIsProgram(u32(m_renderer_id)));
}

bool Shader::IsBound() const
{
  i32 curr_program = -1;
  glGetIntegerv(GL_CURRENT_PROGRAM, &curr_program);
  if (curr_program <= 0)
    return false;

  return static_cast<decltype(m_renderer_id)>(u32(curr_program)) == m_renderer_id;
}

void Shader::Unbind() const
{
  if (IsBound())
    glUseProgram(0);
}
Ptr<Shader> GE::Shader::Make(const std::filesystem::path& vertexPath,
                             const std::filesystem::path& fragPath)
{
  return MakeScope<Shader>(vertexPath, fragPath);
}

i32 Shader::RetrieveUniform(const std::string& name)
{
  GE_ASSERT(IsBound(), "Shader not bound");

  if (m_uniforms.contains(name))
    return m_uniforms[name];

  const i32 location = glGetUniformLocation(u32(m_renderer_id), name.c_str());
  GE_ASSERT(location != -1, "Invalid uniform name: {}", name);

  m_uniforms[name] = location;
  return location;
}
