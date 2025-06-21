#include <WidgetEngine/WidgetEngine.h>

namespace WidgetEngine {
  Engine::Engine(int argc, char** argv) : app(argc, argv) {
    LayerShellQt::Shell::useLayerShell();
  }

  int Engine::Exec() {
    return app.exec();
  }
}
