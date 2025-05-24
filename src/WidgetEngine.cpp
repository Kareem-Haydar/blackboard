#include <headers/WidgetEngine.h>

WidgetEngine::WidgetEngine(int argc, char** argv) : app(argc, argv) {
  LayerShellQt::Shell::useLayerShell();
}

void WidgetEngine::AddWindow(const WindowInfo& info) {
  std::unique_ptr<WindowHandle> handle = std::make_unique<WindowHandle>();

  handle->window = new QWidget();

  handle->window->show();
  handle->window->hide();

  handle->window->setWindowTitle(QString::fromStdString(info.name));
  handle->window->resize(info.width, info.height);

  switch (info.layout) {
    case WidgetEngine::WindowLayout::HorizontalBox:
      handle->layout = new QHBoxLayout(handle->window);
      break;
    case WidgetEngine::WindowLayout::VerticalBox:
      handle->layout = new QVBoxLayout(handle->window);
      break;
    case WidgetEngine::WindowLayout::Grid:
      handle->layout = new QGridLayout(handle->window);
      break;
    default:
      break;
  }

  if (handle->layout) {
    handle->frame = new QFrame(handle->window);
    handle->layout->addWidget(handle->frame);
    //handle->frame->setLayout(handle->layout);
  } else {
    std::cerr << "unknown window layout" << std::endl;
  }

  if (info.order != WidgetEngine::StackingOrder::None) {
    if (handle->window->windowHandle()) {
      if (auto* lsh = LayerShellQt::Window::get(handle->window->windowHandle())) {
        lsh->setScope(QString::fromStdString(info.scope));

        switch (info.order) {
          case WidgetEngine::StackingOrder::Top:
            lsh->setLayer(LayerShellQt::Window::Layer::LayerTop);
            break;
          case WidgetEngine::StackingOrder::Bottom:
            lsh->setLayer(LayerShellQt::Window::Layer::LayerBottom);
            break;
          case WidgetEngine::StackingOrder::Background:
            lsh->setLayer(LayerShellQt::Window::Layer::LayerBackground);
            break;
          case WidgetEngine::StackingOrder::Overlay:
            lsh->setLayer(LayerShellQt::Window::Layer::LayerOverlay);
            break;
          case WidgetEngine::StackingOrder::None:
            break;
          default:
            break;
        }


        if (info.anchorArea > 0 ) {
          if (info.paddingOuter >= info.anchorArea) {
            throw std::runtime_error("anchor padding is greater than anchor area");
          }

          switch (info.anchorZone) {
            case WidgetEngine::AnchorZone::None:
              lsh->setAnchors({});
              break;
            default:
              handle->window->setFixedHeight(info.anchorArea - info.paddingOuter);
              lsh->setExclusiveZone(info.anchorArea);
              lsh->setAnchors(static_cast<LayerShellQt::Window::Anchor>(info.anchorZone));
              lsh->setMargins(QMargins(0, info.paddingInner, 0, 0));
              break;
          }
        }
      }
    }
  }

  handle->window->resize(info.width, info.height);
  windows.emplace(info.name, std::move(handle));
}

WidgetEngine::WindowHandle* WidgetEngine::GetWindow(const std::string& name) {
  auto it = windows.find(name);
  if (it != windows.end() && it->second->window && it->second->window->isWindow()) {
    return it->second.get();
  }
  return nullptr;
}

void WidgetEngine::ShowAll() {
  for (auto& window : windows) {
    if (window.second->window && window.second->window->isWindow()) {
      window.second->window->show();
    }
  }
}

void WidgetEngine::HideAll() {
  for (auto& window : windows) {
    if (window.second->window && window.second->window->isWindow()) {
      window.second->window->hide();
    }
  }
}

int WidgetEngine::Exec() {
  return app.exec();
}
