#include <LuaEngine/LuaEngine.h>

namespace LuaEngine {
  void Engine::RunFile(const std::string& file) {
    lua.script(R"(
      if __debug then
        print("starting blackboard version " .. __version .. " on " .. tostring(os.date("%Y-%m-%d %H:%M:%S")) .. " with debugging enabled\n")
      else 
        print("starting blackboard version " .. __version .. " on " .. tostring(os.date("%Y-%m-%d %H:%M:%S")) .. "\n")
      end
    )");

    if (BL_DEBUG) {
      std::cout << "running lua file: " << file << "\n" << std::endl;
    }

    try {
      lua.script_file(file);
      lua.script("__process_widgets()");

      frameTimer->start(1000 / 120);
    } catch (const sol::error& err) {
      std::cerr << "Lua error: " << err.what() << std::endl;
    }
  }

  void Engine::RunAllFiles() {
    lua.script(R"(
      if __debug then
        print("starting blackboard version " .. __version .. " on " .. tostring(os.date("%Y-%m-%d %H:%M:%S")) .. " with debugging enabled\n")
      else 
        print("starting blackboard version " .. __version .. " on " .. tostring(os.date("%Y-%m-%d %H:%M:%S")) .. "\n")
      end
    )");

    std::string scriptPath = std::string(getenv("HOME")) + "/.config/blackboard";

    for (const auto& entry : std::filesystem::directory_iterator(scriptPath)) {
      if (entry.path().extension() == ".lua") {
        if (BL_DEBUG) {
          std::cout << "running lua file: " << entry.path().string() << "\n";
        }

        try {
          lua.script_file(entry.path().string());
        } catch (const sol::error& err) {
          std::cerr << "Lua error: " << err.what() << std::endl;
        }
      }
    }

    std::cout << "\n";

    lua.script("__process_widgets()");

    std::cout << "processing hooks\n";
    ProcessHooks();

    frameTimer->start(1000 / 120);
  }

  int Engine::Exec() {
    return engine.Exec();
  }

  Engine::Engine(int argc, char** argv) : engine(argc, argv) {
    lua.open_libraries(
      sol::lib::base,
      sol::lib::package,
      sol::lib::coroutine,
      sol::lib::string,
      sol::lib::os,
      sol::lib::math,
      sol::lib::table,
      sol::lib::debug,
      sol::lib::io,
      sol::lib::utf8
    );

    lua_registry = lua.create_table();
    lua["__widget_list"] = lua_registry;

    std::string home = getenv("HOME");

    std::string path = home + "/.luarocks/share/lua/5.4/?.lua;" + home + "/.luarocks/share/lua/5.4/?/init.lua;" + home + "/luarocks/share/lua/5.4/?.lua;" + home + "/Desktop/Projects/blackboard/src/scripts/?.lua;";
    std::string cpath = home + "/.luarocks/lib/lua/5.4/?.so;" + home + "/.luarocks/lib/lua/5.4/?.so;";

    std::string old_path = lua["package"]["path"];
    std::string old_cpath = lua["package"]["cpath"];

    lua["package"]["path"] = old_path + ";" + path;
    lua["package"]["cpath"] = old_cpath + ";" + cpath;

    lua.script(R"(
      local home = os.getenv("HOME")
      package.path = package.path .. home .. "/.luarocks/share/lua/5.4/?.lua;" .. home .. "/.luarocks/share/lua/5.4/?/init.lua;" .. home .. "/luarocks/share/lua/5.4/?.lua;" .. home .. "/Desktop/Projects/blackboard/src/scripts/?.lua;"
      package.cpath = package.cpath .. home .. "/.luarocks/lib/lua/5.4/?.so;" .. home .. "/.luarocks/lib/lua/5.4/?.so;"
    )");

    RegisterBindings();

    lua.script(R"(
      function Window (args)
        args = args or {}
        args.__widget_type = "window"
        table.insert(__widget_list, args)

        return args
      end

      function Button (args)
        args = args or {}
        args.__widget_type = "button"
        table.insert(__widget_list, args)

        return args
      end

      function Label (args)
        args = args or {}
        args.__widget_type = "label"
        table.insert(__widget_list, args)

        return args
      end

      function LineEdit (args)
        args = args or {}
        args.__widget_type = "line_edit"
        table.insert(__widget_list, args)

        return args
      end

      function PropertyAnimation (args)
        args = args or {}

        return args
      end

      function HBox (args)
        args = args or {}
        args.__widget_type = "hbox_layout"

        return args
      end

      function VBox (args)
        args = args or {}
        args.__widget_type = "vbox_layout"

        return args
      end

      function WidgetList (args)
        args = args or {}
        args.__widget_type = "widget_list"

        return args
      end
    )");

    frameTimer = new QTimer(nullptr);
    QObject::connect(frameTimer, &QTimer::timeout, frameTimer, [this]() {
      CallOnFrameCallbacks();
      ProcessSignals();
    });
  }

  Engine::~Engine() {
    if (frameTimer)
      delete frameTimer;
  }
}
