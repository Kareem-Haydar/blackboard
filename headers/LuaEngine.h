#include <sol/sol.hpp>
#include <filesystem>
#include <headers/WidgetEngine.h>

class LuaEngine {
  private:
    WidgetEngine engine;
    sol::state lua;
    std::unordered_map<std::string, sol::table> widget_registry;
    sol::table lua_registry;
    std::unordered_map<std::string, sol::function> on_frame_callbacks;
    QTimer* frameTimer;

    void RegisterBindings();
    void CallOnFrameCallbacks();

    void ProcessTopLevelWidgets();

    void ProcessChildWidget(sol::table widget, const std::string& window);

    void RegisterWindow(sol::table args);
    void RegisterButton(const std::string& window, const std::string& name, const std::string& text, sol::function on_frame, sol::function on_click, sol::function hover_enter, sol::function hover_leave);

  public:
    LuaEngine(int argc, char** argv);
    ~LuaEngine();

    int Exec();
};
