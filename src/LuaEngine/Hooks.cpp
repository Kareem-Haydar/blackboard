#include <LuaEngine/LuaEngine.h>

namespace LuaEngine {
  void Engine::RegisterHook(const std::string& widget_id, sol::table hook) {
    sol::optional<std::string> widget = hook["widget"];
    sol::optional<std::string> event = hook["event"];
    sol::optional<std::string> fn_name = hook["fn_name"];
    sol::optional<sol::function> cb = hook["cb"];

    if (!widget || !event || !fn_name || !cb) {
      std::cerr << "invalid hook\n";
      return;
    }

    if (BL_DEBUG) {
      std::cout << "registering hook for " << widget_id << "\n";
      std::cout << "\t widget: " << widget.value() << "\n";
      std::cout << "\t event: " << event.value() << "\n";
      std::cout << "\t fn_name: " << fn_name.value() << "\n";
      std::cout << "\n";
    }

    if (widget && widget.value() == "") {
      if (event && event.value() == "pre") {
        global_pre_hooks[widget_id].push_back(hook);
      } else if (event && event.value() == "post") {
        global_post_hooks[widget_id].push_back(hook);
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
        widget_pre_hooks[widget_id].push_back(hook);
      } else if (event && event.value() == "post") {
        widget_post_hooks[widget_id].push_back(hook);
      } else {
        std::cerr << "invalid event\n";
        return;
      }
    }
  }

  void Engine::ProcessHooks() {
    for (const auto& kv : widget_registry) {
      sol::table widget = kv.second;
      std::string id = kv.first;

      sol::optional<sol::table> hooks = widget["hooks"];
      if (hooks) {
        for (const auto& hook_kv : hooks.value()) {
          RegisterHook(id, hook_kv.second);
        }
      }
    }
  }

  void Engine::CallWidgetPreHooks(const std::string& fn_name) {
    if (BL_DEBUG) {
      std::cout << "calling pre hooks for " << fn_name << " in widget " << caller_id << "\n";
    }

    for (const auto& kv : widget_pre_hooks) {
      for (const auto& hook : kv.second) {
        std::string widget_id = hook["widget"];
        sol::function cb = hook["cb"];
        std::string hook_fn_name = hook["fn_name"];

        if (hook_fn_name == fn_name && caller_id == widget_id) {
          sol::table self = widget_registry[kv.first];
          sol::protected_function_result result = cb(self);
          if (!result.valid()) {
            sol::error err = result;
            std::cerr << "error calling pre hook: " << err.what() << "\n";
            break;
          }
        }
      }
    }
  }

  void Engine::CallWidgetPostHooks(const std::string& fn_name) {
    if (BL_DEBUG) {
      std::cout << "calling post hooks for " << fn_name << " in widget " << caller_id << "\n";
    }

    for (const auto& kv : widget_post_hooks) {
      for (const auto& hook : kv.second) {
        sol::function cb = hook["cb"];
        std::string hook_fn_name = hook["fn_name"];
        std::string widget_id = hook["widget"];

        if (hook_fn_name == fn_name && caller_id == widget_id) {
          sol::table self = widget_registry[kv.first];
          sol::protected_function_result result = cb(self);
          if (!result.valid()) {
            sol::error err = result;
            std::cerr << "error calling post hook: " << err.what() << "\n";
            break;
          }
        }
      }
    }
  }
}
