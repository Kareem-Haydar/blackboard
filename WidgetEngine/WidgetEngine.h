#include <unordered_map>
#include <iostream>
#include <string>

#include <WidgetEngine/Widgets.h>
#include <WidgetEngine/Structs.h>

#pragma once

namespace WidgetEngine {
  class Engine {
    public:
      std::unordered_map<std::string, std::unique_ptr<::WidgetEngine::WindowHandle>> windows; 
      
      Engine(int argc, char** argv);
      ~Engine() = default;

      int Exec();

      std::vector<::WidgetEngine::MonitorInfo> GetMonitors();

      void AddWindow(const ::WidgetEngine::WindowInfo& info);
      ::WidgetEngine::WindowHandle* GetWindow(const std::string& name);

      std::array<int, 2> GetWindowSize(const std::string& name);
      void ResizeWindow(const std::string& name, unsigned int width, unsigned int height);

      void ShowWindow(const std::string& name);
      void HideWindow(const std::string& name);

      void AddLabel(const std::string& window, const std::string& name, const std::string& text, const ::WidgetEngine::WidgetAlignment& alignment, bool elevated);
      void AddLineEdit(const std::string& window, const std::string& name, const std::string& placeholderText, const ::WidgetEngine::WidgetAlignment& alignment, std::function<void(const std::string&)> onEnter, bool elevated);
      void AddButton(const std::string& window, const ::WidgetEngine::ButtonInfo& info);
      void AddListView(const std::string& window, const std::string& name, const std::vector<QString>& items, const ::WidgetEngine::WidgetAlignment& alignment, std::function<void(Qt::MouseButton, int)> onSelect, bool elevated);

      std::array<int, 2> GetWidgetSize(const std::string& window, const std::string& name);
      void ResizeWidget(const std::string& window, const std::string& name, unsigned int width, unsigned int height);
      void MoveWidget(const std::string& window, const std::string& name, unsigned int x, unsigned int y);

      void SetWidgetStyleSheet(const std::string& window, const std::string& name, const std::string& styleSheet);
      void SetWindowStyleSheet(const std::string& window, const std::string& styleSheet);

      void AnimateWidgetGeometryOnce(const std::string& window, const std::string& name, QRect start, QRect end, unsigned int duration, ::WidgetEngine::AnimCurve curve);
      void AnimateWidgetMaxSizeOnce(const std::string& window, const std::string& name, QSize start, QSize end, unsigned int duration, ::WidgetEngine::AnimCurve curve);
      void AnimateWidgetMinSizeOnce(const std::string& window, const std::string& name, QSize start, QSize end, unsigned int duration, ::WidgetEngine::AnimCurve curve);
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
};
