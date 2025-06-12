#include <headers/LuaEngine.h>

void LuaEngine::RegisterWindow(sol::table args) {
  WidgetEngine::WindowInfo info;

  std::string id = args.get_or("id", std::string(""));

  info.name = id;
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

  engine.AddWindow(info);
  engine.ShowWindow(info.name);

  sol::optional<sol::function> on_frame = args["on_frame"];
  if (on_frame) {
    if (on_frame.value().valid()) {
      on_frame_callbacks.insert(std::make_pair(id, on_frame.value()));
    }
  }

  widget_registry.insert(std::make_pair(id, args));
}

void LuaEngine::CallOnFrameCallbacks() {
  auto it = on_frame_callbacks.begin();
  while (it != on_frame_callbacks.end()) {
    sol::table self = widget_registry.find(it->first)->second;
    sol::protected_function_result result = it->second(self);
    if (!result.valid()) {
      sol::error err = result;
      std::cerr << "Lua error: " << err.what() << std::endl;
      it = on_frame_callbacks.erase(it);  
    } else {
      ++it;
    }
  }
}

void LuaEngine::ProcessTopLevelWidgets() {
  for (auto& kv : lua_registry) {
    sol::object key = kv.first;
    sol::object val = kv.second;

    if (val.get_type() == sol::type::table) {
      sol::table table = val.as<sol::table>();

      sol::optional<std::string> widget_type = table["__widget_type"];
      if (!widget_type) 
        continue;

      std::string type = widget_type.value();

      if (type == "window") {
        RegisterWindow(table);
      } 
    }
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

  lua["mouse_button"] = lua.create_table_with(
    "none", Qt::NoButton,
    "left", Qt::LeftButton,
    "right", Qt::RightButton,
    "middle", Qt::MiddleButton
  );

  lua["monitor"] = lua.new_usertype<WidgetEngine::MonitorInfo>("MonitorInfo",
    "width", &WidgetEngine::MonitorInfo::width,
    "height", &WidgetEngine::MonitorInfo::height,
    "name", &WidgetEngine::MonitorInfo::name,
    "index", &WidgetEngine::MonitorInfo::index
  );

  lua.set_function("__process_widgets", [this]() {
    ProcessTopLevelWidgets();
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

  lua_registry = lua.create_table();
  lua["__widget_list"] = lua_registry;

  lua.script(R"(
    local home = os.getenv("HOME")
    package.path = package.path .. home .. "/.luarocks/share/lua/5.4/?.lua;" .. home .. "/.luarocks/share/lua/5.4/?/init.lua;" .. home .. "/luarocks/share/lua/5.4/?.lua;" .. home .. "/Desktop/Projects/blackboard/src/scripts/?.lua;"
    package.cpath = package.cpath .. home .. "/.luarocks/lib/lua/5.4/?.so;" .. home .. "/.luarocks/lib/lua/5.4/?.so;"
  )");

  RegisterBindings();

  lua.script(R"(
    function Window (args)
      args = args or {}
      args.__widget_type = "window"
      args.id = args.id or ("window_" .. tostring(#__widget_list + 1))
      table.insert(__widget_list, args)

      return args
    end
  )");

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

  lua.script("__process_widgets()");

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
