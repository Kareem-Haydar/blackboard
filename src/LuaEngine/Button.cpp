#include <LuaEngine/LuaEngine.h>

namespace LuaEngine {
  void Engine::RegisterButton(const std::string& parent, sol::table args, bool layout) {
    std::string id = args.get_or("id", std::string(""));
    std::string text = args.get_or("text", std::string("Button"));
    sol::optional<sol::table> size_policy = args["size_policy"];
    sol::optional<sol::function> on_click = args["on_click"];
    sol::optional<sol::function> hover_enter = args["hover_enter"];
    sol::optional<sol::function> hover_leave = args["hover_leave"];

    if (BL_DEBUG) {
      std::cout << "registering button " << id << " with parent " << parent << "\n";
      std::cout << "\t text: " << text << "\n";
      std::cout << "\n";
    }

    // make sure required fields are set
    sol::optional<std::string> widget_type = args["__widget_type"];
    if (!widget_type) {
      args.raw_set("__widget_type", "button");
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
            CallWidgetPreHooks("on_click");
            CallAnimPreHooks("on_click");
            cb(self, static_cast<int>(btn));
            CallAnimPostHooks("on_click");
            CallWidgetPostHooks("on_click");
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
            CallWidgetPreHooks("hover_enter");
            CallAnimPreHooks("hover_enter");
            cb(self);
            CallAnimPostHooks("hover_enter");
            CallWidgetPostHooks("hover_enter");
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
            CallWidgetPreHooks("hover_leave");
            CallAnimPreHooks("hover_leave");
            cb(self);
            CallAnimPostHooks("hover_leave");
            CallWidgetPostHooks("hover_leave");
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

    engine.AddButton(parent, info);

    // add widget to global registry
    widget_registry[id] = args;

    HandleStates(args, parent);
    SetWidgetMetatable(args, id, "button", parent, true);
    ProcessAnimations(args, parent);
  }
}
