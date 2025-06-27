#include <WidgetEngine/WidgetEngine.h>

namespace WidgetEngine {
  void Engine::AddHBoxLayout(const std::string& window, const std::string& name) {
    WidgetEngine::WindowHandle* handle = GetWindow(window);
    if (handle && handle->window && handle->window->isWindow()) {
      QHBoxLayout* layout = new QHBoxLayout(handle->window);
      layout->setObjectName(QString::fromStdString(name));
      layouts[name] = std::move(layout);
    }
  }

  void Engine::AddVBoxLayout(const std::string& window, const std::string& name) {
    WidgetEngine::WindowHandle* handle = GetWindow(window);
    if (handle && handle->window && handle->window->isWindow()) {
      QVBoxLayout* layout = new QVBoxLayout(handle->window);
      layout->setObjectName(QString::fromStdString(name));
      layouts[name] = std::move(layout);
    }
  }
}
