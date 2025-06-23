#include <LuaEngine/LuaEngine.h>
#include <memory>

namespace LuaEngine {
  void Engine::RegisterAnimation(const std::string& parent, const std::string& widget_id, sol::table args) {
    sol::optional<std::string> id = args["id"];
    sol::optional<std::string> start = args["start_state"];
    sol::optional<std::string> end = args["end_state"];
    sol::optional<int> duration = args["duration"];
    sol::optional<bool> interruptible = args["interruptible"];
    sol::optional<sol::table> when = args["when"];
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

      if (when) {
        RegisterAnimHook(widget_id, id.value(), interruptible.value_or(false), when.value());
      }

      engine.AddAnimation(parent, widget_id, id.value(), "maximumSize", duration.value(), curve.value());
      std::cout << "\n";
    }
  }

  void Engine::RegisterAnimHook(const std::string& widget_id, const std::string& anim_id, bool interruptible, sol::table hook) {
    sol::optional<std::string> widget = hook["widget"];
    sol::optional<std::string> event = hook["event"];
    sol::optional<std::string> fn_name = hook["fn_name"];
    sol::optional<std::string> direction = hook["direction"];

    if (!widget || !event || !fn_name || !direction) {
      std::cerr << "invalid animation hook\n";
      return;
    }

    hook["__anim_id"] = anim_id;
    hook["__widget_id"] = widget_id;
    hook["__interruptible"] = interruptible;

    auto it = widget_registry.find(widget_id);
    if (it == widget_registry.end()) {
      std::cerr << "widget not found\n";
      return;
    }

    sol::table tbl = it->second;
    hook["__widget_parent"] = tbl["parent"];

    if (BL_DEBUG) {
      std::cout << "registering hook for animation " << anim_id << "\n";
      std::cout << "\t widget: " << widget.value() << "\n";
      std::cout << "\t event: " << event.value() << "\n";
      std::cout << "\t fn_name: " << fn_name.value() << "\n";
      std::cout << "\n";
    }

    auto widget_handle = widget_registry.find(widget.value());
    if (widget_handle == widget_registry.end()) {
      std::cerr << "invalid widget\n";
      return;
    }

    if (event && event.value() == "pre") {
      animation_pre_hooks[anim_id] = hook;
    } else if (event && event.value() == "post") {
      animation_post_hooks[anim_id] = hook;
    } else {
      std::cerr << "invalid event\n";
      return;
    }
  }

  void Engine::CallAnimPreHooks(const std::string& fn_name) {
    for (const auto& kv : animation_pre_hooks) {
      sol::table hook = kv.second;
      sol::optional<std::string> widget = hook["widget"];
      sol::optional<std::string> event = hook["event"];
      sol::optional<std::string> hook_fn_name = hook["fn_name"];
      sol::optional<std::string> direction = hook["direction"];
      sol::optional<std::string> anim_id = hook["__anim_id"];
      sol::optional<std::string> widget_id = hook["__widget_id"];
      sol::optional<std::string> widget_parent = hook["__widget_parent"];
      sol::optional<bool> interruptible = hook["__interruptible"];

      if (BL_DEBUG) {
        std::cout << "calling pre hook for animation " << anim_id.value() << "\n";
        std::cout << "\t widget: " << widget.value() << "\n";
        std::cout << "\t event: " << event.value() << "\n";
        std::cout << "\t fn_name: " << hook_fn_name.value() << "\n";
        std::cout << "\t direction: " << direction.value() << "\n";
        std::cout << "\t widget_id: " << widget_id.value() << "\n";
        std::cout << "\n";
      }

      auto anim = widget_animations.find(widget_id.value());
      if (anim == widget_animations.end()) {
        std::cerr << "animation not found, skipping\n";
        continue;
      }

      if (direction.value() == "forward") {
        engine.SetAnimDirection(anim_id.value(), true);
      } else if (direction.value() == "backward") {
        engine.SetAnimDirection(anim_id.value(), false);
      } else {
        std::cerr << "invalid direction, skipping\n";
        continue;
      }

      if (widget_id == caller_id && hook_fn_name == fn_name) {
        auto anims = widget_animations.find(widget_id.value());
        if (anims == widget_animations.end()) {
          std::cerr << "animation not found, skipping\n";
          continue;
        }

        auto anim = anims->second[anim_id.value()];
        
        if (anim->property == "size") {
          auto animation = std::static_pointer_cast<SizeAnimation>(anim);

          QSize start_size;
          QSize end_size;

          if (animation->start == "current_state") {
            auto current_size = engine.GetWidgetSize(widget_parent.value(), widget_id.value());
            start_size = {
              current_size[0],
              current_size[1]
            };
          } else {
            start_size = animation->start_size;
          }

          engine.SetAnimStartValue(anim_id.value(), start_size);

          end_size = animation->end_size;
          engine.SetAnimEndValue(anim_id.value(), end_size);

          if (!engine.AnimationPlaying(anim_id.value()) || interruptible) {
            if (engine.AnimationPlaying(anim_id.value()) && interruptible) {
              engine.StopAnimation(anim_id.value());
            } 

            if (BL_DEBUG) {
              std::cout << "starting animation " << anim_id.value() << "\n";
            }

            engine.StartAnimation(anim_id.value());
          }
        }
      }
    }
  }

  void Engine::CallAnimPostHooks(const std::string& fn_name) {
    for (const auto& kv : animation_post_hooks) {
      sol::table hook = kv.second;
      sol::optional<std::string> widget = hook["widget"];
      sol::optional<std::string> event = hook["event"];
      sol::optional<std::string> hook_fn_name = hook["fn_name"];
      sol::optional<std::string> direction = hook["direction"];
      sol::optional<std::string> anim_id = hook["__anim_id"];
      sol::optional<std::string> widget_id = hook["__widget_id"];
      sol::optional<std::string> widget_parent = hook["__widget_parent"];
      sol::optional<bool> interruptible = hook["__interruptible"];

      auto anim = widget_animations.find(anim_id.value());
      if (anim == widget_animations.end()) {
        std::cerr << "animation not found, skipping\n";
        continue;
      }

      if (direction.value() == "forward") {
        engine.SetAnimDirection(anim_id.value(), true);
      } else if (direction.value() == "backward") {
        engine.SetAnimDirection(anim_id.value(), false);
      } else {
        std::cerr << "invalid direction, skipping\n";
        continue;
      }

      if (widget_id == caller_id && hook_fn_name == fn_name) {
        auto anims = widget_animations.find(widget_id.value());
        if (anims == widget_animations.end()) {
          std::cerr << "animation not found, skipping\n";
          continue;
        }
        
        for (const auto& anim : anims->second) {
          if (anim.second->property == "size") {
            auto animation = std::static_pointer_cast<SizeAnimation>(anim.second);

            QSize start_size;
            QSize end_size;

            if (animation->start == "current_state") {
              auto current_size = engine.GetWidgetSize(widget_parent.value(), widget_id.value());
              start_size = {
                current_size[0],
                current_size[1]
              };
            } else {
              start_size = animation->start_size;
            }

            engine.SetAnimStartValue(anim.first, start_size);

            end_size = animation->end_size;
            engine.SetAnimEndValue(anim.first, end_size);

            if (!engine.AnimationPlaying(anim.first) || interruptible) {
              if (engine.AnimationPlaying(anim.first)) {
                engine.StopAnimation(anim.first);
              }

              engine.StartAnimation(anim.first);
            }
          }
        }
      }
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

