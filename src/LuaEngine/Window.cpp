#include <LuaEngine/LuaEngine.h>

namespace LuaEngine {
  void Engine::RegisterWindow(sol::table args) {
    std::string id = args.get_or("id", std::string(""));

    if (BL_DEBUG) {
      std::cout << "registering window " << id << "\n";
      std::cout << "\t screen: " << args.get_or("screen", 0) << "\n";
      std::cout << "\t width: " << args.get_or("width", 800) << "\n";
      std::cout << "\t height: " << args.get_or("height", 600) << "\n";
      std::cout << "\t scope: " << args.get_or("scope", std::string("")) << "\n";
      std::cout << "\t order: " << static_cast<int>(args.get_or("order", WidgetEngine::StackingOrder::None)) << "\n";
      std::cout << "\t anchor_area: " << args.get_or("anchor_area", 0) << "\n";
      std::cout << "\t padding_outer: " << args.get_or("padding_outer", 0) << "\n";
      std::cout << "\t padding_inner: " << args.get_or("padding_inner", 0) << "\n";
      std::cout << "\t anchors: " << args.get_or("anchors", WidgetEngine::AnchorZone::None) << "\n";
      std::cout << "\t layout: " << static_cast<int>(args.get_or("layout", WidgetEngine::WindowLayout::None)) << "\n";
      std::cout << "\n";
    }

    // create and populate WindowInfo object
    WidgetEngine::WindowInfo info;

    info.name = id;
    info.screen = args.get_or("screen", 0);
    info.width = args.get_or("width", 800);
    info.height = args.get_or("height", 600);
    info.scope = args.get_or("scope", std::string(""));
    info.order = args.get_or("order", WidgetEngine::StackingOrder::None);
    info.anchorArea = args.get_or("anchor_area", 0);
    info.paddingOuter = args.get_or("padding_outer", 0);
    info.paddingInner = args.get_or("padding_inner", 0);
    info.anchorZone = args.get_or("anchors", WidgetEngine::AnchorZone::None);
    info.layout = args.get_or("layout", WidgetEngine::WindowLayout::None);

    // create and show window
    engine.AddWindow(info);
    engine.ShowWindow(info.name);

    // make sure required fields are set
    sol::optional<std::string> widget_type = args["__widget_type"];
    if (!widget_type) {
      args.raw_set("__widget_type", "window");
    }

    sol::optional<std::string> parent = args["parent"];
    if (!parent) {
      args.raw_set("parent", "");
    }

    // handle animation states
    HandleStates(args);

    // set metatable
    SetWidgetMetatable(args, id, "window", "", true);

    // add widget to global registry
    widget_registry[id] = args;
  }
}
