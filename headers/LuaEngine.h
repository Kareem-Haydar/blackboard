#include <sol/sol.hpp>
#include <sol/forward.hpp>
#include <filesystem>
#include <headers/WidgetEngine.h>

class LuaEngine {
  private:
    struct userdata {
      std::string id;
      std::string type;
    };

    std::string current_emitter_id = "";

    WidgetEngine engine;
    sol::state lua;
    std::unordered_map<std::string, sol::table> widget_registry;
    sol::table lua_registry;

    std::unordered_map<std::string, sol::function> signal_listeners;
    std::unordered_map<std::string, LuaEngine::userdata> lua_signals;
    std::unordered_map<std::string, sol::function> on_frame_callbacks;
    QTimer* frameTimer;

    void RegisterBindings();
    void CallOnFrameCallbacks();

    void SetWidgetMetatable(sol::table widget, const std::string& id, const std::string& type, const std::string& parent);

    void ProcessTopLevelWidgets();
    void ProcessChildWidgets(sol::table widget, const std::string& parent);

    void RegisterWindow(sol::table args);
    void RegisterButton(const std::string& parent, sol::table args);

  public:
    LuaEngine(int argc, char** argv);
    ~LuaEngine();

    int Exec();
};
