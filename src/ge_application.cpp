#include "ge_application.hpp"

#include "ge_data_types.hpp"
#include "ge_index_buffer.hpp"
#include "ge_renderer.hpp"
#include "ge_shader.hpp"
#include "ge_vertex_array.hpp"
#include "ge_vertex_buffer.hpp"
#include "ge_window.hpp"

#include <ge_event_type.hpp>
#include <ge_texture_2d.hpp>
#include <ge_transformations.hpp>

constexpr auto VERTEX_PER_QUAD = 6;
constexpr auto MAX_QUADS = 100;
constexpr auto MAX_VERTICES = MAX_QUADS * 4;
constexpr auto MAX_INDICES = MAX_QUADS * VERTEX_PER_QUAD;

// clang-format off
  // X - left to right axis
  // Y - bottom to top axis
  // Z - near to far axis
  float vertices[] = {
    // Front face - 2
    -0.5, -0.5,  +0.5, 0.00 + 0 * 0.33, 0.00 + 0 * 0.33,
    +0.5, -0.5,  +0.5, 0.34 + 0 * 0.33, 0.00 + 0 * 0.33,
    +0.5, +0.5,  +0.5, 0.34 + 0 * 0.33, 0.34 + 0 * 0.33,
    -0.5, +0.5,  +0.5, 0.00 + 0 * 0.33, 0.34 + 0 * 0.33,

    // Right face - 1
    +0.5, -0.5,  +0.5, 0.00 + 2 * 0.33, 0.00 + 1 * 0.33,
    +0.5, -0.5,  -0.5, 0.34 + 2 * 0.33, 0.00 + 1 * 0.33,
    +0.5, +0.5,  -0.5, 0.34 + 2 * 0.33, 0.34 + 1 * 0.33,
    +0.5, +0.5,  +0.5, 0.00 + 2 * 0.33, 0.34 + 1 * 0.33,

    // Back face - 5
    +0.5, -0.5,  -0.5, 0.00 + 2 * 0.33, 0.00 + 0 * 0.33,
    -0.5, -0.5,  -0.5, 0.34 + 2 * 0.33, 0.00 + 0 * 0.33,
    -0.5, +0.5,  -0.5, 0.34 + 2 * 0.33, 0.34 + 0 * 0.33,
    +0.5, +0.5,  -0.5, 0.00 + 2 * 0.33, 0.34 + 0 * 0.33,

    // Left face - 6
    -0.5, -0.5,  -0.5, 0.00 + 1 * 0.33, 0.00 + 1 * 0.33,
    -0.5, -0.5,  +0.5, 0.34 + 1 * 0.33, 0.00 + 1 * 0.33,
    -0.5, +0.5,  +0.5, 0.34 + 1 * 0.33, 0.34 + 1 * 0.33,
    -0.5, +0.5,  -0.5, 0.00 + 1 * 0.33, 0.34 + 1 * 0.33,

    // Top face - 3
    -0.5, +0.5,  +0.5, 0.00 + 0 * 0.33, 0.00 + 1 * 0.33,
    +0.5, +0.5,  +0.5, 0.34 + 0 * 0.33, 0.00 + 1 * 0.33,
    +0.5, +0.5,  -0.5, 0.34 + 0 * 0.33, 0.34 + 1 * 0.33,
    -0.5, +0.5,  -0.5, 0.00 + 0 * 0.33, 0.34 + 1 * 0.33,

    // Bottom face - 4
    -0.5, -0.5,  -0.5, 0.00 + 0 * 0.33, 0.00 + 1 * 0.33,
    +0.5, -0.5,  -0.5, 0.34 + 0 * 0.33, 0.00 + 1 * 0.33,
    +0.5, -0.5,  +0.5, 0.34 + 0 * 0.33, 0.34 + 1 * 0.33,
    -0.5, -0.5,  +0.5, 0.00 + 0 * 0.33, 0.34 + 1 * 0.33,
  };
// clang-format on

const Vec3 cubePositions[] = { Vec3(0.0f, 0.0f, 0.0f),    Vec3(2.0f, 5.0f, -15.0f),
                               Vec3(-1.5f, -2.2f, -2.5f), Vec3(-3.8f, -2.0f, -12.3f),
                               Vec3(2.4f, -0.4f, -3.5f),  Vec3(-1.7f, 3.0f, -7.5f),
                               Vec3(1.3f, -2.0f, -2.5f),  Vec3(1.5f, 2.0f, -2.5f),
                               Vec3(1.5f, 0.2f, -1.5f),   Vec3(-1.3f, 1.0f, -1.5f) };

struct Application::Impl
{
  std::unique_ptr<Window> window;
  bool running = true;
  bool minimized = false;
  std::shared_ptr<VertexArray> vao;
  std::shared_ptr<ShaderProgram> shader;
  std::shared_ptr<Texture2D> texture;

  static Application* instance;

  void Finish() { running = false; }

  void OnEvent(Event& event)
  {
    Event::Dispatch(EvType::WINDOW_CLOSE,
                    event,
                    [this](auto&&)
                    {
                      Finish();
                      return true;
                    });

    Event::Dispatch(EvType::WINDOW_RESIZE,
                    event,
                    [](const EvData& ev)
                    {
                      const auto& [_, w, h] = *std::get_if<WindowResizeData>(&ev);
                      Renderer::SetViewport(0, 0, w, h);
                      return true;
                    });
  }
};

Application* Application::Impl::instance = nullptr;

Application::Application(std::string&& title, u32 width, u32 height)
{
  m_pimpl = std::make_unique<Impl>();
  if (m_pimpl->instance != nullptr)
    throw std::exception("App already instantiated!");

  m_pimpl->instance = this;

  m_pimpl->window = std::make_unique<Window>(WindowProps{ title, width, height },
                                             [this](Event& e) { m_pimpl->OnEvent(e); });

  Renderer::Init();
  Renderer::SetViewport(0, 0, width, height);

  m_pimpl->vao = std::make_shared<VertexArray>();
  m_pimpl->vao->Bind();

  auto vbo =
    std::make_shared<VertexBuffer>(vertices, (u32)(sizeof(vertices)), m_pimpl->vao->GetID());

  vbo->Bind();
  m_pimpl->vao->SetVertexBuffer(vbo);

  u32 indices[] = {
    0,  1,  2,  2,  3,  0,  // Front face
    4,  5,  6,  6,  7,  4,  // Right face
    8,  9,  10, 10, 11, 8,  // Back face
    12, 13, 14, 14, 15, 12, // Left face
    16, 17, 18, 18, 19, 16, // Top face
    20, 21, 22, 22, 23, 20, // Bottom face
  };
  auto ibo = std::make_shared<IndexBuffer>(indices, 36, m_pimpl->vao->GetID());
  m_pimpl->vao->SetIndexBuffer(ibo);

  m_pimpl->shader = std::make_shared<ShaderProgram>(
    std::filesystem::path{ "assets/shaders/PositionAndTex2D.vshader.glsl" },
    std::filesystem::path{ "assets/shaders/PositionAndTex2D.fshader.glsl" });

  m_pimpl->shader->Bind();

  const auto M = Transform::Identity(); ///*R **/ S * T;

  m_pimpl->shader->UploadMat4F("u_model", M);

  const auto V =
    Transform::LookAt({ 0.0f, 0.0f, 4.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
  m_pimpl->shader->UploadMat4F("u_view", V);

  const auto P = Transform::Perspective(60.0f, 1.0f, 0.1f, 100.f);
  std::cout << Transform::ToString(P) << std::endl;
  m_pimpl->shader->UploadMat4F("u_proj", P);

  m_pimpl->texture = std::make_shared<Texture2D>("assets/dice.jpg");
  m_pimpl->texture->Bind(0);
  m_pimpl->shader->UploadInt("dice_texture", 0);
}

Application::~Application() = default;

void Application::Run()
{
  //  long long sec_count = 0;
  //  float rot = 0;
  //  float transl_x = 0;
  //  bool increment = true;
  while (m_pimpl->running)
  {
    auto start = std::chrono::high_resolution_clock ::now();
    if (!m_pimpl->minimized)
    {
      Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
      Renderer::Clear();

      float angle = 0.0f;
      for (auto cubePosition : cubePositions)
      {
        Mat4 i_model = Transform::Translate(cubePosition.x, cubePosition.y, cubePosition.z);
        i_model = i_model * Transform::Rotate(angle, Vec3{ 1.0f, 0.3f, 0.5f });
        m_pimpl->shader->UploadMat4F("u_model", i_model);
        Renderer::DrawIndexed(m_pimpl->vao, 36);
        angle += 20.0f;
      }

      m_pimpl->window->OnUpdate();
    }
    //    auto end = std::chrono::high_resolution_clock ::now();
    //    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    //    sec_count += duration.count();
    //    if (sec_count > 16)
    //    {
    //      m_pimpl->shader->Bind();
    //
    //      Mat4<float> model_matx = Transform::RotateX(rot / 2);
    //      Mat4<float> model_maty = Transform::RotateY(rot += 1);
    //      Mat4<float> model_transl;
    //      if (increment)
    //      {
    //        model_transl = Transform::Translate(transl_x -= 0.01f, 0.0, 0.0);
    //        if (transl_x <= -0.5)
    //          increment = !increment;
    //      }
    //      else
    //      {
    //        model_transl = Transform::Translate(transl_x += 0.01f, 0.0, 0.0);
    //        if (transl_x >= 0.5)
    //          increment = !increment;
    //      }
    //      auto model_mat = model_transl * model_matx * model_maty;
    //      m_pimpl->shader->UploadMat4F("u_model", model_mat);
    //      sec_count = 0;
    //  }
    //    std::cout << "FPS: " << 1000 / duration.count() << std::endl;
  }
}
