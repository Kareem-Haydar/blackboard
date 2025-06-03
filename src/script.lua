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

local screens = GetMonitors()
local windows = {}

for i, v in ipairs(screens) do
  windows[v.name] = WindowInfo.new()
  windows[v.name].name = "topbar_" .. v.index
  windows[v.name].screen = v.index
  windows[v.name].scope = ""
  windows[v.name].width = v.width
  windows[v.name].height = 100
  windows[v.name].order = blackboard.StackingOrder.Bottom
  windows[v.name].anchorZone = blackboard.AnchorZone.Top
  windows[v.name].layout = blackboard.WindowLayout.None
  windows[v.name].anchorArea = 30

  print("creating window " .. windows[v.name].name)

  AddWindow(windows[v.name])

  SetWindowStyleSheet(
    windows[v.name].name,
    "background-color: rgba(100, 100, 100, 0);"
  )

  AddButton(windows[v.name].name, "power", "󰤆")
  ResizeWidget(windows[v.name].name, "power", 30, 30)
  SetWidgetStyleSheet(
    windows[v.name].name,
    "power",
    "background-color: rgba(255, 255, 255, 255); border-radius: 15px"
  )
end

ShowAll()
Exec()
