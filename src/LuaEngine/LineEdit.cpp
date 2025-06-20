#include <LuaEngine/LuaEngine.h>

namespace LuaEngine {
  void Engine::RegisterLineEdit(const std::string& parent, sol::table args) {
    sol::optional<std::string> id = args["id"];
    sol::optional<std::string> placeholder = args["text"];
    sol::optional<sol::function> on_enter = args["on_enter"];
    sol::optional<sol::function> on_frame = args["on_frame"];
    sol::optional<sol::function> on_signal = args["on_signal"];

    if (BL_DEBUG) {
      std::cout << "registering line edit " << id.value() << " with parent " << parent << "\n";
      std::cout << "\t placeholder text: " << placeholder.value() << "\n";
      std::cout << "\n";
    }

    std::function<void(const std::string&)> fn_on_enter = [this, id, on_enter, parent] (const std::string& text) {
      if (on_enter) {
        sol::function cb = on_enter.value();
        if (cb.valid()) {
          auto it = widget_registry.find(id.value());
          if (it != widget_registry.end()) {
            if (BL_DEBUG) {
              std::cout << "calling on_enter for " << id.value() << "\n";
              std::cout << "\n";
            }

            sol::table self = it->second;
            SetWidgetMetatable(self, id.value(), self["__widget_type"], parent);
            caller_id = id.value();
            cb(self, text);
            caller_id = "";
          }
        }
      }
    };

    if (on_frame) {
      sol::function cb = on_frame.value();
      if (cb.valid()) {
        on_frame_callbacks[id.value()] = cb;
      }
    }

    if (on_signal) {
      sol::function cb = on_signal.value();
      if (cb.valid()) {
        signal_listeners[id.value()] = cb;
      }
    }

    engine.AddLineEdit(parent, id.value(), placeholder.value(), WidgetEngine::WidgetAlignment::AlignmentNone, fn_on_enter, false);

    HandleStates(args, parent);
    SetWidgetMetatable(args, id.value(), "line_edit", parent, true);

    // add widget to global registry
    widget_registry[id.value()] = args;
  }
}
