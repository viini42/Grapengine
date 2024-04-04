#include "layer/ge_imgui_layer.hpp"

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <controllers/ge_app_controller.hpp>
#include <core/ge_time_step.hpp>
#include <imgui.h>

#define USE_MULTIPLE_VIEWPORTS

using namespace GE;

namespace
{
  constexpr auto IMGUI_LAYER = "IMGUI_LAYER";
}

struct ImGuiLayer::Impl
{
  Ref<Window> window = nullptr;
};

ImGuiLayer::ImGuiLayer(Ref<Window> window) : Layer(IMGUI_LAYER), m_pimpl(MakeScope<Impl>())
{
  m_pimpl->window = std::move(window);
}

ImGuiLayer::~ImGuiLayer() = default;

void ImGuiLayer::OnAttach()
{
  GE_INFO("ImGui initialization")

  Layer::OnAttach();

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#ifdef USE_MULTIPLE_VIEWPORTS
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
#endif

  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
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

  ImGui_ImplGlfw_InitForOpenGL(std::any_cast<GLFWwindow*>(m_pimpl->window->GetNativeHandler()),
                               true);
  ImGui_ImplOpenGL3_Init();
}
void ImGuiLayer::OnDetach()
{
  GE_INFO("ImGui shutdown")

  Layer::OnDetach();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
void ImGuiLayer::Begin()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

Ref<ImGuiLayer> ImGuiLayer::Make(Ref<Window> window)
{
  return MakeRef<ImGuiLayer>(std::move(window));
}

void ImGuiLayer::End()
{
  auto& io = ImGui::GetIO();
  io.DisplaySize = ImVec2(static_cast<f32>(m_pimpl->window->GetWidth()),
                          static_cast<float>(m_pimpl->window->GetWidth()));

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

#ifdef USE_MULTIPLE_VIEWPORTS
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    GLFWwindow* backup_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_context);
  }
#endif
}