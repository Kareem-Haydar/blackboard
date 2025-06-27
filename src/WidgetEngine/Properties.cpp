#include <WidgetEngine/WidgetEngine.h>

namespace WidgetEngine {
  std::array<int, 2> Engine::GetWidgetSize(const std::string& window, const std::string& name) {
    if (auto* handle = GetWindow(window)) {
      if (handle->window && handle->window->isWindow()) {
        if (auto* widget = handle->window->findChild<QWidget*>(QString::fromStdString(name))) {
          return {widget->width(), widget->height()};
        }
      }
    }
    return {-1, -1};
  }

  void Engine::ResizeWidget(const std::string& window, const std::string& name, unsigned int width, unsigned int height) {
    if (auto* handle = GetWindow(window)) {
      if (handle->window && handle->window->isWindow()) {
        if (auto* widget = handle->window->findChild<QWidget*>(QString::fromStdString(name))) {
          widget->resize(width, height);
        }
      }
    }
  }

  void Engine::MoveWidget(const std::string& window, const std::string& name, unsigned int x, unsigned int y) {
    if (auto* handle = GetWindow(window)) {
      if (handle->window && handle->window->isWindow()) {
        if (auto* widget = handle->window->findChild<QWidget*>(QString::fromStdString(name))) {
          widget->move(x, y);
        }
      }
    }
  }

  void Engine::SetWidgetStyleSheet(const std::string& window, const std::string& name, const std::string& styleSheet) {
    if (auto* handle = GetWindow(window)) {
      if (handle->window && handle->window->isWindow()) {
        if (auto* widget = handle->window->findChild<QWidget*>(QString::fromStdString(name))) {
          widget->setStyleSheet(QString::fromStdString(styleSheet));
        }
      }
    }
  }

  void Engine::SetWindowStyleSheet(const std::string& window, const std::string& styleSheet) {
    if (auto* handle = GetWindow(window)) {
      if (handle->window && handle->window->isWindow()) {
        handle->window->setStyleSheet(QString::fromStdString(styleSheet));
      }
    }
  }

  WidgetEngine::WindowHandle* Engine::GetWindow(const std::string& name) {
    auto it = windows.find(name);
    if (it != windows.end() && it->second) {
      if (it->second->window && it->second->window->isWindow()) {
        return it->second.get();
      }
    }
    return nullptr;
  }

  void Engine::ShowAll() {
    for (auto& window : windows) {
      if (window.second->window && window.second->window->isWindow()) {
        window.second->window->show();
      }
    }
  }

  void Engine::HideAll() {
    for (auto& window : windows) {
      if (window.second->window && window.second->window->isWindow()) {
        window.second->window->hide();
      }
    }
  }

  void Engine::ShowWindow(const std::string& name) {
    WidgetEngine::WindowHandle* handle = GetWindow(name);
    if (handle && handle->window && handle->window->isWindow()) {
      handle->window->show();
    }
  }

  void Engine::HideWindow(const std::string& name) {
    WidgetEngine::WindowHandle* handle = GetWindow(name);
    if (handle && handle->window && handle->window->isWindow()) {
      handle->window->hide();
    }
  }

  std::array<int, 2> Engine::GetWindowSize(const std::string& name) {
    WidgetEngine::WindowHandle* handle = GetWindow(name);
    if (handle && handle->window && handle->window->isWindow()) {
      return {handle->window->width(), handle->window->height()};
    }
    return {-1, -1};
  }

  void Engine::ResizeWindow(const std::string& name, unsigned int width, unsigned int height) {
    WidgetEngine::WindowHandle* handle = GetWindow(name);
    if (handle && handle->window && handle->window->isWindow()) {
      handle->window->resize(width, height);
    }
  }

  void Engine::SetWidgetSizePolicy(const std::string& window, const std::string& name, SizePolicy horizontal, SizePolicy vertical) {
    WidgetEngine::WindowHandle* handle = GetWindow(window);
    if (handle && handle->window && handle->window->isWindow()) {
      if (auto* widget = handle->window->findChild<QWidget*>(QString::fromStdString(name))) {
        widget->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(horizontal), static_cast<QSizePolicy::Policy>(vertical)));
      }
    }
  }
};
