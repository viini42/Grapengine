#include "core/ge_memory.hpp"
#include "core/ge_window.hpp"
#include "renderer/ge_shader.hpp"

#include <exception>
#include <gtest/gtest.h>

#if defined(GE_CLANG_COMPILER)
  #pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

namespace
{
  const std::string& INVALID_VSHADER()
  {
    static std::string s = R"(
#version 330 core
layout (location = 0) in vec3 in_position;
out vec4 out_color;
void main()
{
    gl_Position = vec4(in_position, 1.0f);
    out_color = in_color;
})";
    return s;
  }

  const std::string INVALID_FSHADER()
  {
    static std::string s = R"(
#version 330 core
in vec4 out_color;
out vec4 fragColor;
void mainn()
{
    fragColor = out_color;
})";
    return s;
  }
  const std::string VALID_VSHADER()
  {
    static std::string s = R"(
#version 330 core
layout (location = 0) in vec3 in_position;
out vec4 out_color;
void main()
{
    gl_Position = vec4(in_position, 1.0f);
    out_color = vec4(0,0,0,1);
})";
    return s;
  }

  const std::string VALID_FSHADER()
  {
    static std::string s = R"(
#version 330 core
in vec4 out_color;
out vec4 fragColor;
void main()
{
    fragColor = out_color;
})";
    return s;
  }
}

TEST(ShaderProgram, Shader)
{
  using namespace GE;
  Scope<Window> window = MakeScope<Window>(WindowProps{ "Test", { 1, 1 }, {} }, nullptr);
  EXPECT_NE(window, nullptr);

  ASSERT_DEATH({ Shader shader_program(std::string{ "" }, std::string{ "" }); }, "");
  ASSERT_DEATH({ Shader shader_program(INVALID_VSHADER(), INVALID_FSHADER()); }, "");
  ASSERT_DEATH({ Shader shader_program(VALID_VSHADER(), INVALID_FSHADER()); }, "");

  Shader shader_program(VALID_VSHADER(), VALID_FSHADER());
  EXPECT_TRUE(shader_program.IsValid());

  shader_program.Unbind();
  EXPECT_FALSE(shader_program.IsBound());
}
