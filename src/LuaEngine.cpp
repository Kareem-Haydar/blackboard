#include <headers/LuaEngine.h>

void LuaEngine::SetWidgetMetatable(sol::table widget, const std::string& id, const std::string& type, const std::string& parent, bool overwrite) {
  // check if the metatable exists already
  sol::optional<sol::table> mt_exists = widget[sol::metatable_key];
  if (!mt_exists || overwrite) {
    sol::table mt = lua.create_table();
    
    // set newindex method
    mt.set_function("__newindex", [this, id, type, parent] (sol::table table, sol::object key, sol::object val) {
      // make sure key is a string
      if (key.is<std::string>()) {
        std::string k = key.as<std::string>();

        // handle string values
        if (val.is<std::string>()) {
          std::string v = val.as<std::string>();

          // make sure widget is valid
          if (type != "window" && parent == "") {
            std::cerr << "widgets other than windows must have a parent\n";
            return true;
          }

          // handle stylesheet changes
          if (k == "stylesheet") {
            if (type == "window") {
              engine.SetWindowStyleSheet(id, v);
            } else {
              engine.SetWidgetStyleSheet(parent, id, v);
            }
          } 
        // handle integer values
        } else if (val.is<int>()) {
          int v = val.as<int>();

          // makme sure widget is valid
          if (type != "window" && parent == "") {
            std::cerr << "widgets other than windows must have a parent\n";
            return true;
          }

          // handle geometry changes
          if (k == "width") {
            if (type == "window") {
              std::array<int, 2> size = engine.GetWindowSize(id);
              engine.ResizeWindow(id, v, size[1]);
            } else {
              std::array<int, 2> size = engine.GetWidgetSize(parent, id);
              engine.ResizeWidget(parent, id, v, size[1]);
            }
          } else if (k == "height") {
            if (type == "window") {
              std::array<int, 2> size = engine.GetWindowSize(id);
              engine.ResizeWindow(id, size[0], v);
            } else {
              std::array<int, 2> size = engine.GetWidgetSize(parent, id);
              engine.ResizeWidget(parent, id, size[0], v);
            }
          }
        }
      }

      return true;
    });

    // set metatable if it doesn't exist
    widget[sol::metatable_key] = mt;
  }
}

void LuaEngine::HandleStates(sol::table args, const std::string& parent) {
  // make sure widget is valid
  if (parent == "") {
    std::string type = args["__widget_type"];
    if (type != "window") {
      std::cerr << "widgets other than windows must have a parent\n";
      return;
    }
  }

  // get state id
  std::string id = args["id"];

  // check if widget has animation states
  sol::optional<sol::table> widget_states = args["states"];
  if (widget_states && widget_states->valid()) {
    // handle animation states
    std::unordered_map<std::string, std::shared_ptr<State>> state_map;
    sol::table tbl = widget_states.value();
    for (auto& kv : tbl) {
      sol::object key = kv.first;
      sol::object val = kv.second;

      if (val.is<sol::table>()) {
        sol::table v = val.as<sol::table>();
        sol::optional<std::string> property = v["property"];

        // handle state property
        if (property) {
          // handle stylesheet property
          if (property.value() == "stylesheet") {
            // create and populate StylesheetState object
            auto state = std::make_shared<StylesheetState>();
            state->widget_id = id;
            state->type = StateType::Stylesheet;

            sol::optional<std::string> state_id = v["id"];
            sol::optional<std::string> stylesheet = v["value"];

            state->stylesheet = stylesheet.value_or("");

            // add state to map
            state_map[state_id.value_or("")] = state;
          // handle geometry property
          } else if (property.value() == "geometry") {
            // create and populate RectState object
            auto state = std::make_shared<RectState>();
            state->widget_id = id;
            state->type = StateType::Rect;

            sol::optional<std::string> state_id = v["id"];
            sol::optional<int> x = v["x"];
            sol::optional<int> y = v["y"];
            sol::optional<int> w = v["w"];
            sol::optional<int> h = v["h"];

            state->x = x.value_or(0);
            state->y = y.value_or(0);
            state->w = w.value_or(0);
            state->h = h.value_or(0);

            // add state to map
            state_map[state_id.value_or("")] = std::move(state);
          // handle size property
          } else if (property.value() == "size") {
            // create and populate SizeState object
            auto state = std::make_shared<SizeState>();
            state->widget_id = id;
            state->type = StateType::Size;

            sol::optional<std::string> state_id = v["id"];
            sol::optional<int> w = v["w"];
            sol::optional<int> h = v["h"];

            state->w = w.value_or(0);
            state->h = h.value_or(0);

            // add state to map
            state_map[state_id.value_or("")] = std::move(state);
          }
        }
      }
    }

    // add states to global map
    states[id] = state_map;
  }
}

void LuaEngine::RegisterWindow(sol::table args) {
  // create and populate WindowInfo object
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
  info.anchorZone = args.get_or("anchors", WidgetEngine::AnchorZone::None);
  info.layout = args.get_or("layout", WidgetEngine::WindowLayout::None);

  // create and show window
  engine.AddWindow(info);
  engine.ShowWindow(info.name);

  // handle on frame callback
  sol::optional<sol::function> on_frame = args["on_frame"];
  if (on_frame && on_frame.value().valid()) {
    on_frame_callbacks[id] = on_frame.value();
  }

  // handle on signal callback
  sol::optional<sol::function> on_signal = args["on_signal"];
  if (on_signal && on_signal.value().valid()) {
    signal_listeners[id] = on_signal.value();
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

  HandleStates(args);
  SetWidgetMetatable(args, id, "window", "", true);

  // add widget to global registry
  widget_registry[id] = args;
}

void LuaEngine::RegisterButton(const std::string& parent, sol::table args) {
  // get required fields
  std::string id = args.get_or("id", std::string(""));
  std::string text = args.get_or("text", std::string("Button"));

  // get callbacks
  sol::optional<sol::function> on_frame = args["on_frame"];
  sol::optional<sol::function> on_signal = args["on_signal"];
  sol::optional<sol::function> on_click = args["on_click"];
  sol::optional<sol::function> hover_enter = args["hover_enter"];
  sol::optional<sol::function> hover_leave = args["hover_leave"];

  // make sure required fields are set
  sol::optional<std::string> widget_type = args["__widget_type"];
  if (!widget_type) {
    args.raw_set("__widget_type", "window");
  }

  args.raw_set("parent", parent);

  // on click C++ function object
  std::function<void(Qt::MouseButton)> fn_on_click = [this, id, on_click, parent] (Qt::MouseButton btn) {
    if (on_click) {
      sol::function cb = on_click.value();
      if (cb.valid()) {
        auto it = widget_registry.find(id);
        if (it != widget_registry.end()) {
          sol::table self = it->second;
          std::string type = self["__widget_type"];
          SetWidgetMetatable(self, id, type, parent);
          caller_id = id;
          cb(self, static_cast<int>(btn));
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
          sol::table self = it->second;
          SetWidgetMetatable(self, id, self["__widget_type"], parent);
          caller_id = id;
          cb(self);
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
          sol::table self = it->second;
          SetWidgetMetatable(self, id, self["__widget_type"], parent);
          caller_id = id;
          cb(self);
          caller_id = "";
        }
      }
    }
  };

  engine.AddButton(parent, id, text, WidgetEngine::WidgetAlignment::AlignmentNone, fn_on_click, fn_hover_enter, fn_hover_leave);


  // set callbacks if present
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
  SetWidgetMetatable(args, id, "button", parent, true);

  // add widget to global registry
  widget_registry[id] = args;
}

void LuaEngine::RegisterLabel(const std::string& parent, sol::table args) {
  std::string id = args.get_or("id", std::string(""));
  std::string text = args.get_or("text", std::string("Label"));

  sol::optional<sol::function> on_frame = args["on_frame"];
  sol::optional<sol::function> on_signal = args["on_signal"];

  sol::optional<std::string> widget_type = args["__widget_type"];
  if (!widget_type) {
    args.raw_set("__widget_type", "label");
  }

  engine.AddLabel(parent, id, text, WidgetEngine::WidgetAlignment::AlignmentNone);

  widget_registry[id] = args;

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
}

void LuaEngine::CallOnFrameCallbacks() {
  auto it = on_frame_callbacks.begin();
  while (it != on_frame_callbacks.end()) {
    const std::string& window_id = it->first;
    
    auto registry_it = widget_registry.find(window_id);
    if (registry_it == widget_registry.end()) {
      ++it;
      continue;
    }
    
    sol::table self = registry_it->second;

    SetWidgetMetatable(self, self["id"], self["__widget_type"], "");
    
    sol::protected_function_result result = it->second(self);
    if (!result.valid()) {
      sol::error err = result;
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
      std::string id = table["id"];

      if (type == "window") {
        RegisterWindow(table);
        ProcessChildWidgets(table, id);
      } 
    }
  }
}

void LuaEngine::ProcessChildWidgets(sol::table widget, const std::string& parent) {
  for (auto& kv : widget) {
    sol::object key = kv.first;
    sol::object val = kv.second;

    if (val.get_type() == sol::type::table) {
      sol::table t = val.as<sol::table>();
      sol::optional<std::string> type = t["__widget_type"];

      if (type && type.value() == "button") {
        RegisterButton(parent, t);
      }

      if (type && type.value() == "label") {
        RegisterLabel(parent, t);
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

  lua["easing"] = lua.create_table_with(
    "in_quad", QEasingCurve::InQuad,
    "out_quad", QEasingCurve::OutQuad,
    "in_out_quad", QEasingCurve::InOutQuad,
    "in_cubic", QEasingCurve::InCubic,
    "out_cubic", QEasingCurve::OutCubic,
    "in_out_cubic", QEasingCurve::InOutCubic
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

  lua.set_function("animate_once", [this](sol::table args) {
    if (caller_id == "") {
      std::cerr << "animate_once must be called from a widget\n";
      return;
    }

    sol::optional<std::string> start_state = args["start_state"];
    sol::optional<std::string> end_state = args["end_state"];

    auto widget_states = states.find(caller_id);
    if (widget_states == states.end()) {
      std::cerr << "no states in widget\n";
      return;
    }

    auto start_it = widget_states->second.find(start_state.value());
    auto end_it = widget_states->second.find(end_state.value());

    if (start_it != widget_states->second.end() && end_it != widget_states->second.end()) {
      if (start_it->second->type == StateType::Rect) {
        auto start_rect = std::static_pointer_cast<RectState>(start_it->second);
        auto end_rect = std::static_pointer_cast<RectState>(end_it->second);

        QRect start = {
          start_rect->x,
          start_rect->y,
          start_rect->w,
          start_rect->h
        };

        QRect end = {
          end_rect->x,
          end_rect->y,
          end_rect->w,
          end_rect->h
        };

        sol::optional<std::string> parent = widget_registry[caller_id]["parent"];
        sol::optional<WidgetEngine::AnimCurve> curve = args["curve"];
        sol::optional<int> duration = args["duration"];

        engine.AnimateWidgetGeometryOnce(parent.value_or(""), caller_id, start, end, duration.value_or(1000), curve.value_or(WidgetEngine::AnimCurve::InOutQuad));
      } else if (start_it->second->type == StateType::Size) {
        auto start_size = std::static_pointer_cast<SizeState>(start_it->second);
        auto end_size = std::static_pointer_cast<SizeState>(end_it->second);

        QSize start = {
          start_size->w,
          start_size->h
        };

        QSize end = {
          end_size->w,
          end_size->h
        };

        sol::optional<std::string> parent = widget_registry[caller_id]["parent"];
        sol::optional<WidgetEngine::AnimCurve> curve = args["curve"];
        sol::optional<int> duration = args["duration"];

        engine.AnimateWidgetMaxSizeOnce(parent.value_or(""), caller_id, start, end, duration.value_or(1000), curve.value_or(WidgetEngine::AnimCurve::InOutQuad));
      }
    }
  });

  lua.set_function("emit", [this](const std::string& signal) {
    if (caller_id == "") {
      std::cerr << "emit must be called from a widget\n";
      return;
    }

    sol::table widget = widget_registry.find(caller_id)->second;
    std::string emitter_type = widget["__widget_type"];

    userdata data = { widget["id"], emitter_type };
    lua_signals[signal] = data;
    for (const auto& kv : signal_listeners) {
      std::string id = kv.first;

      sol::optional<std::string> parent = widget_registry[id]["parent"];
      std::string parent_str = parent.value_or("");

      std::string listener_type = widget_registry[id]["__widget_type"];
      sol::table self = widget_registry[id];

      SetWidgetMetatable(self, id, listener_type, parent_str);
      kv.second(self, signal);
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

    function Button (args)
      args = args or {}
      args.__widget_type = "button"
      args.id = args.id or ("button_" .. tostring(#__widget_list + 1))
      table.insert(__widget_list, args)

      return args
    end

    function Label (args)
      args = args or {}
      args.__widget_type = "label"
      args.id = args.id or ("label_" .. tostring(#__widget_list + 1))
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
