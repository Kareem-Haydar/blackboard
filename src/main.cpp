#include <LuaEngine/LuaEngine.h>

void Run(int argc, char** argv) {
  LuaEngine::Engine engine(argc, argv);

  if (argc == 1) {
    engine.RunAllFiles();
    exit(engine.Exec());
  }

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
      std::cout << "Blackboard version " << BL_VERSION << "\n";
      exit(0);
    }

    if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0) {
      BL_DEBUG = true;

      engine.RunAllFiles();
      exit(engine.Exec());
    }

    if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) {
      if (i + 1 > argc) {
        std::cerr << "no file specified\n";
        exit(1);
      }

      engine.RunFile(argv[i + 1]);
      exit(engine.Exec());
    }

    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      std::cout << "Usage: blackboard [options]\n";
      std::cout << "options:\n";
      std::cout << "\t -f, --file <file>  Run a specific file\n";
      std::cout << "\t -h, --help         Show this help message\n";
      std::cout << "\t -v, --version      Show version information\n";
      std::cout << "\t -d, --debug        Enable debug mode\n";
      std::cout << "\t no options         Run all files in ~/.config/blackboard\n";

      exit(0);
    }
  }
}

int main(int argc, char** argv) {

  Run(argc, argv);
  
  return 0;
}
