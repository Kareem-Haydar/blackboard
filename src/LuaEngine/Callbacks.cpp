#include <LuaEngine/LuaEngine.h>

namespace LuaEngine {
  void Engine::CallOnFrameCallbacks() {
    auto it = on_frame_callbacks.begin();
    while (it != on_frame_callbacks.end()) {
      const std::string& widget_id = it->first;
      
      auto registry_it = widget_registry.find(widget_id);
      if (registry_it == widget_registry.end()) {
        ++it;
        continue;
      }

      if (BL_DEBUG) {
        std::cout << "calling on_frame for " << widget_id << "\n";
      }
      
      sol::table self = registry_it->second;

      SetWidgetMetatable(self, self["id"], self["__widget_type"], "");
      
      sol::protected_function_result result = it->second(self);
      if (!result.valid()) {
        sol::error err = result;
        it = on_frame_callbacks.erase(it);  
      } else {
        ++it;
      }
    }
  }
}
