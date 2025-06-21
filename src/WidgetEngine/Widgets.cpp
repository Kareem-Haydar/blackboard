#include <WidgetEngine/WidgetEngine.h>

namespace WidgetEngine {
  void Engine::AddWindow(const WindowInfo& info) {
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

  void Engine::AddButton(const std::string& window, const ButtonInfo& info) {
    WidgetEngine::WindowHandle* handle = GetWindow(window);
    if (handle && handle->window && handle->window->isWindow()) {
      Button* btn = new Button(handle->window);
      btn->show();
      btn->setObjectName(info.name);
      btn->setText(QString::fromStdString(info.text));
      if (handle->layout) {
        //btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        handle->layout->addWidget(btn);
      }

      if (info.elevated) {
        btn->raise();
      }

      if (info.onClick) {
        QObject::connect(btn, &Button::Clicked, [info] (Qt::MouseButton button) {
          info.onClick(button);
        });
      }

      if (info.hoverEnter) {
        QObject::connect(btn, &Button::HoverEnter, [info] () {
          info.hoverEnter();
        });
      }

      if (info.hoverLeave) {
        QObject::connect(btn, &Button::HoverLeave, [info] () {
          info.hoverLeave();
        });
      }
    }
  }

  void Engine::AddLineEdit(const std::string& window, const std::string& name, const std::string& placeholderText, const WidgetAlignment& alignment, std::function<void(const std::string&)> onEnter, bool elevated) {
    WidgetEngine::WindowHandle* handle = GetWindow(window);
    if (handle && handle->window->isWindow()) {
      QLineEdit* lineEdit = new QLineEdit(handle->window);
      lineEdit->show();
      lineEdit->setObjectName(QString::fromStdString(name));
      lineEdit->setPlaceholderText(QString::fromStdString(placeholderText));
      if (elevated) {
        lineEdit->raise();
      }
      if (onEnter) {
        QObject::connect(lineEdit, &QLineEdit::returnPressed, [onEnter, lineEdit] () {
          onEnter(lineEdit->text().toStdString());
        });
      }
      if (handle->layout) {
        handle->layout->addWidget(lineEdit);
      }
    }
  }

  void Engine::AddLabel(const std::string& window, const WidgetEngine::LabelInfo& info) {
    WidgetEngine::WindowHandle* handle = GetWindow(window);
    if (handle && handle->window && handle->window->isWindow()) {
      QLabel* label = new QLabel(handle->window);
      label->show();
      label->setObjectName(QString::fromStdString(info.name));
      label->setText(QString::fromStdString(info.text));
      if (info.elevated) {
        label->raise();
      }

      if (handle->layout) {
        //label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        handle->layout->addWidget(label);
      }
    }
  }
};
