#include <WidgetEngine/Widgets.h>

#include <LayerShellQt/Shell>
#include <LayerShellQt/Window>

#pragma once

namespace WidgetEngine {
  enum class StackingOrder {
    Top = 0,
    Bottom = 1,
    Background = 2,
    Overlay = 3,
    None = 4
  };

  enum AnchorZone {
    Top = LayerShellQt::Window::Anchor::AnchorTop,
    Bottom = LayerShellQt::Window::Anchor::AnchorBottom,
    Left = LayerShellQt::Window::Anchor::AnchorLeft,
    Right = LayerShellQt::Window::Anchor::AnchorRight,
    None = LayerShellQt::Window::Anchor::AnchorNone
  };

  enum class WindowLayout {
    None = 0,
    HorizontalBox = 1,
    VerticalBox = 2,
    Grid = 3,
    //Form = 4
  };

  enum WidgetAlignment {
    CenterH = Qt::AlignHCenter,
    CenterV = Qt::AlignVCenter,
    TopV = Qt::AlignTop,
    BottomV = Qt::AlignBottom,
    LeftH = Qt::AlignLeft,
    RightH = Qt::AlignRight,
    AlignmentNone = 0,
  };

  enum AnimCurve {
    Linear = QEasingCurve::Linear,
    InQuad = QEasingCurve::InQuad,
    OutQuad = QEasingCurve::OutQuad,
    InOutQuad = QEasingCurve::InOutQuad,
    InCubic = QEasingCurve::InCubic,
    OutCubic = QEasingCurve::OutCubic,
    InOutCubic = QEasingCurve::InOutCubic
  };

  struct WindowInfo {
    std::string name;
    int screen;
    unsigned int width = 800;
    unsigned int height = 600;
    std::string scope;
    StackingOrder order = StackingOrder::None;
    unsigned int anchorArea = 0;
    unsigned int paddingOuter = 0;
    unsigned int paddingInner = 0;
    int anchorZone;
    WindowLayout layout = WindowLayout::HorizontalBox;
  };

  struct ButtonInfo {
    std::string name;
    std::string text;
    std::function<void(Qt::MouseButton)> onClick;
    std::function<void()> hoverEnter;
    std::function<void()> hoverLeave;
    WidgetAlignment alignment;
    bool elevated;
  };

  struct LabelInfo {
    std::string name;
    std::string text;
    WidgetAlignment allignment;
    bool elevated;
  };

  struct WindowHandle {
    QWidget* window = nullptr;
    QFrame* frame = nullptr;
    QLayout* layout = nullptr;
  };

  struct MonitorInfo {
    int index;
    int width;
    int height;
    std::string name;
  };
}
