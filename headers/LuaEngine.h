#include <sol/sol.hpp>
#include <headers/WidgetEngine.h>

class LuaEngine {
  private:
    WidgetEngine engine;
    sol::state lua;

    void RegisterBindings();
    void ShowWindow(const std::string& name);
    void HideWindow(const std::string& name);

  public:
    LuaEngine(int argc, char** argv);
    ~LuaEngine() = default;

    void RunScript(const std::string& scriptPath);
};
