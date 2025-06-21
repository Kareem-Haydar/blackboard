#pragma once

#include <sol/sol.hpp>
#include <sol/forward.hpp>

#include <BlackboardConfig.h>
#include <WidgetEngine/WidgetEngine.h>
#include <LuaEngine/Structs.h>

#include <filesystem>

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

      std::unordered_map<std::string, sol::function> widget_pre_hooks;
      std::unordered_map<std::string, sol::function> widget_post_hooks;

      std::unordered_map<std::string, sol::table> global_pre_hooks;
      std::unordered_map<std::string, sol::table> global_post_hooks;

      std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<PropertyAnimation>>> widget_animations;

      std::unordered_map<std::string, Widget> widgets;

      QTimer* frameTimer;

      void RegisterHooks(const std::string& widget_id, sol::table hooks);

      void CallGlobalPreHooks(const std::string& fn_name);
      void CallWidgetPreHooks(const std::string& fn_name);

      void CallGlobalPostHooks(const std::string& fn_name);
      void CallWidgetPostHooks(const std::string& fn_name);

      void RegisterBindings();
      void CallOnFrameCallbacks();

      void SetWidgetMetatable(sol::table widget, const std::string& id, const std::string& type, const std::string& parent, bool overwrite = false);

      void ProcessTopLevelWidgets();
      void ProcessChildWidgets(sol::table widget, const std::string& parent);

      void ProcessAnimations(sol::table widget, const std::string& parent);

      void HandleStates(sol::table args, const std::string& parent = "");

      void RegisterWindow(sol::table args);
      void RegisterLabel(const std::string& parent, sol::table args);
      void RegisterButton(const std::string& parent, sol::table args);
      void RegisterLineEdit(const std::string& parent, sol::table args);

      void RegisterAnimation(const std::string& parent, const std::string& widget_id, sol::table args);

      void HandleAnimations();

      void EmitSignal(const std::string& signal);

    public:
      Engine(int argc, char** argv);
      ~Engine();

      void RunFile(const std::string& file);
      void RunAllFiles();

      int Exec();
  };
}
