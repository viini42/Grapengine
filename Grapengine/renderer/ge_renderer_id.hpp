#ifndef GRAPENGINE_GE_RENDERER_ID_HPP
#define GRAPENGINE_GE_RENDERER_ID_HPP

#include "core/ge_type_aliases.hpp"

namespace GE
{
  class RendererID
  {
  public:
    RendererID(u32 id) : m_id(id) {}

    explicit operator u32() const { return m_id; }
    explicit operator i32() const { return i32(m_id); }

    [[nodiscard]] bool operator==(const RendererID& rhs) const { return m_id == rhs.m_id; }
    [[nodiscard]] bool operator!=(const RendererID& rhs) const { return !(rhs == *this); }

  private:
    u32 m_id;
  };
}

#endif // GRAPENGINE_GE_RENDERER_ID_HPP
