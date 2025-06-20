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

        if (type == "window") {
          RegisterWindow(table);
          ProcessChildWidgets(table, id);
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
        sol::optional<std::string> type = t["__widget_type"];

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
