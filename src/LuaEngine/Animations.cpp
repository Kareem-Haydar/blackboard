#include <LuaEngine/LuaEngine.h>
#include <memory>

namespace LuaEngine {
  void Engine::RegisterAnimation(const std::string& parent, const std::string& widget_id, sol::table args) {
    sol::optional<std::string> id = args["id"];
    sol::optional<std::string> start = args["start_state"];
    sol::optional<std::string> end = args["end_state"];
    sol::optional<int> duration = args["duration"];
    sol::optional<bool> interruptible = args["interruptible"];
    sol::optional<sol::function> when = args["when"];
    sol::optional<WidgetEngine::AnimCurve> curve = args["curve"];

    if (!id || !start || !end || !when || !curve || !duration) {
      std::cerr << "invalid arguments\n";
      return;
    }

    if (BL_DEBUG) {
      std::cout << "PropertyAnimation called from " << widget_id << "\n";
      std::cout << "\t start state: " << start.value() << "\n";
      std::cout << "\t end state: " << end.value() << "\n";
    }


    auto widget_states = states.find(widget_id);
    if (widget_states == states.end()) {
      std::cerr << "no states in widget\n";
      return;
    }

    auto start_state_it = widget_states->second.find(start.value());
    auto end_state_it = widget_states->second.find(end.value());

    if (start != "current_state" && start_state_it == widget_states->second.end()) {
      std::cerr << "start state not found\n";
      return;
    }

    if (end_state_it == widget_states->second.end()) {
      std::cerr << "end state not found\n";
      return;
    }

    if (end == "current_state") {
      std::cerr << "end state cannot be current_state\n";
      return;
    }

    if (end_state_it->second->type == StateType::Size) {
      if (BL_DEBUG) {
        std::cout << "\tsize animation\n";
      }

      QSize start_size;
      QSize end_size;

      auto end_state = std::static_pointer_cast<SizeState>(end_state_it->second);

      if (!end_state) {
        std::cerr << "end state is not a size state\n";
        return;
      }

      if (start == "current_state") {
        auto current_size = engine.GetWidgetSize(parent, widget_id);
        start_size = {
          current_size[0],
          current_size[1]
        };
      } else {
        auto start_state = std::static_pointer_cast<SizeState>(start_state_it->second);
        start_size = {
          start_state->w,
          start_state->h
        };
      }

      end_size = {
        end_state->w,
        end_state->h
      };

      std::shared_ptr<SizeAnimation> animation = std::make_shared<SizeAnimation>();
      animation->widget_id = widget_id;
      animation->start = start.value();
      animation->end = end.value();
      animation->start_size = start_size;
      animation->end_size = end_size;
      animation->interruptible = interruptible.value_or(false);
      animation->when = when.value();
      animation->property = "size";

      auto widget_animations_map = widget_animations.find(widget_id);
      if (widget_animations_map == widget_animations.end()) {
        if (BL_DEBUG) {
          std::cout << "\tcreating new animation map for widget " << widget_id << "\n";
        }
        std::unordered_map<std::string, std::shared_ptr<PropertyAnimation>> animations;
        animations[id.value()] = std::move(animation);
        widget_animations[widget_id] = std::move(animations);
      } else {
        if (BL_DEBUG) {
          std::cout << "\tadding animation to existing animation map for widget " << widget_id << "\n";
        }
        widget_animations_map->second[id.value()] = std::move(animation);
      }

      engine.AddAnimation(parent, widget_id, id.value(), "maximumSize", duration.value(), curve.value());
      std::cout << "\n";
    }
  }

  void Engine::HandleAnimations() {
    if (BL_DEBUG) {
      std::cout << "handling animations\n";
    }

    // iterate through each widget
    auto top_level_animations_it = widget_animations.begin();
    while (top_level_animations_it != widget_animations.end()) {
      // iterate through each animation
      auto widget_animations_it = top_level_animations_it->second.begin();
      std::string parent = widget_registry[top_level_animations_it->first]["parent"];
      std::string widget_id = top_level_animations_it->first;

      if (BL_DEBUG) {
        std::cout << "handling animations for widget " << widget_id << "\n";
      }

      while (widget_animations_it != top_level_animations_it->second.end()) {
        // handle size animations
        if (widget_animations_it->second->property == "size") {
          auto anim = std::static_pointer_cast<SizeAnimation>(widget_animations_it->second);
          sol::object when = anim->when();

          if (BL_DEBUG) {
            std::cout << "\tfound size animtion " << widget_animations_it->first << " for widget " << widget_animations_it->second->widget_id << "\n";
          }

          if (when.is<int>() && when.as<int>() == 1) {
            QSize start_size;

            // handle current state
            if (widget_animations_it->second->start == "current_state") {
              auto current_size = engine.GetWidgetSize(parent, widget_animations_it->second->widget_id);
              start_size = {
                current_size[0],
                current_size[1]
              };
            } else {
              start_size = anim->start_size;
            }

            engine.SetAnimStartValue(widget_animations_it->first, start_size);
            engine.SetAnimEndValue(widget_animations_it->first, anim->end_size);

            engine.SetAnimDirection(widget_animations_it->first, true);

            if (anim->interruptible || !engine.AnimationPlaying(widget_animations_it->first)) {
              if (BL_DEBUG) {
                std::cout << "playing forward animation " << widget_animations_it->first << " for widget " << widget_animations_it->second->widget_id << "\n";
              }
              engine.StartAnimation(widget_animations_it->first);
            } else if (anim->interruptible && engine.AnimationPlaying(widget_animations_it->first)) {
              if (BL_DEBUG) {
                std::cout << "interrupting current animtion and playing forward animation " << widget_animations_it->first << " for widget " << widget_animations_it->second->widget_id << "\n";
              }
              engine.StopAnimation(widget_animations_it->first);
              engine.StartAnimation(widget_animations_it->first);
            }
          } else if (when.is<int>() && when.as<int>() == -1) {
            auto anim = std::static_pointer_cast<SizeAnimation>(widget_animations_it->second);
            sol::object when = anim->when();

            if (when.is<int>() && when.as<int>() == 1) {
              QSize start_size;

              // handle current state
              if (widget_animations_it->second->start == "current_state") {
                auto current_size = engine.GetWidgetSize(parent, widget_animations_it->second->widget_id);
                start_size = {
                  current_size[0],
                  current_size[1]
                };
              } else {
                start_size = anim->start_size;
              }

              if (BL_DEBUG) {
                std::cout << "playing backward animation " << widget_animations_it->first << " for widget " << widget_animations_it->second->widget_id << "\n";
              }

              engine.SetAnimStartValue(widget_animations_it->first, start_size);
              engine.SetAnimEndValue(widget_animations_it->first, anim->end_size);

              engine.SetAnimDirection(widget_animations_it->first, false);

              if (anim->interruptible || !engine.AnimationPlaying(widget_animations_it->first)) {
                engine.StartAnimation(widget_animations_it->first);
              } else if (anim->interruptible && engine.AnimationPlaying(widget_animations_it->first)) {
                engine.StopAnimation(widget_animations_it->first);
                engine.StartAnimation(widget_animations_it->first);
              }
            }
          }
        }
        ++widget_animations_it;
      }
      ++top_level_animations_it;
    }
  }

  void Engine::ProcessAnimations(sol::table widget, const std::string& parent) {
    sol::optional<std::string> id = widget["id"];
    if (!id) {
      if (BL_DEBUG) {
        std::cout << "tried to register animation on an invalid widget\n";
      }
      return;
    }

    for (auto kv : widget) {
      sol::object key = kv.first;
      sol::object val = kv.second;

      if (val.is<sol::table>()) {
        sol::table v = val.as<sol::table>();
        sol::optional<std::string> type = v["__widget_type"];

        if (type && type.value() == "property_animation") {
          if (BL_DEBUG) {
            std::cout << "rigistering property animation for " << id.value() << "\n";
          }
          RegisterAnimation(parent, id.value(), v);
          if (BL_DEBUG) {
            std::cout << "registered property animation for " << id.value() << "\n";
            std::cout << "\n";
          }
        }
      }
    }
  }
};
