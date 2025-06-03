#include <sol/sol.hpp>
#include <headers/WidgetEngine.h>

class LuaEngine {
  private:
    WidgetEngine engine;
    sol::state lua;
   void RegisterBindings();

  public:
    LuaEngine(int argc, char** argv);
    ~LuaEngine() = default;

    void RunScript(const std::string& scriptPath);
};
