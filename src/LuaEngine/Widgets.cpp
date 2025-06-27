#include <LuaEngine/LuaEngine.h>

namespace LuaEngine {
  void Engine::ProcessTopLevelWidgets() {
    for (auto& kv : lua_registry) {
      sol::object key = kv.first;
      sol::object val = kv.second;

      if (val.get_type() == sol::type::table) {
        sol::table table = val.as<sol::table>();

        sol::optional<std::string> widget_type = table["__widget_type"];
        if (!widget_type) 
          continue;

        std::string type = widget_type.value();
        std::string id = table["id"];
        
        sol::optional<sol::function> on_frame = table["on_frame"];
        sol::optional<sol::function> on_signal = table["on_signal"];

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

        if (type == "window") {
          RegisterWindow(table);
          ProcessChildWidgets(table, id);
        } 
      }
    }
  }

  void Engine::ProcessLayoutWidgets(sol::table widget, const std::string& parent) {
    for (auto& kv : widget) {
      sol::object key = kv.first;
      sol::object val = kv.second;

      if (val.get_type() == sol::type::table) {
        sol::table t = val.as<sol::table>();

        std::string id = t["id"];
        sol::optional<std::string> type = t["__widget_type"];

        if (!type)
          continue;

        sol::optional<sol::function> on_frame = t["on_frame"];
        sol::optional<sol::function> on_signal = t["on_signal"];

        if (on_frame && type.value() != "vbox_layout" && type.value() != "hbox_layout") {
          sol::function cb = on_frame.value();
          if (cb.valid()) {
            on_frame_callbacks[id] = cb;
          }
        }

        if (on_signal && type.value() != "vbox_layout" && type.value() != "hbox_layout") {
          sol::function cb = on_signal.value();
          if (cb.valid()) {
            signal_listeners[id] = cb;
          }
        }

        if (type.value() == "hbox_layout") {
          RegisterHBoxLayout(parent, t);
          ProcessLayoutWidgets(t, id);
        }

        if (type.value() == "vbox_layout") {
          RegisterVBoxLayout(parent, t);
          ProcessLayoutWidgets(t, id);
        }

        if (type && type.value() == "button") {
          std::string layout_id = widget["id"];
          RegisterButton(layout_id, t, true);
        }

        if (type && type.value() == "label") {
          RegisterLabel(parent, t, true);
        }

        if (type && type.value() == "line_edit") {
          RegisterLineEdit(parent, t);
        }
      }
    }
  }

  void Engine::ProcessChildWidgets(sol::table widget, const std::string& parent) {
    for (auto& kv : widget) {
      sol::object key = kv.first;
      sol::object val = kv.second;

      if (val.get_type() == sol::type::table) {
        sol::table t = val.as<sol::table>();

        std::string id = t["id"];
        sol::optional<std::string> type = t["__widget_type"];

        if (!type)
          continue;

        sol::optional<sol::function> on_frame = t["on_frame"];
        sol::optional<sol::function> on_signal = t["on_signal"];

        if (on_frame && type.value() != "vbox_layout" && type.value() != "hbox_layout") {
          sol::function cb = on_frame.value();
          if (cb.valid()) {
            on_frame_callbacks[id] = cb;
          }
        }

        if (on_signal && type.value() != "vbox_layout" && type.value() != "hbox_layout") {
          sol::function cb = on_signal.value();
          if (cb.valid()) {
            signal_listeners[id] = cb;
          }
        }

        if (type.value() == "hbox_layout") {
          RegisterHBoxLayout(parent, t);
          ProcessLayoutWidgets(t, parent);
        }

        if (type.value() == "vbox_layout") {
          RegisterVBoxLayout(parent, t);
          ProcessLayoutWidgets(t, parent);
        }

        if (type && type.value() == "button") {
          RegisterButton(parent, t);
        }

        if (type && type.value() == "label") {
          RegisterLabel(parent, t);
        }

        if (type && type.value() == "line_edit") {
          RegisterLineEdit(parent, t);
        }
      }
    }
  }
}
