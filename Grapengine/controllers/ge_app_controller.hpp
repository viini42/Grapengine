#ifndef GRAPENGINE_GE_APP_CONTROLLER_HPP
#define GRAPENGINE_GE_APP_CONTROLLER_HPP

#include "core/ge_application.hpp"

namespace GE::Ctrl
{
  class App
  {
  public:
    static void Init(const Ptr<Application>& app);
    static void Shutdown();

    static void Close();

    static void SetVSync(bool enable);
    static bool IsVSyncOn();

    static void AllowImGuiEvents(bool value);

    static f64 GetFPS();

  private:
    App() = default;

    static App& Get();

    Ptr<Application> m_application = nullptr;
  };
}
#endif // GRAPENGINE_GE_APP_CONTROLLER_HPP
