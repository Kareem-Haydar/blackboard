local wininfo = blackboard.WindowInfo.new()
wininfo.name = "test"
wininfo.scope = ""
wininfo.width = 100
wininfo.height = 100
wininfo.order = blackboard.StackingOrder.Top
wininfo.anchorZone = blackboard.AnchorZone.Top
wininfo.layout = blackboard.WindowLayout.VerticalBox
wininfo.anchorArea = 20
wininfo.paddingInner = 0
wininfo.paddingOuter = 0

blackboard.AddWindow(wininfo)
blackboard.ShowAll()
blackboard.Exec()
