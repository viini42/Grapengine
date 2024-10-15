#include "layer/ge_imgui_layer.hpp"

#include "core/ge_time_step.hpp"
#include "profiling/ge_profiler.hpp"

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#undef USE_MULTIPLE_VIEWPORTS

using namespace GE;

namespace
{
  constexpr std::string_view IMGUI_LAYER = "IMGUI_LAYER";
}

ImGuiLayer::ImGuiLayer(Ptr<Window> window) : Layer(IMGUI_LAYER), m_window(std::move(window)) {}

ImGuiLayer::~ImGuiLayer() = default;

#if defined(GE_CLANG_COMPILER)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
void ImGuiLayer::OnAttach()
{
  GE_PROFILE;
  GE_INFO("ImGui initialization")

  Layer::OnAttach();

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.IniFilename = nullptr;
  constexpr auto FONT_SIZE = 16.0f;
  io.Fonts->AddFontFromFileTTF("Assets/fonts/Inter-Regular.ttf", FONT_SIZE);
  io.Fonts->AddFontFromFileTTF("Assets/fonts/Inter-Bold.ttf", FONT_SIZE);

  ImGui::LoadIniSettingsFromDisk("imgui.ini");
#ifdef USE_MULTIPLE_VIEWPORTS
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
#endif

  ImGuiStyle& style = ImGui::GetStyle();
  style.IndentSpacing = 2.0f;
  // NOLINTBEGIN(*-magic-numbers)
  if (bool(io.ConfigFlags & ImGuiConfigFlags_DockingEnable))
  {
    style.WindowRounding = 0;
    style.Alpha = 1.0f;
    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

    // Headers
    colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
    colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
  }
  // NOLINTEND(*-magic-numbers)

  ImGui_ImplGlfw_InitForOpenGL(std::any_cast<GLFWwindow*>(m_window->GetNativeHandler()), true);
  ImGui_ImplOpenGL3_Init();
}
#if defined(GE_CLANG_COMPILER)
  #pragma clang diagnostic pop
#endif

void ImGuiLayer::OnDetach()
{
  GE_PROFILE;
  GE_INFO("ImGui shutdown")

  Layer::OnDetach();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
void ImGuiLayer::Begin()
{
  GE_PROFILE;
  if (m_window == nullptr)
    return;
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

Ptr<ImGuiLayer> ImGuiLayer::Make(Ptr<Window> window)
{
  return MakeRef<ImGuiLayer>(std::move(window));
}

void ImGuiLayer::End()
{
  GE_PROFILE;
  auto& io = ImGui::GetIO();
  const Dimensions win_dim = m_window->GetDimension();
  io.DisplaySize = ImVec2(static_cast<f32>(win_dim.width), static_cast<float>(win_dim.height));

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

#ifdef USE_MULTIPLE_VIEWPORTS
  if (bool(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable))
  {
    GLFWwindow* backup_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_context);
  }
#endif
}

void GE::ImGuiLayer::OnEvent(Event& e)
{
  if (!m_allow_imgui_events)
    return;

  const ImGuiIO& io = ImGui::GetIO();
  if (bool(io.WantCaptureMouse))
  {
    if (e.IsType(EvType::MOUSE_BUTTON_PRESSED) || e.IsType(EvType::MOUSE_BUTTON_RELEASE) ||
        e.IsType(EvType::MOUSE_MOVE) || e.IsType(EvType::MOUSE_SCROLL))
    {
      e.SetHandled();
      return;
    }
  }

  if (io.WantCaptureKeyboard)
  {
    if (e.IsType(EvType::KEY_PRESS) || e.IsType(EvType::KEY_RELEASE))
    {
      e.SetHandled();
      return;
    }
  }
}

void GE::ImGuiLayer::AllowMouseAndKeyboardEvents(bool allow)
{
  m_allow_imgui_events = allow;
}
