#pragma once

namespace LuaEngine {
    // userdata for signals
    struct userdata {
      std::string id;
      std::string type;
    };

    // animation state enums
    enum StateType {
      Stylesheet = 0,
      Rect,
      Size,
    };

    struct State {
      StateType type;
      std::string widget_id;
    };

    struct SizeState : public State {
      int w;
      int h;
    };

    struct RectState : public State {
      int x;
      int y;
      int w;
      int h;
    };

    struct StylesheetState : public State {
      std::string stylesheet;
    };

    struct Widget {
      // general widget properties
      std::string id;
      std::string type;
      std::string parent;

      // animation states
      std::unordered_map<std::string, std::shared_ptr<State>> states;

      // widget specific properties
      std::unordered_map<std::string, sol::object> specific_properties;

      // widget specific callbacks
      std::unordered_map<std::string, sol::function> specific_callbacks;
    };

    struct PropertyAnimation {
      std::string widget_id;
      std::string property;
      std::string start;
      std::string end;
      bool interruptible;
      int duration;
      WidgetEngine::AnimCurve curve;
      sol::function when;
    };

    struct SizeAnimation : public PropertyAnimation {
      QSize start_size;
      QSize end_size;
    };
}
