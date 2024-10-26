#ifndef GRAPENGINE_APPLICATION_HPP
#define GRAPENGINE_APPLICATION_HPP

#include "ge_config.hpp"
#include "ge_type_aliases.hpp"
#include "layer/ge_imgui_layer.hpp"

namespace GE
{
  class Window;
  class Event;
  class Layer;

  class Application
  {
  public:
    Application(std::string_view title, Dimensions dim, std::string_view icon);
    virtual ~Application();

    void AddLayer(const Ptr<Layer>& layer);

    void Run();

    void Close();

    void OnEvent(Event& e);

    [[nodiscard]] Ptr<ImGuiLayer> GetImGuiLayer() const;

    f64 GetFPS() const { return m_current_fps; }

    Ptr<Window> GetWindow();

  private:
    void
    Init(std::string_view title, Dimensions dim, std::string_view icon, const EventCallbackFn& cb);

    void OnDestroy();

    void Finish();

    Ptr<Window> m_window;
    bool m_running = true;
    bool m_minimized = false;
    u64 m_last_frame_time{ 0 };
    f64 m_current_fps{ 0 };
    std::vector<Ptr<Layer>> m_layers;
    Ptr<ImGuiLayer> m_imgui_layer;
  };
}

#endif
