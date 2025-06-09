#include <headers/LuaEngine.h>

int main(int argc, char** argv) {
  LuaEngine engine(argc, argv);
  
  return engine.Exec();
}
