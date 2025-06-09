#include <sol/sol.hpp>
#include <filesystem>
#include <headers/WidgetEngine.h>

class LuaEngine {
  private:
    WidgetEngine engine;
    sol::state lua;
    std::vector<sol::function> on_frame_callbacks;
    QTimer* frameTimer;

    void RegisterBindings();
    void CallOnFrameCallbacks();

    void ProcessChildWidget(sol::table widget, const std::string& window);

    void RegisterWindow(const WidgetEngine::WindowInfo& info, sol::function on_frame);
    void RegisterButton(const std::string& window, const std::string& name, const std::string& text, sol::function on_click);

  public:
    LuaEngine(int argc, char** argv);
    ~LuaEngine();

    int Exec();
};
