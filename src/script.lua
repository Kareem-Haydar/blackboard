local inifile = require("inifile")

-- Load a .desktop or .ini file
local data = inifile.parse("/usr/share/applications/zen.desktop")

-- Access sections and keys
if data and data["Desktop Entry"] then
    local entry = data["Desktop Entry"]
    print("Name:", entry.Name)
    print("Exec:", entry.Exec)
    print("Icon:", entry.Icon)
end

local blackboard = require("blackboard")

Window {
  name = "blackboard",
  screen = 1,
  width = 800,
  height = 600,
  anchor_area = 30,
  anchor_zone = blackboard.anchor_zone.top,
  stacking_order = blackboard.stacking_order.top,
  on_create = function (data)
    blackboard.show_window(data.name)
  end
}

blackboard.exec()
