#pragma once

#include <sol/sol.hpp>
#include <sol/forward.hpp>

#include <BlackboardConfig.h>
#include <WidgetEngine/WidgetEngine.h>
#include <LuaEngine/Structs.h>

#include <filesystem>
#include <queue>

#pragma once

namespace LuaEngine {
  class Engine {
    private:
      WidgetEngine::Engine engine;
      sol::state lua;

      std::unordered_map<std::string, sol::table> widget_registry;
      sol::table lua_registry;

      std::unordered_map<std::string, sol::function> signal_listeners;
      std::unordered_map<std::string, LuaEngine::userdata> lua_signals;
      std::string caller_id = "";

      std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<State>>> states;

      std::unordered_map<std::string, sol::function> on_frame_callbacks;

      std::unordered_map<std::string, std::vector<sol::table>> widget_pre_hooks;
      std::unordered_map<std::string, std::vector<sol::table>> widget_post_hooks;
      std::unordered_map<std::string, sol::table> animation_pre_hooks;

      std::unordered_map<std::string, std::vector<sol::table>> global_pre_hooks;
      std::unordered_map<std::string, std::vector<sol::table>> global_post_hooks;
      std::unordered_map<std::string, sol::table> animation_post_hooks;

      std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<PropertyAnimation>>> widget_animations;

      std::unordered_map<std::string, Widget> widgets;
      std::queue<std::pair<std::string, std::string>> signal_queue;

      QTimer* frameTimer;

      void RegisterHook(const std::string& widget_id, sol::table hook);
      void ProcessHooks();

      void CallGlobalPreHooks(const std::string& fn_name);
      void CallWidgetPreHooks(const std::string& fn_name);

      void CallGlobalPostHooks(const std::string& fn_name);
      void CallWidgetPostHooks(const std::string& fn_name);

      void RegisterBindings();
      void CallOnFrameCallbacks();

      void SetWidgetMetatable(sol::table widget, const std::string& id, const std::string& type, const std::string& parent, bool overwrite = false);

      void ProcessTopLevelWidgets();
      void ProcessChildWidgets(sol::table widget, const std::string& parent);
      void ProcessLayoutWidgets(sol::table layout, const std::string& parent);

      void HandleStates(sol::table args, const std::string& parent = "");
      void ProcessAnimations(sol::table widget, const std::string& parent);

      void RegisterHBoxLayout(const std::string& parent, sol::table args);
      void RegisterVBoxLayout(const std::string& parent, sol::table args);

      void RegisterWindow(sol::table args);
      void RegisterLabel(const std::string& parent, sol::table args, bool layout = false);
      void RegisterButton(const std::string& parent, sol::table args, bool layout = false);
      void RegisterLineEdit(const std::string& parent, sol::table args);

      void RegisterWidgetList(const std::string& parent, sol::table args);

      void RegisterAnimation(const std::string& parent, const std::string& widget_id, sol::table args);
      void RegisterAnimHook(const std::string& widget_id, const std::string& anim_id, bool interruptible, sol::table hook);
      void CallAnimPreHooks(const std::string& fn_name);
      void CallAnimPostHooks(const std::string& fn_name);

      void EmitSignal(const std::string& signal);
      void ProcessSignals();

    public:
      Engine(int argc, char** argv);
      ~Engine();

      void RunFile(const std::string& file);
      void RunAllFiles();

      int Exec();
  };
}
