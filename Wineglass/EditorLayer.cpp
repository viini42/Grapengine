#include "EditorLayer.hpp"

#include "nativescripts/CamController.hpp"

static constexpr const auto CLEAR_COLOR = 0x222222;

using namespace GE;

EditorLayer::EditorLayer() :
    Layer("EditorLayer"),
    m_scene(Scene::Make("Default")),
    m_front_camera_entity(m_scene->CreateEntity("Front Camera")),
    m_scene_panel(nullptr),
    m_editor_camera(Vec3{ 0, 0, 10 }, Vec3{ 0, 0, 0 })
{
}

void EditorLayer::OnAttach()
{
  auto amb_light = m_scene->CreateEntity("Ambient light");
  m_scene->AddComponent<AmbientLightComponent>(amb_light, Colors::WHITE, .25f);
  m_scene->GetComponent<AmbientLightComponent>(amb_light).SetActive(true);

  m_scene->AddComponent<CameraComponent>(m_front_camera_entity,
                                         Vec3{ 0, 0, 10 },
                                         Vec3{ 0, 0, 0 },
                                         true,
                                         false);

  auto cube_ent = m_scene->CreateEntity("Cube");
  m_scene->AddComponent<PrimitiveComponent>(cube_ent, Cube().GetDrawable(), Colors::WHITE);
  m_scene->AddComponent<TransformComponent>(cube_ent, Vec3{ 0, 0, 0 });

  m_scene->AddComponent<NativeScriptComponent>(m_front_camera_entity);
  m_scene->GetComponent<NativeScriptComponent>(m_front_camera_entity).Bind<CamController>();

  m_scene->OnAttach();
  m_scene_panel = MakeRef<SceneHierarchyPanel>(m_scene);

  m_fb = Framebuffer::Make({ 1280, 720 });
}

void EditorLayer::OnUpdate(TimeStep ts)
{
    m_fb->Resize(m_viewport_dimension);
    m_scene->OnViewportResize(m_viewport_dimension);
    m_editor_camera.UpdateAspectRatio(m_viewport_dimension);

  m_fb->Bind();

  Renderer::SetClearColor(Color{ CLEAR_COLOR }.ToVec4());
  Renderer::Clear();

  if (m_in_editor_camera)
  {
    m_editor_camera.OnUpdate(ts);
    m_scene->OnUpdateEditor(ts, m_editor_camera);
  }
  else
  {
    m_scene->OnUpdate(ts);
  }

  m_fb->Unbind();
}

static f64 s_timer_checker = 0;

void EditorLayer::OnImGuiUpdate(TimeStep ts)
{
  static ImGuiDockNodeFlags dock_node_flags = ImGuiDockNodeFlags_None;

  ImGuiViewport* vp = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(vp->Pos);
  ImGui::SetNextWindowSize(vp->Size);
  ImGui::SetNextWindowViewport(vp->ID);
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_None                    //
                                  | ImGuiWindowFlags_MenuBar               //
                                  | ImGuiWindowFlags_NoDocking             //
                                  | ImGuiWindowFlags_NoTitleBar            //
                                  | ImGuiWindowFlags_NoCollapse            //
                                  | ImGuiWindowFlags_NoResize              //
                                  | ImGuiWindowFlags_NoMove                //
                                  | ImGuiWindowFlags_NoBringToFrontOnFocus //
                                  | ImGuiWindowFlags_NoNavFocus;

  //  if (dock_node_flags & ImGuiDockNodeFlags_PassthruCentralNode)
  //    window_flags |= ImGuiWindowFlags_NoBackground;

  static auto dockspace_open = true;
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::Begin("Dockspace", &dockspace_open, window_flags);
  ImGui::PopStyleVar(3);

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigDockingWithShift = true;
  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
  {
    ImGuiID dock_id = ImGui::GetID("MyDock");
    ImGui::DockSpace(dock_id, ImVec2{ 0, 0 }, dock_node_flags);
  }

  static bool show_demo = false;
  static bool is_vsync = Ctrl::App::IsVSyncOn();
  if (ImGui::BeginMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("Save scene"))
      {
        std::string filename =
          std::vformat("Scene_{}.yaml", std::make_format_args(m_scene->GetName()));
        std::filesystem::path path = std::filesystem::current_path() / "Assets/Scene/" / filename;
        SceneSerializer{ m_scene }.SerializeToFile(path);
      }
      if (ImGui::MenuItem("Load scene"))
      {
        ReadToCurrentScene("Assets/scenes/Scene_Default.yaml");
      }
      if (ImGui::BeginMenu("Load local scenes"))
      {
        std::filesystem::path curr_path = std::filesystem::current_path() / "Assets/scenes/";
        for (const auto& entry : std::filesystem::directory_iterator(curr_path))
        {
          if (entry.is_regular_file() && entry.path().extension() == ".yaml")
          {
            if (ImGui::MenuItem(entry.path().string().c_str()))
            {
              ReadToCurrentScene(entry);
            }
          }
        }
        ImGui::EndMenu();
      }
      if (ImGui::MenuItem("Exit"))
        Ctrl::App::Close();
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Renderer"))
    {
      if (ImGui::MenuItem("Enable VSync", nullptr, &is_vsync))
      {
        Ctrl::App::SetVSync(is_vsync);
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Help"))
    {
      if (ImGui::MenuItem("Show demo", nullptr, &show_demo))
      {
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

  m_scene_panel->OnImGuiRender();

  ImGui::Begin("Stats");
  {
    static Renderer::Statistics stats{};
    static f64 fps{};
    if (s_timer_checker > 1)
    {
      stats = Renderer::GetStats();
      fps = static_cast<double>(Ctrl::App::GetFPS());
      s_timer_checker = 0;
    }
    ImGui::Text("Renderer stats:");
    //    ImGui::Text("Draw calls: %05" PRIu64, stats.draw_calls);
    ImGui::Text("Vertices count: %" PRIu64, stats.vertices_count);
    ImGui::Text("Indices count: %" PRIu64, stats.indices_count);
    ImGui::Text("Time spent to batch: %f s", static_cast<f64>(stats.time_spent) * 1e-9);
    ImGui::Text("FPS %.2f", fps);
    s_timer_checker += ts.Secs();
  }
  ImGui::End();

  ImGui::Begin("Scene settings");
  ImGui::Checkbox("Editor camera", &m_in_editor_camera);
  ImGui::End();

  ImGui::Begin("Viewport");

  //  m_viewport_focused = ImGui::IsWindowFocused();
  m_viewport_hovered = ImGui::IsWindowHovered();
  Ctrl::App::AllowImGuiEvents(/*!m_viewport_focused ||*/ !m_viewport_hovered);
  ImVec2 vp_size = ImGui::GetContentRegionAvail();
  m_viewport_dimension.width = u32(vp_size.x);
  m_viewport_dimension.height = u32(vp_size.y);
  RendererID tex = m_fb->GetColorAttachmentID();
  const auto [w, h] = m_fb->GetDimension();
  ImVec2 size{ f32(w), f32(h) };
  ImGui::Image(TypeUtils::ToVoidPtr(u32(tex)), size, { 0, 1 }, { 1, 0 });
  ImGui::End();
  ImGui::PopStyleVar();

  ImGui::End();

  if (show_demo)
    ImGui::ShowDemoWindow(nullptr);
}

void EditorLayer::OnEvent(Event& e)
{
  if (m_in_editor_camera)
    m_editor_camera.OnEvent(e);
  m_scene->OnEvent(e);
}

void EditorLayer::ReadToCurrentScene(const std::filesystem::path& path)
{
  m_scene = Scene::Make("Untitled");
  m_scene_panel->SetContext(m_scene);
  SceneSerializer{ m_scene }.DeserializeFromFile(path);
  m_scene->OnAttach();
}
