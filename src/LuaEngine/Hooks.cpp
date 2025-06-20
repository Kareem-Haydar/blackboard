#include <LuaEngine/LuaEngine.h>

namespace LuaEngine {
  void Engine::RegisterHooks(const std::string& widget_id, sol::table hooks) {
    sol::optional<std::string> widget = hooks["widget"];
    sol::optional<std::string> event = hooks["event"];
    if (widget && widget.value() == "") {
      if (event && event.value() == "pre") {
        global_pre_hooks[widget_id] = hooks;
      } else if (event && event.value() == "post") {
        global_post_hooks[widget_id] = hooks;
      } else {
        std::cerr << "invalid event\n";
        return;
      }
    } else {
      auto widget_handle = widget_registry.find(widget.value());
      if (widget_handle == widget_registry.end()) {
        std::cerr << "invalid widget\n";
        return;
      }

      if (event && event.value() == "pre") {
        widget_pre_hooks[widget_id] = hooks;
      } else if (event && event.value() == "post") {
        widget_post_hooks[widget_id] = hooks;
      } else {
        std::cerr << "invalid event\n";
        return;
      }
    }
  }
}
