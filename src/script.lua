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
local screens = blackboard.get_monitors()

if screens ~= nil then
  for _, screen in pairs(screens) do
    print("------------------------------------")
    print("Screen name:", screen.name)
    print("Screen width:", screen.width)
    print("Screen height:", screen.height)
  end

  print("------------------------------------")
end

local window = blackboard.window_info.new()
window.name = "blackboard"
window.screen = screens[2].index
window.width = 800
window.height = 600
window.stacking_order = blackboard.stacking_order.bottom
window.anchor_zone = blackboard.anchor_zone.bottom
window.anchor_area = 30
window.padding_inner = 5
window.padding_outer = 5

blackboard.add_window(window)
blackboard.show_window("blackboard")

blackboard.exec()
