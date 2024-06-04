#include "ge_app_controller.hpp"

#include "core/ge_assert.hpp"

using namespace GE;

void Ctrl::App::Init(const Ptr<Application>& app)
{
  Get().m_application = app;
}

void GE::Ctrl::App::Shutdown()
{
  Get().m_application.reset();
}

void GE::Ctrl::App::Close()
{
  const auto app = Get().m_application;
  GE_ASSERT(app != nullptr, "Application destroyed")

  app->Close();
}
void GE::Ctrl::App::AllowImGuiEvents(bool value)
{
  const auto app = Get().m_application;
  GE_ASSERT(app != nullptr, "Application destroyed")

  app->GetImGuiLayer()->AllowMouseAndKeyboardEvents(value);
}

Ctrl::App& Ctrl::App::Get()
{
  static App app;
  return app;
}

f32 Ctrl::App::GetFPS()
{
  return Get().m_application->GetFPS();
}
