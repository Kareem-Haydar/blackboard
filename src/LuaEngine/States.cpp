#include <LuaEngine/LuaEngine.h>

namespace LuaEngine {
  void Engine::HandleStates(sol::table args, const std::string& parent) {
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
              if (BL_DEBUG) {
                std::cout << "adding stylesheet animation state for " << id << " with parent " << parent << "\n";
                std::cout << "\n";
              }

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
              if (BL_DEBUG) {
                std::cout << "adding geometry animation state for " << id << " with parent " << parent << "\n";
                std::cout << "\n";
              }

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
              if (BL_DEBUG) {
                std::cout << "adding size animation state for " << id << " with parent " << parent << "\n";
                std::cout << "\n";
              }

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
}
