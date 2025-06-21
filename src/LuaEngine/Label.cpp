#include <LuaEngine/LuaEngine.h>

namespace LuaEngine {
  void Engine::RegisterLabel(const std::string& parent, sol::table args) {
    std::string id = args.get_or("id", std::string(""));
    std::string text = args.get_or("text", std::string("Label"));

    sol::optional<sol::function> on_frame = args["on_frame"];
    sol::optional<sol::function> on_signal = args["on_signal"];

    sol::optional<std::string> widget_type = args["__widget_type"];
    if (!widget_type) {
      args.raw_set("__widget_type", "label");
    }

    WidgetEngine::LabelInfo info;
    info.name = id;
    info.text = text;
    info.elevated = false;
    info.allignment = WidgetEngine::WidgetAlignment::AlignmentNone;

    engine.AddLabel(parent, info);

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

    widget_registry[id] = args;
  }
}
