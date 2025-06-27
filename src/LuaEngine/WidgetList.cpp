#include <LuaEngine/LuaEngine.h>

namespace LuaEngine {
  void Engine::RegisterWidgetList(const std::string& parent, sol::table args) {
    sol::optional<std::string> id = args["id"];
    sol::optional<sol::table> model = args["model"];
    sol::optional<sol::function> widget_template = args["template"];

    if (!id || !model || !widget_template) {
      std::cerr << "invalid arguments\n";
      return;
    }

    for (const auto& kv : model.value()) {
      sol::table tbl = kv.second;

      sol::table res = widget_template.value().call(tbl);
      if (!res.valid()) {
        std::cerr << "invalid template result\n";
        return;
      }

      sol::optional<std::string> type = res["__widget_type"];

      if (!type) {
        std::cout << "invalid widget template\n";
      }

      bool active = tbl["active"];
      if (!active)
        continue;

      std::string id = tbl["id"];

      if (type == "button") {
        auto it = widget_registry.find(id);
        if (it == widget_registry.end()) {
          RegisterButton(parent, res, false);
        }
      }
    }
  }
}
