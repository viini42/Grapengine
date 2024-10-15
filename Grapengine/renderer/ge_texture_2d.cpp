#include "renderer/ge_texture_2d.hpp"

#include <glad/glad.h>

// clang-format off
#define STB_IMAGE_IMPLEMENTATION
#if defined(GE_GCC_COMPILER) && (__GNUC__ <= 12)
#define STBI_NO_SIMD
#endif
#include <stb_image.h>
// clang-format on

#include "core/ge_assert.hpp"
#include "profiling/ge_profiler.hpp"

using namespace GE;

namespace
{
  constexpr auto WHITE_RGBA{ 0xFF'FF'FF'FF };
}

Texture2D::Texture2D(const std::filesystem::path& path) : m_dim(), m_renderer_ID(0)
{
  GE_PROFILE;

  GE_ASSERT(std::filesystem::exists(path), "File not found at: {}", path.string());

  stbi_set_flip_vertically_on_load(1);
  i32 w{};
  i32 h{};
  i32 channels{};
  stbi_uc* data = stbi_load(path.string().c_str(), &w, &h, &channels, 0);

  m_dim = Dimensions{ u32(w), u32(h) };

  u32 internal_format = 0;
  u32 format = 0;
  if (channels == 4)
  {
    internal_format = GL_RGBA8;
    format = GL_RGBA;
  }
  else if (channels != 0)
  {
    internal_format = GL_RGB8;
    format = GL_RGB;
  }

  {
    u32 id = 0;
    glCreateTextures(GL_TEXTURE_2D, 1, &id);
    m_renderer_ID = RendererID{ id };
  }
  glTextureStorage2D(u32(m_renderer_ID), 1, internal_format, w, h);

  glTextureParameteri(u32(m_renderer_ID), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(u32(m_renderer_ID), GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTextureParameteri(u32(m_renderer_ID), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTextureParameteri(u32(m_renderer_ID), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  glTextureSubImage2D(u32(m_renderer_ID), 0, 0, 0, w, h, format, GL_UNSIGNED_BYTE, data);

  stbi_image_free(data);
}

Texture2D::Texture2D() : m_dim(), m_renderer_ID(0)
{
  GE_PROFILE;
  const uint32_t internal_format = GL_RGBA8;
  const uint32_t format = GL_RGBA;

  //  constexpr auto size = sizeof(u32);
  constexpr auto width = 1;
  constexpr auto height = 1;
  const bool size_check = true; // size == width * height * 4;
  GE_ASSERT(size_check, "Error at texture size!");

  {
    u32 i = 0;
    glCreateTextures(GL_TEXTURE_2D, 1, &i);
    m_renderer_ID = RendererID{ i };
  }
  glTextureStorage2D(u32(m_renderer_ID), 1, internal_format, width, height);

  glTextureParameteri(u32(m_renderer_ID), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(u32(m_renderer_ID), GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTextureParameteri(u32(m_renderer_ID), GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(u32(m_renderer_ID), GL_TEXTURE_WRAP_T, GL_REPEAT);

  std::array<u32, 1> data{ { WHITE_RGBA } };
  glTextureSubImage2D(u32(m_renderer_ID),
                      0,
                      0,
                      0,
                      width,
                      height,
                      format,
                      GL_UNSIGNED_BYTE,
                      data.data());
}

Texture2D::~Texture2D()
{
  const u32 id = u32(m_renderer_ID);
  glDeleteTextures(1, &id);
}

void Texture2D::Bind(u32 slot) const
{
  glBindTextureUnit(slot, u32(m_renderer_ID));
}

Ptr<Texture2D> GE::Texture2D::Make()
{
  return GE::MakeRef<Texture2D>();
}

Ptr<Texture2D> GE::Texture2D::Make(const std::filesystem::path& path)
{
  return GE::MakeRef<Texture2D>(path);
}

RendererID GE::Texture2D::GetRendererID() const
{
  return m_renderer_ID;
}
