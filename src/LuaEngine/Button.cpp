#include <LuaEngine/LuaEngine.h>

namespace LuaEngine {
  void Engine::RegisterButton(const std::string& parent, sol::table args) {
    // get required fields
    std::string id = args.get_or("id", std::string(""));
    std::string text = args.get_or("text", std::string("Button"));

    if (BL_DEBUG) {
      std::cout << "registering button " << id << " with parent " << parent << "\n";
      std::cout << "\t text: " << text << "\n";
      std::cout << "\n";
    }

    // get callbacks
    sol::optional<sol::function> on_frame = args["on_frame"];
    sol::optional<sol::function> on_signal = args["on_signal"];
    sol::optional<sol::function> on_click = args["on_click"];
    sol::optional<sol::function> hover_enter = args["hover_enter"];
    sol::optional<sol::function> hover_leave = args["hover_leave"];

    // make sure required fields are set
    sol::optional<std::string> widget_type = args["__widget_type"];
    if (!widget_type) {
      args.raw_set("__widget_type", "window");
    }

    args.raw_set("parent", parent);

    // on click C++ function object
    std::function<void(Qt::MouseButton)> fn_on_click = [this, id, on_click, parent] (Qt::MouseButton btn) {
      if (on_click) {
        sol::function cb = on_click.value();
        if (cb.valid()) {
          auto it = widget_registry.find(id);
          if (it != widget_registry.end()) {
            if (BL_DEBUG) {
              std::cout << "calling on_click for " << id << "\n";
              std::cout << "\n";
            }

            sol::table self = it->second;
            std::string type = self["__widget_type"];
            SetWidgetMetatable(self, id, type, parent);
            caller_id = id;
            cb(self, static_cast<int>(btn));
            caller_id = "";
          }
        }
      }
    };

    // hover enter C++ function object
    std::function<void()> fn_hover_enter = [this, id, hover_enter, parent] () {
      if (hover_enter) {
        sol::function cb = hover_enter.value();
        if (cb.valid()) {
          auto it = widget_registry.find(id);
          if (it != widget_registry.end()) {
            if (BL_DEBUG) {
              std::cout << "calling hover_enter for " << id << "\n";
              std::cout << "\n";
            }

            sol::table self = it->second;
            SetWidgetMetatable(self, id, self["__widget_type"], parent);
            caller_id = id;
            cb(self);
            caller_id = "";
          }
        }
      }
    };

    // hover leave C++ function object
    std::function<void()> fn_hover_leave = [this, id, hover_leave, parent] () {
      if (hover_leave) {
        sol::function cb = hover_leave.value();
        if (cb.valid()) {
          auto it = widget_registry.find(id);
          if (it != widget_registry.end()) {
            if (BL_DEBUG) {
              std::cout << "calling hover_leave for " << id << "\n";
              std::cout << "\n";
            }

            sol::table self = it->second;
            SetWidgetMetatable(self, id, self["__widget_type"], parent);
            caller_id = id;
            cb(self);
            caller_id = "";
          }
        }
      }
    };

    WidgetEngine::ButtonInfo info;
    info.name = id;
    info.text = text;
    info.onClick = fn_on_click;
    info.hoverEnter = fn_hover_enter;
    info.hoverLeave = fn_hover_leave;
    info.alignment = WidgetEngine::WidgetAlignment::AlignmentNone;
    info.elevated = false;

    engine.AddButton(parent, info);


    // set callbacks if present
    if (on_frame) {
      sol::function cb = on_frame.value();
      if (cb.valid()) {
        on_frame_callbacks[id] = cb;
      }
    }

    if (on_signal) {
      sol::function cb = on_signal.value();
      if (cb.valid()) {
        signal_listeners[id] = cb;
      }
    }

    HandleStates(args, parent);
    ProcessAnimations(args, parent);
    SetWidgetMetatable(args, id, "button", parent, true);

    // add widget to global registry
    widget_registry[id] = args;
  }
}
