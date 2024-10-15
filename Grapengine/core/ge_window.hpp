#ifndef GRAPENGINE_WINDOW_HPP
#define GRAPENGINE_WINDOW_HPP

#include "GLFW/glfw3.h"
#include "drawables/ge_color.hpp"
#include "events/ge_event.hpp"
#include "ge_context.hpp"
#include "log/ge_logger.hpp"
#include "math/ge_vector.hpp"
#include "utils/ge_dimension.hpp"

namespace GE
{
  using EventCallbackFn = std::function<void(Event&)>;

  class Context;

  struct WindowProps
  {
    std::string title;
    std::string icon_path;
    Dimensions dim;

    explicit WindowProps(std::string_view titleStr, Dimensions d, std::string_view icon) :
        title(titleStr), icon_path(icon), dim(d)
    {
    }
  };

  class Window
  {
  public:
    static Ptr<Window> Make(const WindowProps& props, const EventCallbackFn& cb);

    explicit Window(const WindowProps& props, const EventCallbackFn& cb);
    ~Window();

    [[deprecated("Use GetDimension")]] [[nodiscard]] u32 GetWidth() const;
    [[deprecated("Use GetDimension")]] [[nodiscard]] u32 GetHeight() const;
    [[nodiscard]] Dimensions GetDimension() const;

    void SetVsync(bool enabled);
    bool IsVsync() const;

    void OnUpdate();

    [[nodiscard]] std::any GetNativeHandler() const;

  private:
    void SetupCallbacks(const EventCallbackFn& cb);

    WindowProps m_window_props;
    bool m_vsync;
    GLFWwindow* m_window;
    Context m_context;
    EventCallbackFn m_event_callback;
  };
}

#endif
