#pragma once

#include <LayerShellQt/Shell>
#include <LayerShellQt/Window>

#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QWindow>
#include <QLayout>
#include <QFrame>
#include <QLabel>

#include <unordered_map>
#include <iostream>
#include <string>

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

    /**
     * @class WindowInfo
     * @brief info struct for creating WindowHandles
     *
     */
    struct WindowInfo {
      std::string name;
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

    /**
     * @class Button
     * @brief button struct
     *
     */
    struct Button : public Widget {
      QPushButton* button = nullptr;
      std::string text;
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

    
    WidgetEngine(int argc, char** argv);
    ~WidgetEngine() = default;

    /**
     * @brief executes the Qt6 event loop
     *
     * @return 
     */
    int Exec();

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

    void AddLabel(const std::string& window, const std::string& name, const std::string& text, const WidgetAlignment& alignment);
    
    void AddButton(const std::string& window, const std::string& name, const std::string& text, const WidgetAlignment& alignment);

    void ResizeWidget(const std::string& window, const std::string& name, unsigned int width, unsigned int height);

    void MoveWidget(const std::string& window, const std::string& name, unsigned int x, unsigned int y);

    void SetWidgetStyleSheet(const std::string& window, const std::string& name, const std::string& styleSheet);
    void SetWindowStyleSheet(const std::string& window, const std::string& styleSheet);

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
    std::unordered_map<std::string, std::unique_ptr<WindowHandle>> windows; 
};
