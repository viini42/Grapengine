#include "MainLayer.hpp"

MainLayer::MainLayer() : GE::Layer("MainLayer"), m_scene(GE::Scene::Make("MainLayer"))
{
  GE::SceneSerializer{m_scene}.DeserializeFromFile("Assets/scenes/Scene_Learn.yaml");
}

MainLayer::~MainLayer() = default;

void MainLayer::OnAttach()
{
  m_scene->OnAttach();
}

void MainLayer::OnUpdate(GE::TimeStep step)
{
  Layer::OnUpdate(step);

  GE::Renderer::SetClearColor(GE::Color{ 0x0072d5 }.ToVec4());
  GE::Renderer::Clear();

  m_scene->OnUpdate(step);
}

void MainLayer::OnEvent(GE::Event& event)
{
  Layer::OnEvent(event);
}
