#include <LuaEngine/LuaEngine.h>

namespace LuaEngine {
  void Engine::RegisterLabel(const std::string& parent, sol::table args, bool layout) {
    std::string id = args.get_or("id", std::string(""));
    std::string text = args.get_or("text", std::string("Label"));
    sol::optional<sol::table> size_policy = args["size_policy"];

    if (BL_DEBUG) {
      std::cout << "registering label " << id << " with parent " << parent << "\n";
      std::cout << "\t text: " << text << "\n";
      std::cout << "\n";
    }

    sol::optional<std::string> widget_type = args["__widget_type"];
    if (!widget_type) {
      args.raw_set("__widget_type", "label");
    }

    WidgetEngine::LabelInfo info;
    info.name = id;
    info.text = text;
    info.elevated = false;
    info.allignment = WidgetEngine::WidgetAlignment::AlignmentNone;
    info.layout = layout;

    if (size_policy) {
      sol::table tbl = size_policy.value();
      sol::optional<WidgetEngine::SizePolicy> horizontal = tbl["horizontal"];
      sol::optional<WidgetEngine::SizePolicy> vertical = tbl["vertical"];

      if (horizontal) {
        info.horizontal = horizontal.value();
      }
      if (vertical) {
        info.vertical = vertical.value();
      }
    }

    engine.AddLabel(parent, info);

    widget_registry[id] = args;

    ProcessAnimations(args, parent);
    HandleStates(args, parent);
    SetWidgetMetatable(args, id, "label", parent, true);
  }
}
