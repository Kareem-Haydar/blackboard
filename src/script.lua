local wininfo = blackboard.WindowInfo.new()
wininfo.name = "test"
wininfo.scope = ""
wininfo.width = 100
wininfo.height = 100
wininfo.order = blackboard.StackingOrder.Bottom
wininfo.anchorZone = blackboard.AnchorZone.Top  |
                     blackboard.AnchorZone.Left |
                     blackboard.AnchorZone.Right
wininfo.layout = blackboard.WindowLayout.None
wininfo.anchorArea = 40
wininfo.paddingInner = 0
wininfo.paddingOuter = 0

AddWindow(wininfo)
SetWindowStyleSheet(
  "test",
  "background-color: rgba(100, 100, 100, 0);"
)

AddButton("test", "test", "test")
ResizeWidget("test", "test", 30, 30)

SetWidgetStyleSheet(
  "test",
  "test",
  "background-color: rgba(255, 255, 255, 255); border: none; border-radius: 15px; font-weight: bold;"
)

ShowAll()
Exec()
