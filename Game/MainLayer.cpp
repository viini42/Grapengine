#include "MainLayer.hpp"

MainLayer::MainLayer() : GE::Layer("MainLayer") {}
MainLayer::~MainLayer() = default;

void MainLayer::OnAttach()
{
  Layer::OnAttach();
}

void MainLayer::OnUpdate(GE::TimeStep step)
{
  Layer::OnUpdate(step);

  GE::Renderer::SetClearColor(GE::Color{ 0x0072d5 }.ToVec4());
  GE::Renderer::Clear();
}

void MainLayer::OnEvent(GE::Event& event)
{
  Layer::OnEvent(event);
}
