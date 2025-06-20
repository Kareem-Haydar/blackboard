#include <LuaEngine/LuaEngine.h>

namespace LuaEngine {
  void Engine::RegisterBindings() {
    sol::object debug = sol::make_object(lua, BL_DEBUG);
    lua["__debug"] = debug;

    sol::object version = sol::make_object(lua, BL_VERSION);
    lua["__version"] = version;

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
      if (BL_DEBUG) {
        std::cout << "processing " << widget_registry.size() << " widgets\n";
      }

      ProcessTopLevelWidgets();
    });

    lua.set_function("animate_once", [this](sol::table args) {
      if (caller_id == "") {
        std::cerr << "animate_once must be called from a widget\n";
        return;
      }

      sol::optional<std::string> parent = widget_registry[caller_id]["parent"];

      sol::optional<std::string> start_state = args["start_state"];
      sol::optional<std::string> end_state = args["end_state"];

      if (BL_DEBUG) {
        std::cout << "animate_once called from " << caller_id << "\n";
        std::cout << "\t start state: " << start_state.value() << "\n";
        std::cout << "\t end state: " << end_state.value() << "\n";
        std::cout << "\n";
      }

      if (!start_state) {
        std::cerr << "animate_once must have a start state\n";
        return;
      }

      if (!end_state) {
        std::cerr << "animate_once must have an end state\n";
        return;
      }

      auto widget_states = states.find(caller_id);
      if (widget_states == states.end()) {
        std::cerr << "no states in widget\n";
        return;
      }

      auto start_state_it = widget_states->second.find(start_state.value());
      auto end_state_it = widget_states->second.find(end_state.value());

      if (start_state.value() != "current_state" && start_state_it == widget_states->second.end()) {
        std::cerr << "start state not found\n";
        return;
      }

      if (end_state_it == widget_states->second.end()) {
        std::cerr << "end state not found\n";
        return;
      }

      if (end_state_it->second->type == StateType::Size) {
        auto end_state = std::static_pointer_cast<SizeState>(end_state_it->second);

        QSize end = {
          end_state->w,
          end_state->h
        };

        QSize start;

        if (start_state.value() == "current_state") {
          auto current_size = engine.GetWidgetSize(parent.value(), caller_id);

          start = {
            current_size[0],
            current_size[1]
          };
        } else {
          auto start_state = std::static_pointer_cast<SizeState>(start_state_it->second);

          start = {
            start_state->w,
            start_state->h
          };
        }

        if (start.width() <= 0 || start.height() <= 0) {
          std::cerr << "invalid start size\n";
          return;
        }

        if (end.width() <= 0 || end.height() <= 0) {
          std::cerr << "invalid end size\n";
          return;
        }

        sol::optional<int> duration = args["duration"];
        sol::optional<WidgetEngine::AnimCurve> curve = args["curve"];

        engine.AnimateWidgetMaxSizeOnce(parent.value(), caller_id, start, end, duration.value_or(1000), curve.value_or(WidgetEngine::AnimCurve::OutQuad));
      }
    });


    lua.set_function("emit", [this](const std::string& signal) {
      if (caller_id == "") {
        std::cerr << "emit must be called from a widget\n";
        return;
      }

      if (BL_DEBUG) {
        std::cout << "emitting signal " << signal << " from " << caller_id << std::endl;
        std::cout << "\n";
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

        if (BL_DEBUG) {
          std::cout << "calling on_signal from " << id << "\n";
        }

        SetWidgetMetatable(self, id, listener_type, parent_str);
        kv.second(self, signal);
      }

      if (BL_DEBUG) {
        std::cout << "\n";
      }
    });
  }
}
