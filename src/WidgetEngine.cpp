#include <headers/WidgetEngine.h>

Button::Button(QWidget* parent) : QPushButton(parent) {
  setAttribute(Qt::WA_Hover);
}

Button::~Button() {}

WidgetEngine::WidgetEngine(int argc, char** argv) : app(argc, argv) {
  LayerShellQt::Shell::useLayerShell();
}

std::vector<WidgetEngine::MonitorInfo> WidgetEngine::GetMonitors() {
  const auto screens = QApplication::screens();

  std::vector<MonitorInfo> info;
  info.resize(screens.size());

  for (int i = 0; i < screens.size(); i++) {
    info[i].index = i;
    info[i].width = screens[i]->geometry().width();
    info[i].height = screens[i]->geometry().height();
    info[i].name = screens[i]->name().toStdString();
  }

  return info;
}

void WidgetEngine::AddWindow(const WindowInfo& info) {
  std::unique_ptr<WindowHandle> handle = std::make_unique<WindowHandle>();

  handle->window = new QWidget();

  handle->window->show();
  handle->window->hide();

  if (info.screen >=0) {
    handle->window->windowHandle()->setScreen(QApplication::screens()[info.screen]);
  }

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
  } else if (info.layout != WidgetEngine::WindowLayout::None) {
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

void WidgetEngine::ShowWindow(const std::string& name) {
  WidgetEngine::WindowHandle* handle = GetWindow(name);
  if (handle && handle->window && handle->window->isWindow()) {
    handle->window->show();
  }
}

void WidgetEngine::HideWindow(const std::string& name) {
  WidgetEngine::WindowHandle* handle = GetWindow(name);
  if (handle && handle->window && handle->window->isWindow()) {
    handle->window->hide();
  }
}

std::array<int, 2> WidgetEngine::GetWindowSize(const std::string& name) {
  WidgetEngine::WindowHandle* handle = GetWindow(name);
  if (handle && handle->window && handle->window->isWindow()) {
    return {handle->window->width(), handle->window->height()};
  }
  return {-1, -1};
}

void WidgetEngine::ResizeWindow(const std::string& name, unsigned int width, unsigned int height) {
  WidgetEngine::WindowHandle* handle = GetWindow(name);
  if (handle && handle->window && handle->window->isWindow()) {
    handle->window->resize(width, height);
  }
}

void WidgetEngine::AddButton(const std::string& window, const std::string& name, const std::string& text, const WidgetAlignment& alignment, std::function<void(Qt::MouseButton)> onClick, std::function<void()> hoverEnter, std::function<void()> hoverLeave, bool homogeneous) {
  WidgetEngine::WindowHandle* handle = GetWindow(window);
  if (handle && handle->window && handle->window->isWindow()) {
    Button* btn = new Button(handle->window);
    btn->show();
    btn->setObjectName(name);
    btn->setText(QString::fromStdString(text));
    if (handle->layout) {
      //btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
      handle->layout->addWidget(btn);
    }

    if (onClick) {
      QObject::connect(btn, &Button::Clicked, [onClick] (Qt::MouseButton button) {
        onClick(button);
      });
    }

    if (hoverEnter) {
      QObject::connect(btn, &Button::HoverEnter, [hoverEnter] () {
        hoverEnter();
      });
    }

    if (hoverLeave) {
      QObject::connect(btn, &Button::HoverLeave, [hoverLeave] () {
        hoverLeave();
      });
    }
    //QPushButton* button = new QPushButton(handle->window);
    //button->show();
    //button->setObjectName(QString::fromStdString(name));
    //button->setText(QString::fromStdString(text));
    //if (handle->layout) {
    //  handle->layout->addWidget(button);
    //} 
  }
}

void WidgetEngine::AddLabel(const std::string& window, const std::string& name, const std::string& text, const WidgetAlignment& alignment, bool homogeneous) {
  WidgetEngine::WindowHandle* handle = GetWindow(window);
  if (handle && handle->window && handle->window->isWindow()) {
    QLabel* label = new QLabel(handle->window);
    label->show();
    label->setObjectName(QString::fromStdString(name));
    label->setText(QString::fromStdString(text));
    if (handle->layout) {
      label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
      handle->layout->addWidget(label);
    }
  }
}

std::array<int, 2> WidgetEngine::GetWidgetSize(const std::string& window, const std::string& name) {
  if (auto* handle = GetWindow(window)) {
    if (handle->window && handle->window->isWindow()) {
      if (auto* widget = handle->window->findChild<QWidget*>(QString::fromStdString(name))) {
        return {widget->width(), widget->height()};
      }
    }
  }
  return {-1, -1};
}

void WidgetEngine::ResizeWidget(const std::string& window, const std::string& name, unsigned int width, unsigned int height) {
  if (auto* handle = GetWindow(window)) {
    if (handle->window && handle->window->isWindow()) {
      if (auto* widget = handle->window->findChild<QWidget*>(QString::fromStdString(name))) {
        widget->resize(width, height);
      }
    }
  }
}

void WidgetEngine::MoveWidget(const std::string& window, const std::string& name, unsigned int x, unsigned int y) {
  if (auto* handle = GetWindow(window)) {
    if (handle->window && handle->window->isWindow()) {
      if (auto* widget = handle->window->findChild<QWidget*>(QString::fromStdString(name))) {
        widget->move(x, y);
      }
    }
  }
}

void WidgetEngine::SetWidgetStyleSheet(const std::string& window, const std::string& name, const std::string& styleSheet) {
  if (auto* handle = GetWindow(window)) {
    if (handle->window && handle->window->isWindow()) {
      if (auto* widget = handle->window->findChild<QWidget*>(QString::fromStdString(name))) {
        widget->setStyleSheet(QString::fromStdString(styleSheet));
      }
    }
  }
}

void WidgetEngine::SetWindowStyleSheet(const std::string& window, const std::string& styleSheet) {
  if (auto* handle = GetWindow(window)) {
    if (handle->window && handle->window->isWindow()) {
      handle->window->setStyleSheet(QString::fromStdString(styleSheet));
    }
  }
}

void WidgetEngine::AnimateWidgetGeometryOnce(const std::string& window, const std::string& name, QRect start, QRect end, unsigned int duration, AnimCurve curve) {
  WindowHandle* win = GetWindow(window);
  if (win && win->window && win->window->isWindow()) {
    if (auto* w = win->window->findChild<QWidget*>(QString::fromStdString(name))) {
      w->setGeometry(start);
      QPropertyAnimation* anim = new QPropertyAnimation(w, "geometry");
      QObject::connect(anim, &QPropertyAnimation::finished, anim, &QPropertyAnimation::deleteLater);
      anim->setDuration(duration);
      anim->setEasingCurve(QEasingCurve::Type(curve));
      anim->setStartValue(start);
      anim->setEndValue(end);
      anim->start();
    }
  }
}

void WidgetEngine::AnimateWidgetMaxSizeOnce(const std::string& window, const std::string& name, QSize start, QSize end, unsigned int duration, AnimCurve curve) {
  WindowHandle* win = GetWindow(window);
  if (win && win->window && win->window->isWindow()) {
    if (auto* w = win->window->findChild<QWidget*>(QString::fromStdString(name))) {
      w->setMaximumSize(start);
      QPropertyAnimation* anim = new QPropertyAnimation(w, "maximumSize");
      QObject::connect(anim, &QPropertyAnimation::finished, anim, &QPropertyAnimation::deleteLater);
      anim->setDuration(duration);
      anim->setEasingCurve(QEasingCurve::Type(curve));
      anim->setStartValue(start);
      anim->setEndValue(end);
      anim->start();
    }
  }
}

void WidgetEngine::AnimateWidgetMinSizeOnce(const std::string& window, const std::string& name, QSize start, QSize end, unsigned int duration, AnimCurve curve) {
  WindowHandle* win = GetWindow(window);
  if (win && win->window && win->window->isWindow()) {
    if (auto* w = win->window->findChild<QWidget*>(QString::fromStdString(name))) {
      w->setMinimumSize(start);
      QPropertyAnimation* anim = new QPropertyAnimation(w, "minimumSize");
      QObject::connect(anim, &QPropertyAnimation::finished, anim, &QPropertyAnimation::deleteLater);
      anim->setDuration(duration);
      anim->setEasingCurve(QEasingCurve::Type(curve));
      anim->setStartValue(start);
      anim->setEndValue(end);
      anim->start();
    }
  }
}

void WidgetEngine::AddAnimation(const std::string& name, const std::string& window, const std::string& widget, const std::string& property, unsigned int duration, int curve) {
  WindowHandle* win = GetWindow(window);
  if (win && win->window && win->window->isWindow()) {
    if (auto* w = win->window->findChild<QWidget*>(QString::fromStdString(widget))) {
      std::unique_ptr<QPropertyAnimation> anim(new QPropertyAnimation(w, QString::fromStdString(property).toUtf8()));
      anim->setDuration(duration);
      anim->setEasingCurve(QEasingCurve::Type(curve));

      animations.emplace(name, std::move(anim));
    }
  }
}

void WidgetEngine::StartAnimation(const std::string& animation) {
  auto it = animations.find(animation);
  if (it != animations.end()) {
    it->second->start();
  }
}

void WidgetEngine::StopAnimation(const std::string& animation) {
  auto it = animations.find(animation);
  if (it != animations.end()) {
    it->second->stop();
  }
}

void WidgetEngine::SetAnimStartValueRect(const std::string& animation, unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
  auto it = animations.find(animation);
  if (it != animations.end()) {
    it->second->setStartValue(QRect(x, y, width, height));
  }
}

void WidgetEngine::SetAnimEndValueRect(const std::string& animation, unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
  auto it = animations.find(animation);
  if (it != animations.end()) {
    it->second->setEndValue(QRect(x, y, width, height));
  }
}

void WidgetEngine::SetAnimDirection(const std::string& animation, bool forward) {
  auto it = animations.find(animation);
  if (it != animations.end()) {
    it->second->setDirection(forward ? QAbstractAnimation::Direction::Forward : QAbstractAnimation::Direction::Backward);
  }
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
