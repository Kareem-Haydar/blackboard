#include <LuaEngine/LuaEngine.h>

namespace LuaEngine {
  void Engine::SetWidgetMetatable(sol::table widget, const std::string& id, const std::string& type, const std::string& parent, bool overwrite) {
    // check if the metatable exists already
    sol::optional<sol::table> mt_exists = widget[sol::metatable_key];
    if (!mt_exists || overwrite) {
      if (BL_DEBUG) {
        std::cout << "setting metatable for " << id << "\n";
        std::cout << "\n";
      }

      sol::table mt = lua.create_table();
      
      // set newindex method
      mt.set_function("__newindex", [this, id, type, parent] (sol::table table, sol::object key, sol::object val) {
        // make sure key is a string
        if (key.is<std::string>()) {
          std::string k = key.as<std::string>();

          // handle string values
          if (val.is<std::string>()) {
            std::string v = val.as<std::string>();

            // make sure widget is valid
            if (type != "window" && parent == "") {
              std::cerr << "widgets other than windows must have a parent\n";
              return true;
            }

            // handle stylesheet changes
            if (k == "stylesheet") {
              if (type == "window") {
                if (BL_DEBUG) {
                  std::cout << "setting window stylesheet for " << id << "\n";
                  std::cout << "\n";
                }

                engine.SetWindowStyleSheet(id, v);
              } else {
                if (BL_DEBUG) {
                  std::cout << "setting widget stylesheet for " << id << " with parent " << parent << "\n";
                  std::cout << "\n";
                }

                engine.SetWidgetStyleSheet(parent, id, v);
              }
            } 
          // handle integer values
          } else if (val.is<int>()) {
            int v = val.as<int>();

            // makme sure widget is valid
            if (type != "window" && parent == "") {
              std::cerr << "widgets other than windows must have a parent\n";
              return true;
            }

            // handle geometry changes
            if (k == "width") {
              if (type == "window") {
                if (BL_DEBUG) {
                  std::cout << "setting window width for " << id << "\n";
                  std::cout << "\n";
                }

                std::array<int, 2> size = engine.GetWindowSize(id);
                engine.ResizeWindow(id, v, size[1]);
              } else {
                if (BL_DEBUG) {
                  std::cout << "setting widget width for " << id << " with parent " << parent << "\n";
                  std::cout << "\n";
                }

                std::array<int, 2> size = engine.GetWidgetSize(parent, id);
                engine.ResizeWidget(parent, id, v, size[1]);
              }
            } else if (k == "height") {
              if (type == "window") {
                if (BL_DEBUG) {
                  std::cout << "setting window height for " << id << "\n";
                  std::cout << "\n";
                }

                std::array<int, 2> size = engine.GetWindowSize(id);
                engine.ResizeWindow(id, size[0], v);
              } else {
                if (BL_DEBUG) {
                  std::cout << "setting widget height for " << id << " with parent " << parent << "\n";
                  std::cout << "\n";
                }

                std::array<int, 2> size = engine.GetWidgetSize(parent, id);
                engine.ResizeWidget(parent, id, size[0], v);
              }
            }
          }
        }

        return true;
      });

      // set metatable if it doesn't exist
      widget[sol::metatable_key] = mt;
    }
  }
}
