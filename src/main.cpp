#include <headers/LuaEngine.h>

int main(int argc, char** argv) {
  LuaEngine engine(argc, argv);
  engine.RunScript("/home/kareem/Desktop/Projects/blackboard/src/scripts/blackboard.lua");
  engine.RunScript("/home/kareem/Desktop/Projects/blackboard/src/script.lua");
  return 0;
}
