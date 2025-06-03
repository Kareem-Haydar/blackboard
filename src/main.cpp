#include <headers/LuaEngine.h>

int main(int argc, char** argv) {
  LuaEngine engine(argc, argv);
  engine.RunScript("/home/kareem/Desktop/Projects/blackboard/src/scrips/blackboard.lua");
  return 0;
}
