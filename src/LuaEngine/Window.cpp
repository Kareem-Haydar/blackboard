#include <LuaEngine/LuaEngine.h>

namespace LuaEngine {
  void Engine::RegisterWindow(sol::table args) {
    sol::optional<std::string> id = args["id"];
    sol::optional<int> screen = args["screen"];
    sol::optional<std::string> scope = args["scope"];
    sol::optional<WidgetEngine::StackingOrder> order = args["order"];
    sol::optional<int> anchor_area = args["anchor_area"];
    sol::optional<int> padding_outer = args["padding_outer"];
    sol::optional<int> padding_inner = args["padding_inner"];
    sol::optional<WidgetEngine::AnchorZone> anchors = args["anchors"];
    sol::optional<WidgetEngine::WindowLayout> layout = args["layout"];
    sol::optional<int> width = args["width"];
    sol::optional<int> height = args["height"];

    if (BL_DEBUG) {
      std::cout << "registering window " << id.value() << "\n";
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

    if (!id) {
      std::cerr << "Tried to register window with no id\n";
      return;
    }
    
    // make sure required fields are set
    sol::optional<std::string> widget_type = args["__widget_type"];
    if (!widget_type) {
      args.raw_set("__widget_type", "window");
    }

    sol::optional<std::string> parent = args["parent"];
    if (!parent) {
      args.raw_set("parent", "");
    }

    // create and populate WindowInfo object
    WidgetEngine::WindowInfo info;

    info.name = id.value();
    info.screen = screen.value_or(0);
    info.width = width.value_or(800);
    info.height = height.value_or(600);
    info.scope = scope.value_or(std::string(""));
    info.order = order.value_or(WidgetEngine::StackingOrder::None);
    info.anchorArea = anchor_area.value_or(0);
    info.paddingOuter = padding_outer.value_or(0);
    info.paddingInner = padding_inner.value_or(0);
    info.anchorZone = anchors.value_or(WidgetEngine::AnchorZone::None);
    info.layout = layout.value_or(WidgetEngine::WindowLayout::None);

    // create and show window
    engine.AddWindow(info);
    engine.ShowWindow(info.name);

    // add widget to global registry
    widget_registry[id.value()] = args;

    HandleStates(args);
    SetWidgetMetatable(args, id.value(), "window", "", true);
  }
}
