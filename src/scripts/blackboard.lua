local blackboard = {}

blackboard.stacking_order = {
  none = StackingOrderCpp.None,
  top = StackingOrderCpp.Top,
  bottom = StackingOrderCpp.Bottom,
  left = StackingOrderCpp.Left,
  right = StackingOrderCpp.Right
}

blackboard.anchor_zone = {
  none = AnchorZoneCpp.None,
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
  local self = {}
  self._cobj = WindowInfoCpp.new()

  local proxy = {
    name = "",
    width = 0,
    height = 0,
    screen = "",
    stacking_order = blackboard.stacking_order.none,
    anchor_area = 0,
    anchor_zone = blackboard.anchor_zone.none,
    padding_inner = 0,
    padding_outer = 0,
    layout = blackboard.window_layout.none
  }

  setmetatable(proxy, {
    __index = function(_, key)
      return self._cobj[key]
    end,
    __newindex = function(_, key, value)
      rawset(proxy, key, value)
    end
  })

  return proxy
end

blackboard.monitor_info = {}
blackboard.monitor_info.__index = blackboard.monitor_info

function blackboard.monitor_info.new()
  local self = {}
  self._cobj = MonitorInfoCpp.new()

  local proxy = {
    name = "",
    index = 0,
    width = 0,
    height = 0
  }


  setmetatable(proxy, {
    __index = function(_, key)
      return self._cobj[key]
    end,
    __newindex = function(_, key, value)
      rawset(proxy, key, value)
    end
  })

  return proxy
end

blackboard.get_monitors = function ()
  return GetMonitorsCpp()
end

blackboard.add_window = function (info)
  local wininfo = WindowInfoCpp.new()

  if info.name then
    wininfo.name = info.name
  end

  if info.screen then
    wininfo.screen = info.screen
  end

  if info.scope then
    wininfo.scope = info.scope
  end

  if info.width then
    wininfo.width = info.width
  end

  if info.height then
    wininfo.height = info.height
  end

  if info.stacking_order then
    wininfo.order = info.stacking_order
  end

  if info.anchor_area then
    wininfo.anchorArea = info.anchor_area
  end

  if info.anchor_zone then
    wininfo.anchorZone = info.anchor_zone
  end

  if info.padding_inner then
    wininfo.paddingInner = info.padding_inner
  end

  if info.padding_outer then
    wininfo.paddingOuter = info.padding_outer
  end

  if info.layout then
    wininfo.layout = info.layout
  else
    wininfo.layout = WindowLayoutCpp.None
  end

  AddWindowCpp(wininfo)
end

blackboard.show_window = function (name)
  ShowWindowCpp(name)
end

blackboard.hide_window = function (name)
  HideWindowCpp(name)
end

blackboard.show_all_windows = function ()
  ShowAllCpp()
end

blackboard.hide_all_windows = function ()
  HideAllCpp()
end

blackboard.add_button = function (window, name, text)
  AddButtonCpp(window, name, text)
end

blackboard.add_label = function (window, name, text)
  AddLabelCpp(window, name, text)
end

blackboard.exec = function ()
  ExecCpp()
end

return blackboard
