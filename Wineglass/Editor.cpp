#include "Editor.hpp"

#include "EditorLayer.hpp"
#include "core/ge_entry_point.hpp"

namespace
{
  constexpr GE::Dimensions HD_DIMENSION{ 1280, 720 };
}

GE::Editor::Editor() :
    Application("Wineglass - Grapengine", HD_DIMENSION, "Assets/ic_grapengine.png")
{
  GE_INFO("Create Wineglass Editor")
  auto editor_layer = MakeRef<EditorLayer>();
  Application::AddLayer(editor_layer);
}

GE::Editor::~Editor() = default;

GE::Ptr<GE::Application> CreateApplication()
{
  return GE::MakeRef<GE::Editor>();
}
