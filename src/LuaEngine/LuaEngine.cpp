#include <LuaEngine/LuaEngine.h>

namespace LuaEngine {
  void Engine::RunFile(const std::string& file) {
    lua.script(R"(
      if __debug then
        print("starting blackboard version " .. __version .. " on " .. tostring(os.date("%Y-%m-%d %H:%M:%S")) .. " with debugging enabled")
      else 
        print("starting blackboard version " .. __version .. " on " .. tostring(os.date("%Y-%m-%d %H:%M:%S")))
      end
    )");

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
        print("starting blackboard version " .. __version .. " on " .. tostring(os.date("%Y-%m-%d %H:%M:%S")) .. " with debugging enabled")
      else 
        print("starting blackboard version " .. __version .. " on " .. tostring(os.date("%Y-%m-%d %H:%M:%S")))
      end
    )");

    std::string scriptPath = std::string(getenv("HOME")) + "/.config/blackboard";

    for (const auto& entry : std::filesystem::directory_iterator(scriptPath)) {
      if (entry.path().extension() == ".lua") {
        try {
          lua.script_file(entry.path().string());
        } catch (const sol::error& err) {
          std::cerr << "Lua error: " << err.what() << std::endl;
        }
      }
    }

    lua.script("__process_widgets()");

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
        args.id = args.id or ("window_" .. tostring(#__widget_list + 1))
        table.insert(__widget_list, args)

        return args
      end

      function Button (args)
        args = args or {}
        args.__widget_type = "button"
        args.id = args.id or ("button_" .. tostring(#__widget_list + 1))
        table.insert(__widget_list, args)

        return args
      end

      function Label (args)
        args = args or {}
        args.__widget_type = "label"
        args.id = args.id or ("label_" .. tostring(#__widget_list + 1))
        table.insert(__widget_list, args)

        return args
      end

      function LineEdit (args)
        args = args or {}
        args.__widget_type = "line_edit"
        args.id = args.id or ("line_edit_" .. tostring(#__widget_list + 1))
        table.insert(__widget_list, args)

        return args
      end

      function PropertyAnimation (args)
        args = args or {}
        args.__widget_type = "property_animation"

        return args
      end
    )");

    frameTimer = new QTimer(nullptr);
    QObject::connect(frameTimer, &QTimer::timeout, frameTimer, [this]() {
      CallOnFrameCallbacks();
      HandleAnimations();
    });
  }

  Engine::~Engine() {
    if (frameTimer)
      delete frameTimer;
  }
}
