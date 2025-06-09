#include <headers/LuaEngine.h>

void LuaEngine::RegisterWindow(const WidgetEngine::WindowInfo& info, sol::function on_frame) {
  engine.AddWindow(info);
  if (on_frame.valid()) {
    on_frame_callbacks.push_back(std::move(on_frame));
  }
}

void LuaEngine::RegisterButton(const std::string& window, const std::string& name, const std::string& text, sol::function on_frame) {
  engine.AddButton(window, name, text, WidgetEngine::WidgetAlignment::AlignmentNone);
  if (on_frame.valid()) {
    on_frame_callbacks.push_back(std::move(on_frame));
  }
}

void LuaEngine::CallOnFrameCallbacks() {
  auto it = on_frame_callbacks.begin();
  while (it != on_frame_callbacks.end()) {
    sol::protected_function_result result = (*it)();
    if (!result.valid()) {
      sol::error err = result;
      std::cerr << "Lua error: " << err.what() << std::endl;
      it = on_frame_callbacks.erase(it);  
    } else {
      ++it;
    }
  }
}

void LuaEngine::ProcessChildWidget(sol::table widget, const std::string& window) {
  sol::optional<std::string> type = widget["__type"];
  if (!type) {
    std::cerr << "Widget has no type" << std::endl;
    return;
  } 

  std::string widgetType = type.value();

  if (widgetType == "Button") {
    sol::optional<sol::function> on_frame = widget["on_frame"];
    RegisterButton(window, widget.get_or("name", std::string("")), widget.get_or("text", std::string("")),  on_frame.value_or(sol::nil));
  }
}

void LuaEngine::RegisterBindings() {
  lua["stacking_order"] = lua.create_table_with(
    "top", WidgetEngine::StackingOrder::Top,
    "bottom", WidgetEngine::StackingOrder::Bottom,
    "background", WidgetEngine::StackingOrder::Background,
    "overlay", WidgetEngine::StackingOrder::Overlay,
    "none", WidgetEngine::StackingOrder::None
  );

  lua["anchor_zone"] = lua.create_table_with(
    "top", WidgetEngine::AnchorZone::Top,
    "bottom", WidgetEngine::AnchorZone::Bottom,
    "left", WidgetEngine::AnchorZone::Left,
    "right", WidgetEngine::AnchorZone::Right,
    "none", WidgetEngine::AnchorZone::None
  );

  lua["layout"] = lua.create_table_with(
    "hbox", WidgetEngine::WindowLayout::HorizontalBox,
    "vbox", WidgetEngine::WindowLayout::VerticalBox,
    "grid", WidgetEngine::WindowLayout::Grid,
    "none", WidgetEngine::WindowLayout::None
  );


  lua["anim_curve"] = lua.create_table_with(
    "in_quad", WidgetEngine::AnimCurve::InQuad,
    "out_quad", WidgetEngine::AnimCurve::OutQuad,
    "in_out_quad", WidgetEngine::AnimCurve::InOutQuad,
    "in_cubic", WidgetEngine::AnimCurve::InCubic,
    "out_cubic", WidgetEngine::AnimCurve::OutCubic,
    "in_out_cubic", WidgetEngine::AnimCurve::InOutCubic
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

  lua["monitor"] = lua.new_usertype<WidgetEngine::MonitorInfo>("MonitorInfo",
    "width", &WidgetEngine::MonitorInfo::width,
    "height", &WidgetEngine::MonitorInfo::height,
    "name", &WidgetEngine::MonitorInfo::name,
    "index", &WidgetEngine::MonitorInfo::index
  );

  lua.set_function("ButtonCpp", [this](sol::table args) {
    sol::optional<sol::function> on_frame = args["on_frame"];
    RegisterButton(args.get_or("window", std::string("")), args.get_or("name", std::string("")), args.get_or("text", std::string("")), on_frame.value_or(sol::nil));
  });

  lua.set_function("Window", [this](sol::table args) {
    WidgetEngine::WindowInfo info;

    info.name = args.get_or("name", std::string(""));
    info.screen = args.get_or("screen", 0);
    info.width = args.get_or("width", 800);
    info.height = args.get_or("height", 600);
    info.scope = args.get_or("scope", std::string(""));
    info.order = args.get_or("order", WidgetEngine::StackingOrder::None);
    info.anchorArea = args.get_or("anchor_area", 0);
    info.paddingOuter = args.get_or("padding_outer", 0);
    info.paddingInner = args.get_or("padding_inner", 0);
    info.anchorZone = args.get_or("anchor_zone", WidgetEngine::AnchorZone::None);
    info.layout = args.get_or("layout", WidgetEngine::WindowLayout::None);

    sol::optional<sol::function> on_frame = args["on_frame"];
    RegisterWindow(info, on_frame.value_or(sol::nil));

    bool visible = args.get_or("visible", true);
    if (visible) {
      engine.ShowWindow(info.name);
    }

    for (auto& kv : args) {
      sol::object k = kv.first;
      sol::object v = kv.second;

      if (v.get_type() == sol::type::table) {
        sol::table child = v.as<sol::table>();

        sol::optional<std::string> type = child["__type"];
        if (type) {
          ProcessChildWidget(child, info.name);
        }
      }
    }
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

  lua.script(R"(
    function Button (args)
      args = args or {}
      args.__type = "Button"

      return args
    end
  )");

  RegisterBindings();

  std::string scriptPath = std::string(getenv("HOME")) + "/.config/blackboard";

  for (const auto& entry : std::filesystem::directory_iterator(scriptPath)) {
    if (entry.path().extension() == ".lua") {
      try {
        lua.script_file(entry.path().string());
      } catch (const sol::error& err) {
        std::cerr << "Lua error: " << err.what() << std::endl;
      }
    }
  }

  frameTimer = new QTimer(nullptr);
  QObject::connect(frameTimer, &QTimer::timeout, frameTimer, [this]() {
    CallOnFrameCallbacks();
  });
  frameTimer->start(1000 / 120);
}

LuaEngine::~LuaEngine() {
  delete frameTimer;
}

int LuaEngine::Exec() {
  return engine.Exec();
}
