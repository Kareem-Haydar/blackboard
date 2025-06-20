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
}
