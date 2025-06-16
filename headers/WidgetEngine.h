#include <LayerShellQt/Shell>
#include <LayerShellQt/Window>

#include <qnamespace.h>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QApplication>
#include <QPushButton>
#include <QMouseEvent>
#include <QWidget>
#include <QWindow>
#include <QLayout>
#include <QObject>
#include <QFrame>
#include <QLabel>
#include <QTimer>

#include <qtmetamacros.h>
#include <unordered_map>
#include <iostream>
#include <string>

class Button : public QPushButton {
    Q_OBJECT
  public:
    using QPushButton::QPushButton;

    ~Button();
    Button(QWidget* parent = nullptr);

  signals:
    void Clicked(Qt::MouseButton btn);
    void HoverEnter();
    void HoverLeave();

  protected:
    void mousePressEvent(QMouseEvent* e) override {
      QPushButton::mousePressEvent(e);
      emit Clicked(e->button());
    }

    bool event(QEvent* e) override {
      switch (e->type()) {
        case QEvent::HoverEnter:
          emit HoverEnter();
          break;
        case QEvent::HoverLeave:
          emit HoverLeave();
          break;
        default:
          break;
      }
      return QPushButton::event(e);
    }
};

class WidgetEngine {
  public:
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

    struct Widget {
      std::string name;
    };

    struct Label : public Widget {
      QLabel* label = nullptr;
      std::string text;
    };

    struct WindowHandle {
      QWidget* window = nullptr;
      QFrame* frame = nullptr;
      QLayout* layout = nullptr;
      std::vector<Widget> widgets;
    };

    struct MonitorInfo {
      int index;
      int width;
      int height;
      std::string name;
    };

    std::unordered_map<std::string, std::unique_ptr<WindowHandle>> windows; 
    
    WidgetEngine(int argc, char** argv);
    ~WidgetEngine() = default;

    int Exec();

    std::vector<MonitorInfo> GetMonitors();

    void AddWindow(const WindowInfo& info);
    WindowHandle* GetWindow(const std::string& name);

    std::array<int, 2> GetWindowSize(const std::string& name);
    void ResizeWindow(const std::string& name, unsigned int width, unsigned int height);

    void ShowWindow(const std::string& name);
    void HideWindow(const std::string& name);

    void AddLabel(const std::string& window, const std::string& name, const std::string& text, const WidgetAlignment& alignment, bool homogeneous = true);
    void AddButton(const std::string& window, const std::string& name, const std::string& text, const WidgetAlignment& alignment, std::function<void(Qt::MouseButton)> onClick, std::function<void()> hoverEnter, std::function<void()> hoverLeave, bool homogeneous = true);

    std::array<int, 2> GetWidgetSize(const std::string& window, const std::string& name);
    void ResizeWidget(const std::string& window, const std::string& name, unsigned int width, unsigned int height);
    void MoveWidget(const std::string& window, const std::string& name, unsigned int x, unsigned int y);

    void SetWidgetStyleSheet(const std::string& window, const std::string& name, const std::string& styleSheet);
    void SetWindowStyleSheet(const std::string& window, const std::string& styleSheet);

    void AnimateWidgetGeometryOnce(const std::string& window, const std::string& name, QRect start, QRect end, unsigned int duration, AnimCurve curve);
    void AnimateWidgetMaxSizeOnce(const std::string& window, const std::string& name, QSize start, QSize end, unsigned int duration, AnimCurve curve);
    void AnimateWidgetMinSizeOnce(const std::string& window, const std::string& name, QSize start, QSize end, unsigned int duration, AnimCurve curve);
    void AddAnimation(const std::string& name, const std::string& window, const std::string& widget, const std::string& property, unsigned int duration, int curve);
    void StartAnimation(const std::string& animation);
    void StopAnimation(const std::string& animation);
    void SetAnimStartValueRect(const std::string& animation, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
    void SetAnimEndValueRect(const std::string& animation, unsigned int x, unsigned int y, unsigned int width, unsigned int height);

    void SetAnimDirection(const std::string& animation, bool forward);

    void ShowAll();
    void HideAll();

  private:
    QApplication app;
    std::unordered_map<std::string, std::unique_ptr<QPropertyAnimation>> animations;
};
