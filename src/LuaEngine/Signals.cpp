#include <LuaEngine/LuaEngine.h>

namespace LuaEngine {
  void Engine::EmitSignal(const std::string& signal) {
      if (BL_DEBUG) {
        std::cout << "emitting signal " << signal << " from " << caller_id << std::endl;
        std::cout << "\n";
      }

      sol::table widget = widget_registry.find(caller_id)->second;
      std::string emitter_type = widget["__widget_type"];

      userdata data = { widget["id"], emitter_type };
      lua_signals[signal] = data;
      for (const auto& kv : signal_listeners) {
        std::string id = kv.first;

        sol::optional<std::string> parent = widget_registry[id]["parent"];
        std::string parent_str = parent.value_or("");

        std::string listener_type = widget_registry[id]["__widget_type"];
        sol::table self = widget_registry[id];

        if (BL_DEBUG) {
          std::cout << "calling on_signal from " << id << "\n";
        }

        SetWidgetMetatable(self, id, listener_type, parent_str);
        kv.second(self, signal, caller_id);
      }

      if (BL_DEBUG) {
        std::cout << "\n";
      }
  }
};
