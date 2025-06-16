#include <sol/sol.hpp>
#include <sol/forward.hpp>
#include <filesystem>
#include <headers/WidgetEngine.h>

class LuaEngine {
  private:
    // userdata for signals
    struct userdata {
      std::string id;
      std::string type;
    };

    // animation state enums
    enum StateType {
      Stylesheet = 0,
      Rect,
      Size,
    };

    struct State {
      StateType type;
      std::string widget_id;
    };

    struct SizeState : public State {
      int w;
      int h;
    };

    struct RectState : public State {
      int x;
      int y;
      int w;
      int h;
    };

    struct StylesheetState : public State {
      std::string stylesheet;
    };

    WidgetEngine engine;
    sol::state lua;

    std::unordered_map<std::string, sol::table> widget_registry;
    sol::table lua_registry;

    std::unordered_map<std::string, sol::function> signal_listeners;
    std::unordered_map<std::string, LuaEngine::userdata> lua_signals;
    std::string caller_id = "";

    std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<State>>> states;

    std::unordered_map<std::string, sol::function> on_frame_callbacks;

    QTimer* frameTimer;

    void RegisterBindings();
    void CallOnFrameCallbacks();

    void SetWidgetMetatable(sol::table widget, const std::string& id, const std::string& type, const std::string& parent, bool overwrite = false);

    void ProcessTopLevelWidgets();
    void ProcessChildWidgets(sol::table widget, const std::string& parent);

    void HandleStates(sol::table args, const std::string& parent = "");

    void RegisterWindow(sol::table args);
    void RegisterLabel(const std::string& parent, sol::table args);
    void RegisterButton(const std::string& parent, sol::table args);

  public:
    LuaEngine(int argc, char** argv);
    ~LuaEngine();

    int Exec();
};
