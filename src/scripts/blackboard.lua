local blackboard = {}

blackboard.stacking_order = {
  top = StackingOrderCpp.Top,
  bottom = StackingOrderCpp.Bottom,
  left = StackingOrderCpp.Left,
  right = StackingOrderCpp.Right
}

blackboard.anchor_zone = {
  top = AnchorZoneCpp.Top,
  bottom = AnchorZoneCpp.Bottom,
  left = AnchorZoneCpp.Left,
  right = AnchorZoneCpp.Right
}

blackboard.window_layout = {
  none = WindowLayoutCpp.None,
  hbox = WindowLayoutCpp.HorizontalBox,
  vbox = WindowLayoutCpp.VerticalBox
}

blackboard.anim_curve = {
  in_quad = AnimCurveCpp.InQuad,
  out_quad = AnimCurveCpp.OutQuad,
  in_out_quad = AnimCurveCpp.InOutQuad,
  in_cubic = AnimCurveCpp.InCubic,
  out_cubic = AnimCurveCpp.OutCubic,
  in_out_cubic = AnimCurveCpp.InOutCubic
}

blackboard.window_info = {}
blackboard.window_info.__index = blackboard.window_info

function blackboard.window_info.new()
  local obj = WindowInfoCpp.new()
  local proxy = {}

  setmetatable(proxy, {
    __index = function(_, key)
      return obj[key]
    end,
    __newindex = function(_, key, value)
      obj[key] = value
    end
  })

  return proxy
end

blackboard.monitor_info = {}
blackboard.monitor_info.__index = blackboard.monitor_info

function blackboard.monitor_info.new()
  local obj = MonitorInfoCpp.new()
  local proxy = {}

  setmetatable(proxy, {
    __index = function(_, key)
      return obj[key]
    end,
    __newindex = function(_, key, value)
      obj[key] = value
    end
  })

  return proxy
end

blackboard.get_monitors = function ()
  return GetMonitorsCpp()
end

blackboard.add_window = function (info)
  AddWindowCpp(info)
end

blackboard.add_button = function (window, name, text)
  AddButtonCpp(window, name, text)
end

blackboard.add_label = function (window, name, text)
  AddLabelCpp(window, name, text)
end

return blackboard
