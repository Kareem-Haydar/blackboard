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

/**
 * @class Button
 * @brief button struct
 *
 */
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

/**
 * @class WidgetEngine
 * @brief widget engine abstraction on top of Qt6
 *
 */
class WidgetEngine {
  public:
    /**
     * @brief Qt6 layer shell stacking order abstraction enum
     */
    enum class StackingOrder {
        Top = 0,
        Bottom = 1,
        Background = 2,
        Overlay = 3,
        None = 4
    };

    /**
     * @brief Qt6 layer shell anchor zone abstraction enum
     */
    enum AnchorZone {
      Top = LayerShellQt::Window::Anchor::AnchorTop,
      Bottom = LayerShellQt::Window::Anchor::AnchorBottom,
      Left = LayerShellQt::Window::Anchor::AnchorLeft,
      Right = LayerShellQt::Window::Anchor::AnchorRight,
      None = LayerShellQt::Window::Anchor::AnchorNone
    };

    /**
     * @brief Qt6 window layout abstraction enum
     */
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

    /**
     * @class WindowInfo
     * @brief info struct for creating WindowHandles
     *
     */
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

    /**
     * @class Widget
     * @brief base widget struct
     *
     */
    struct Widget {
      std::string name;
    };


    struct Label : public Widget {
      QLabel* label = nullptr;
      std::string text;
    };

    /**
     * @class WindowHandle
     * @brief simple Qt6 window hand;e
     *
     */
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

    /**
     * @brief executes the Qt6 event loop
     *
     * @return 
     */
    int Exec();

    std::vector<MonitorInfo> GetMonitors();

    /**
     * @brief creates a new window handle and adds it to the table
     *
     * @param info 
     */
    void AddWindow(const WindowInfo& info);

    /**
     * @brief retrieves a window handle based on its name
     *
     * @param name 
     * @return 
     */
    WindowHandle* GetWindow(const std::string& name);

    /**
     * @brief returns the size of a window
     *
     * @param name 
     */
    std::array<int, 2> GetWindowSize(const std::string& name);

    /**
     * @brief resizes a window
     *
     * @param name 
     * @param width 
     * @param height 
     */
    void ResizeWindow(const std::string& name, unsigned int width, unsigned int height);

    /**
     * @brief shows a window
     *
     * @param name 
     */
    void ShowWindow(const std::string& name);

    /**
     * @brief hides a window
     *
     * @param name 
     */
    void HideWindow(const std::string& name);

    /**
     * @brief adds a label to a window
     *
     * @param window 
     * @param name 
     * @param text 
     * @param alignment 
     */
    void AddLabel(const std::string& window, const std::string& name, const std::string& text, const WidgetAlignment& alignment);
    
    /**
     * @brief adds a button to a window
     *
     * @param window 
     * @param name 
     * @param text 
     * @param alignment 
     */
    void AddButton(const std::string& window, const std::string& name, const std::string& text, const WidgetAlignment& alignment, std::function<void(Qt::MouseButton)> onClick, std::function<void()> hoverEnter, std::function<void()> hoverLeave);

    /**
     * @brief resizes a widget
     *
     * @param window 
     * @param name 
     * @param width 
     * @param height 
     */
    void ResizeWidget(const std::string& window, const std::string& name, unsigned int width, unsigned int height);

    /**
     * @brief moves a widget
     *
     * @param window 
     * @param name 
     * @param x 
     * @param y 
     */
    void MoveWidget(const std::string& window, const std::string& name, unsigned int x, unsigned int y);

    /**
     * @brief sets the style sheet of a widget
     *
     * @param window 
     * @param name 
     * @param styleSheet 
     */
    void SetWidgetStyleSheet(const std::string& window, const std::string& name, const std::string& styleSheet);
      
    /**
     * @brief sets the style sheet of a window
     *
     * @param window 
     * @param styleSheet 
     */
    void SetWindowStyleSheet(const std::string& window, const std::string& styleSheet);

    /**
     * @brief adds a property animation
     *
     * @param name 
     * @param window 
     * @param widget 
     * @param property 
     * @param duration 
     * @param curve 
     */
    void AddAnimation(const std::string& name, const std::string& window, const std::string& widget, const std::string& property, unsigned int duration, int curve);

    /**
     * @brief starts an animation
     *
     * @param animation 
     */
    void StartAnimation(const std::string& animation);

    /**
     * @brief stops an animation
     *
     * @param animation 
     */
    void StopAnimation(const std::string& animation);

    /**
     * @brief sets the start value of an animation (rectangle)
     *
     * @param animation 
     * @param x 
     * @param y 
     * @param width 
     * @param height 
     */
    void SetAnimStartValueRect(const std::string& animation, unsigned int x, unsigned int y, unsigned int width, unsigned int height);

    /**
     * @brief sets the end value of an animation (rectangle)
     *
     * @param animation 
     * @param x 
     * @param y 
     * @param width 
     * @param height 
     */
    void SetAnimEndValueRect(const std::string& animation, unsigned int x, unsigned int y, unsigned int width, unsigned int height);

    void SetAnimDirection(const std::string& animation, bool forward);

    /**
     * @brief shows all windows
     */
    void ShowAll();

    /**
     * @brief hides all windows
     */
    void HideAll();

  private:
    QApplication app;
    std::unordered_map<std::string, std::unique_ptr<QPropertyAnimation>> animations;
};
