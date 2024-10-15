#ifndef GRAPENGINE_TEXTURE_2D_HPP
#define GRAPENGINE_TEXTURE_2D_HPP

#include "ge_renderer_id.hpp"
#include "utils/ge_dimension.hpp"

#include <filesystem>

namespace GE
{
  class Texture2D
  {
  public:
    constexpr static u32 EMPTY_TEX_SLOT = 0;
    static Ptr<Texture2D> Make();
    static Ptr<Texture2D> Make(const std::filesystem::path& path);

    explicit Texture2D();
    explicit Texture2D(const std::filesystem::path& path);
    ~Texture2D();

    void Bind(u32 slot) const;
    RendererID GetRendererID() const;

  private:
    std::filesystem::path m_path;
    Dimensions m_dim;
    RendererID m_renderer_ID;
  };
}

#endif // GRAPENGINE_TEXTURE_2D_HPP
