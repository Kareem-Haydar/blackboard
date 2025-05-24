#include <headers/LuaEngine.h>

void LuaEngine::ShowWindow(const std::string& name) {
  WidgetEngine::WindowHandle* handle = engine.GetWindow(name);
  if (handle && handle->window && handle->window->isWindow()) handle->window->show();
}

void LuaEngine::HideWindow(const std::string& name) {
  WidgetEngine::WindowHandle* handle = engine.GetWindow(name);
  if (handle && handle->window && handle->window->isWindow()) handle->window->hide();
}

void LuaEngine::RegisterBindings() {
  lua["blackboard"] = lua.create_table();

  lua["blackboard"]["StackingOrder"] = lua.create_table_with(
    "Top", WidgetEngine::StackingOrder::Top,
    "Bottom", WidgetEngine::StackingOrder::Bottom,
    "Background", WidgetEngine::StackingOrder::Background,
    "Overlay", WidgetEngine::StackingOrder::Overlay,
    "None", WidgetEngine::StackingOrder::None
  );

  lua["blackboard"]["AnchorZone"] = lua.create_table_with(
    "Top", WidgetEngine::AnchorZone::Top,
    "Bottom", WidgetEngine::AnchorZone::Bottom,
    "Left", WidgetEngine::AnchorZone::Left,
    "Right", WidgetEngine::AnchorZone::Right,
    "None", WidgetEngine::AnchorZone::None
  );

  lua["blackboard"]["WindowLayout"] = lua.create_table_with(
    "HorizontalBox", WidgetEngine::WindowLayout::HorizontalBox,
    "VerticalBox", WidgetEngine::WindowLayout::VerticalBox,
    "Grid", WidgetEngine::WindowLayout::Grid
  );

  lua["blackboard"]["WindowInfo"] = lua.new_usertype<WidgetEngine::WindowInfo>("WindowInfo",
    sol::constructors<WidgetEngine::WindowInfo()>(),
    "name", &WidgetEngine::WindowInfo::name,
    "scope", &WidgetEngine::WindowInfo::scope,
    "width", &WidgetEngine::WindowInfo::width,
    "height", &WidgetEngine::WindowInfo::height,
    "order", &WidgetEngine::WindowInfo::order,
    "anchorArea", &WidgetEngine::WindowInfo::anchorArea,
    "paddingOuter", &WidgetEngine::WindowInfo::paddingOuter,
    "paddingInner", &WidgetEngine::WindowInfo::paddingInner,
    "anchorZone", &WidgetEngine::WindowInfo::anchorZone,
    "layout", &WidgetEngine::WindowInfo::layout
  );

  lua["blackboard"]["WindowHandle"] = lua.new_usertype<WidgetEngine::WindowHandle>("WindowHandle",
    "window", &WidgetEngine::WindowHandle::window ,
    "frame", &WidgetEngine::WindowHandle::frame,
    "layout", &WidgetEngine::WindowHandle::layout
  );

  lua["blackboard"]["AddWindow"] = lua.set_function("AddWindow", [this](const WidgetEngine::WindowInfo& info) {
    engine.AddWindow(info);
  });

  lua["blackboard"]["GetWindow"] = lua.set_function("GetWindow", [this](const std::string& name) -> sol::optional<WidgetEngine::WindowHandle*> {
      WidgetEngine::WindowHandle* handle = engine.GetWindow(name);
      if (handle) {
        return handle;
      } else {
        return sol::nullopt;
      }
  });

  lua["blackboard"]["ShowWindow"] = lua.set_function("ShowWindow", [this](const std::string& name) {
    this->ShowWindow(name);
  });

  lua["blackboard"]["HideWindow"] = lua.set_function("HideWindow", [this](const std::string& name) {
    this->HideWindow(name);
  });

  lua["blackboard"]["ShowAll"] = lua.set_function("ShowAll", [this]() {
    engine.ShowAll();
  });

  lua["blackboard"]["HideAll"] = lua.set_function("HideAll", [this]() {
    engine.HideAll();
  });

  lua["blackboard"]["Exec"] = lua.set_function("Exec", [this]() {
    return engine.Exec();
  });
}

LuaEngine::LuaEngine(int argc, char** argv) : engine(argc, argv) {
  lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::os);
  RegisterBindings();
}

void LuaEngine::RunScript(const std::string& scriptPath) {
  try {
    lua.script_file(scriptPath);
  } catch (const sol::error& err) {
    std::cerr << "Lua error: " << err.what() << std::endl;
  }
}
