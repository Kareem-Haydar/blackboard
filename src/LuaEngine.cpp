#include <headers/LuaEngine.h>

void LuaEngine::RegisterBindings() {

  lua["StackingOrderCpp"] = lua.create_table_with(
    "Top", WidgetEngine::StackingOrder::Top,
    "Bottom", WidgetEngine::StackingOrder::Bottom,
    "Background", WidgetEngine::StackingOrder::Background,
    "Overlay", WidgetEngine::StackingOrder::Overlay,
    "None", WidgetEngine::StackingOrder::None
  );

  lua["AnchorZoneCpp"] = lua.create_table_with(
    "Top", WidgetEngine::AnchorZone::Top,
    "Bottom", WidgetEngine::AnchorZone::Bottom,
    "Left", WidgetEngine::AnchorZone::Left,
    "Right", WidgetEngine::AnchorZone::Right,
    "None", WidgetEngine::AnchorZone::None
  );

  lua["WindowLayoutCpp"] = lua.create_table_with(
    "HorizontalBox", WidgetEngine::WindowLayout::HorizontalBox,
    "VerticalBox", WidgetEngine::WindowLayout::VerticalBox,
    "Grid", WidgetEngine::WindowLayout::Grid,
    "None", WidgetEngine::WindowLayout::None
  );


  lua["AnimCurveCpp"] = lua.create_table_with(
    "inQuad", WidgetEngine::AnimCurve::InQuad,
    "outQuad", WidgetEngine::AnimCurve::OutQuad,
    "inOutQuad", WidgetEngine::AnimCurve::InOutQuad,
    "inCubic", WidgetEngine::AnimCurve::InCubic,
    "outCubic", WidgetEngine::AnimCurve::OutCubic,
    "inOutCubic", WidgetEngine::AnimCurve::InOutCubic
  );

  lua["WindowInfoCpp"] = lua.new_usertype<WidgetEngine::WindowInfo>("WindowInfo",
    sol::constructors<WidgetEngine::WindowInfo()>(),
    "name", &WidgetEngine::WindowInfo::name,
    "screen", &WidgetEngine::WindowInfo::screen,
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

  lua["WindowHandle"] = lua.new_usertype<WidgetEngine::WindowHandle>("WindowHandle",
    "window", &WidgetEngine::WindowHandle::window ,
    "frame", &WidgetEngine::WindowHandle::frame,
    "layout", &WidgetEngine::WindowHandle::layout
  );

  lua["MonitorInfoCpp"] = lua.new_usertype<WidgetEngine::MonitorInfo>("MonitorInfo",
    "width", &WidgetEngine::MonitorInfo::width,
    "height", &WidgetEngine::MonitorInfo::height,
    "name", &WidgetEngine::MonitorInfo::name,
    "index", &WidgetEngine::MonitorInfo::index
  );

  lua.set_function("GetMonitorsCpp", [this]() {
    return engine.GetMonitors();
  });

  lua.set_function("AddWindowCpp", [this](const WidgetEngine::WindowInfo& info) {
    engine.AddWindow(info);
  });

  lua.set_function("GetWindowCpp", [this](const std::string& name) -> sol::optional<WidgetEngine::WindowHandle*> {
      WidgetEngine::WindowHandle* handle = engine.GetWindow(name);
      if (handle) {
        return handle;
      } else {
        return sol::nullopt;
      }
  });

  lua.set_function("AddButtonCpp", [this](const std::string& window, const std::string& name, const std::string& text) {
    engine.AddButton(window, name, text, WidgetEngine::WidgetAlignment::AlignmentNone);
  });

  lua.set_function("AddLabelCpp", [this](const std::string& window, const std::string& name, const std::string& text) {
    engine.AddLabel(window, name, text, WidgetEngine::WidgetAlignment::AlignmentNone);    
  });

  lua.set_function("MoveWidgetCpp", [this](const std::string& window, const std::string& name, unsigned int x, unsigned int y) {
    engine.MoveWidget(window, name, x, y);
  });

  lua.set_function("ResizeWidgetCpp", [this](const std::string& window, const std::string& name, unsigned int width, unsigned int height) {
    engine.ResizeWidget(window, name, width, height);
  });

  lua.set_function("SetWidgetStyleSheet", [this](const std::string& window, const std::string& name, const std::string& styleSheet) {
    engine.SetWidgetStyleSheet(window, name, styleSheet);
  });

  lua.set_function("SetWindowStyleSheet", [this](const std::string& window, const std::string& styleSheet) {
    engine.SetWindowStyleSheet(window, styleSheet);
  });

  lua.set_function("ShowWindowCpp", [this](const std::string& name) {
      engine.ShowWindow(name);
  });

  lua.set_function("HideWindowCpp", [this](const std::string& name) {
    engine.HideWindow(name);
  });

  lua.set_function("ResizeWindow", [this](const std::string& name, unsigned int width, unsigned int height) {
    engine.ResizeWindow(name, width, height);    
  });

  lua.set_function("GetWindowSize", [this](const std::string& name) {
      return engine.GetWindowSize(name);
  });

  lua.set_function("AddAnimation", [this](const std::string& name, const std::string& window, const std::string& widget, const std::string& property, unsigned int duration, int curve) {
    engine.AddAnimation(name, window, widget, property, duration, curve);    
  });

  lua.set_function("SetAnimStartValueRect", [this](const std::string& animation, unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
    engine.SetAnimStartValueRect(animation, x, y, width, height);
  });

  lua.set_function("SetAnimEndValueRect", [this](const std::string& animation, unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
    engine.SetAnimEndValueRect(animation, x, y, width, height);
  });

  lua.set_function("SetAnimDirection", [this](const std::string& animation, bool forward) {
    engine.SetAnimDirection(animation, forward);
  });

  lua.set_function("StartAnimation", [this](const std::string& animation) {
    engine.StartAnimation(animation);
  });

  lua.set_function("StopAnimation", [this](const std::string& animation) {
    engine.StopAnimation(animation);
  });

  lua.set_function("ShowAllCpp", [this]() {
    engine.ShowAll();
  });

  lua.set_function("HideAllCpp", [this]() {
    engine.HideAll();
  });

  lua.set_function("ExecCpp", [this]() {
    return engine.Exec();
  });
}

LuaEngine::LuaEngine(int argc, char** argv) : engine(argc, argv) {
  lua.open_libraries(
    sol::lib::base,
    sol::lib::package,
    sol::lib::coroutine,
    sol::lib::string,
    sol::lib::os,
    sol::lib::math,
    sol::lib::table,
    sol::lib::debug,
    sol::lib::io,
    sol::lib::utf8
  );

  lua.script(R"(
    local home = os.getenv("HOME")
    package.path = package.path .. home .. "/.luarocks/share/lua/5.4/?.lua;" .. home .. "/.luarocks/share/lua/5.4/?/init.lua;" .. home .. "/luarocks/share/lua/5.4/?.lua;" .. home .. "/Desktop/Projects/blackboard/src/scripts/?.lua;"
    package.cpath = package.cpath .. home .. "/.luarocks/lib/lua/5.4/?.so;" .. home .. "/.luarocks/lib/lua/5.4/?.so;"
  )");


  RegisterBindings();
}

void LuaEngine::RunScript(const std::string& scriptPath) {
  try {
    lua.script_file(scriptPath);
  } catch (const sol::error& err) {
    std::cerr << "Lua error: " << err.what() << std::endl;
  }
}
