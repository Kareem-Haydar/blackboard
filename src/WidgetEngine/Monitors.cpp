#include <WidgetEngine/WidgetEngine.h>

namespace WidgetEngine {
  std::vector<WidgetEngine::MonitorInfo> Engine::GetMonitors() {
    const auto screens = QApplication::screens();

    std::vector<MonitorInfo> info;
    info.resize(screens.size());

    for (int i = 0; i < screens.size(); i++) {
      info[i].index = i;
      info[i].width = screens[i]->geometry().width();
      info[i].height = screens[i]->geometry().height();
      info[i].name = screens[i]->name().toStdString();
    }

    return info;
  }

};
