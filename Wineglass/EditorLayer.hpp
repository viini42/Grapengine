#ifndef GRAPENGINE_EDITORLAYER_HPP
#define GRAPENGINE_EDITORLAYER_HPP

#include "grapengine.hpp"
#include "panels/SceneHierarchyPanel.hpp"

#define FIRST_SCENE_TEST

namespace GE
{
  class EditorLayer : public Layer
  {
  public:
    EditorLayer();

    void OnAttach() override;

    void OnUpdate(TimeStep ts) override;

    void OnImGuiUpdate(TimeStep ts) override;

    void OnEvent(Event& ev) override;

  private:
    void ReadToCurrentScene(const std::filesystem::path& path);

    Ptr<Scene> m_scene;
    Entity m_front_camera_entity;

    Ptr<Framebuffer> m_fb = nullptr;
    Dimensions m_viewport_dimension{};
    //    bool m_viewport_focused{};
    bool m_viewport_hovered = true;

    Ptr<SceneHierarchyPanel> m_scene_panel;
  };
}

#endif // GRAPENGINE_EDITORLAYER_HPP
