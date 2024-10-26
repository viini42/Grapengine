#include "core/ge_application.hpp"

#include "core/ge_platform.hpp"
#include "core/ge_time_step.hpp"
#include "core/ge_window.hpp"
#include "events/ge_event_type.hpp"
#include "input/ge_input.hpp"
#include "layer/ge_imgui_layer.hpp"
#include "layer/ge_layer.hpp"
#include "profiling/ge_profiler.hpp"
#include "renderer/ge_renderer.hpp"
#include "renderer/ge_texture_2d.hpp"

using namespace GE;

namespace
{
#if defined(PROFILING_ENABLED)
  constexpr auto IMGUI_FRAME = "IMGUI_FRAME";
  constexpr auto LAYERS_FRAME = "LAYERS_FRAME";
#endif
}

Application::Application(std::string_view title, Dimensions dim, std::string_view icon)
{
  GE_PROFILE;
  GE_INFO("Application creation")

  Init(title, dim, icon, [this](auto&& e) { OnEvent(e); });

  Renderer::Init();
  Renderer::SetViewport(0, 0, dim);
}

Application::~Application()
{
  GE_PROFILE;
  OnDestroy();

  GE_INFO("Application shutdown")
}

Ptr<Window> Application::GetWindow()
{
  return m_window;
}

void Application::Init(std::string_view title,
                       Dimensions dim,
                       std::string_view icon,
                       const EventCallbackFn& cb)
{
  GE_PROFILE;
  m_window = MakeScope<Window>(WindowProps{ title, dim, icon }, cb);
  Input::Initialize(m_window);
  m_imgui_layer = ImGuiLayer::Make(m_window);
  m_imgui_layer->OnAttach();
}

void Application::Finish()
{
  GE_PROFILE;
  m_running = false;
}

void Application::OnDestroy()
{
  GE_PROFILE;
  Input::Shutdown();
  m_imgui_layer->OnDetach();
  std::ranges::for_each(m_layers, [](auto&& l) { l->OnDetach(); });
}

void Application::Run()
{
  GE_PROFILE;
  m_last_frame_time = Platform::GetCurrentTimeNS();

  while (m_running)
  {
    const u64 time_ms = Platform::GetCurrentTimeNS();
    TimeStep step{ time_ms - m_last_frame_time };
    m_last_frame_time = time_ms;
    m_current_fps = 1.0 / step.Secs();

    if (!m_minimized)
    {
      GE_PROFILE_FRAME_START(LAYERS_FRAME);
      std::ranges::for_each(m_layers, [&](auto&& l) { l->OnUpdate(step); });
      GE_PROFILE_FRAME_END(LAYERS_FRAME);

      GE_PROFILE_FRAME_START(IMGUI_FRAME);
      m_imgui_layer->Begin();
      std::ranges::for_each(m_layers, [&](auto&& l) { l->OnImGuiUpdate(step); });
      m_imgui_layer->End();
      GE_PROFILE_FRAME_END(IMGUI_FRAME);
    }

    m_window->OnUpdate();
    GE_PROFILE_FRAME;
  }
}

void GE::Application::OnEvent(Event& event)
{
  GE_PROFILE;
  m_imgui_layer->OnEvent(event);

  for (const auto& layer : m_layers | std::views::reverse)
  {
    if (event.IsHandled())
      break;
    layer->OnEvent(event);
  }

  event //
    .When(EvType::WINDOW_CLOSE)
    .Then([this] { Finish(); });
}

void GE::Application::AddLayer(const Ptr<Layer>& layer)
{
  GE_PROFILE;
  m_layers.push_back(layer);
  layer->OnAttach();
}

void GE::Application::Close()
{
  GE_PROFILE;
  Finish();
}

Ptr<ImGuiLayer> GE::Application::GetImGuiLayer() const
{
  return m_imgui_layer;
}
