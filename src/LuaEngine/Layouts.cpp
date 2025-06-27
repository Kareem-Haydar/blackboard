#include <LuaEngine/LuaEngine.h>

namespace LuaEngine {
  void Engine::RegisterVBoxLayout(const std::string& parent, sol::table args) {
    sol::optional<std::string> id = args["id"];
    if (!id) {
      std::cerr << "invalid arguments\n";
      return;
    }

    if (BL_DEBUG) {
      std::cout << "registering vertical layout " << id.value() << " with parent " << parent << "\n";
      std::cout << "\n";
    }

    engine.AddVBoxLayout(parent, id.value());
  }

  void Engine::RegisterHBoxLayout(const std::string& parent, sol::table args) {
    sol::optional<std::string> id = args["id"];

    if (!id) {
      std::cerr << "invalid arguments\n";
      return;
    }

    engine.AddHBoxLayout(parent, id.value());
  }
}
