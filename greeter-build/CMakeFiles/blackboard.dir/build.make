# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 4.0

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kareem/Desktop/Projects/blackboard

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kareem/Desktop/Projects/blackboard/greeter-build

# Include any dependencies generated for this target.
include CMakeFiles/blackboard.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/blackboard.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/blackboard.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/blackboard.dir/flags.make

CMakeFiles/blackboard.dir/codegen:
.PHONY : CMakeFiles/blackboard.dir/codegen

CMakeFiles/blackboard.dir/src/WidgetEngine.cpp.o: CMakeFiles/blackboard.dir/flags.make
CMakeFiles/blackboard.dir/src/WidgetEngine.cpp.o: /home/kareem/Desktop/Projects/blackboard/src/WidgetEngine.cpp
CMakeFiles/blackboard.dir/src/WidgetEngine.cpp.o: CMakeFiles/blackboard.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kareem/Desktop/Projects/blackboard/greeter-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/blackboard.dir/src/WidgetEngine.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/blackboard.dir/src/WidgetEngine.cpp.o -MF CMakeFiles/blackboard.dir/src/WidgetEngine.cpp.o.d -o CMakeFiles/blackboard.dir/src/WidgetEngine.cpp.o -c /home/kareem/Desktop/Projects/blackboard/src/WidgetEngine.cpp

CMakeFiles/blackboard.dir/src/WidgetEngine.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/blackboard.dir/src/WidgetEngine.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kareem/Desktop/Projects/blackboard/src/WidgetEngine.cpp > CMakeFiles/blackboard.dir/src/WidgetEngine.cpp.i

CMakeFiles/blackboard.dir/src/WidgetEngine.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/blackboard.dir/src/WidgetEngine.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kareem/Desktop/Projects/blackboard/src/WidgetEngine.cpp -o CMakeFiles/blackboard.dir/src/WidgetEngine.cpp.s

CMakeFiles/blackboard.dir/src/LuaEngine.cpp.o: CMakeFiles/blackboard.dir/flags.make
CMakeFiles/blackboard.dir/src/LuaEngine.cpp.o: /home/kareem/Desktop/Projects/blackboard/src/LuaEngine.cpp
CMakeFiles/blackboard.dir/src/LuaEngine.cpp.o: CMakeFiles/blackboard.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kareem/Desktop/Projects/blackboard/greeter-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/blackboard.dir/src/LuaEngine.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/blackboard.dir/src/LuaEngine.cpp.o -MF CMakeFiles/blackboard.dir/src/LuaEngine.cpp.o.d -o CMakeFiles/blackboard.dir/src/LuaEngine.cpp.o -c /home/kareem/Desktop/Projects/blackboard/src/LuaEngine.cpp

CMakeFiles/blackboard.dir/src/LuaEngine.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/blackboard.dir/src/LuaEngine.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kareem/Desktop/Projects/blackboard/src/LuaEngine.cpp > CMakeFiles/blackboard.dir/src/LuaEngine.cpp.i

CMakeFiles/blackboard.dir/src/LuaEngine.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/blackboard.dir/src/LuaEngine.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kareem/Desktop/Projects/blackboard/src/LuaEngine.cpp -o CMakeFiles/blackboard.dir/src/LuaEngine.cpp.s

CMakeFiles/blackboard.dir/greeter/greeter.cpp.o: CMakeFiles/blackboard.dir/flags.make
CMakeFiles/blackboard.dir/greeter/greeter.cpp.o: /home/kareem/Desktop/Projects/blackboard/greeter/greeter.cpp
CMakeFiles/blackboard.dir/greeter/greeter.cpp.o: CMakeFiles/blackboard.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kareem/Desktop/Projects/blackboard/greeter-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/blackboard.dir/greeter/greeter.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/blackboard.dir/greeter/greeter.cpp.o -MF CMakeFiles/blackboard.dir/greeter/greeter.cpp.o.d -o CMakeFiles/blackboard.dir/greeter/greeter.cpp.o -c /home/kareem/Desktop/Projects/blackboard/greeter/greeter.cpp

CMakeFiles/blackboard.dir/greeter/greeter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/blackboard.dir/greeter/greeter.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kareem/Desktop/Projects/blackboard/greeter/greeter.cpp > CMakeFiles/blackboard.dir/greeter/greeter.cpp.i

CMakeFiles/blackboard.dir/greeter/greeter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/blackboard.dir/greeter/greeter.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kareem/Desktop/Projects/blackboard/greeter/greeter.cpp -o CMakeFiles/blackboard.dir/greeter/greeter.cpp.s

# Object files for target blackboard
blackboard_OBJECTS = \
"CMakeFiles/blackboard.dir/src/WidgetEngine.cpp.o" \
"CMakeFiles/blackboard.dir/src/LuaEngine.cpp.o" \
"CMakeFiles/blackboard.dir/greeter/greeter.cpp.o"

# External object files for target blackboard
blackboard_EXTERNAL_OBJECTS =

blackboard: CMakeFiles/blackboard.dir/src/WidgetEngine.cpp.o
blackboard: CMakeFiles/blackboard.dir/src/LuaEngine.cpp.o
blackboard: CMakeFiles/blackboard.dir/greeter/greeter.cpp.o
blackboard: CMakeFiles/blackboard.dir/build.make
blackboard: CMakeFiles/blackboard.dir/compiler_depend.ts
blackboard: /usr/lib/libQt6Widgets.so.6.9.0
blackboard: /usr/lib/libLayerShellQtInterface.so.6.3.5
blackboard: /usr/lib/libQt6WaylandClient.so.6.9.0
blackboard: /usr/lib/libQt6Gui.so.6.9.0
blackboard: /usr/lib/libQt6DBus.so.6.9.0
blackboard: /usr/lib/libQt6Core.so.6.9.0
blackboard: /usr/lib/libGLX.so
blackboard: /usr/lib/libOpenGL.so
blackboard: /usr/lib/libwayland-client.so
blackboard: /usr/lib/libwayland-cursor.so
blackboard: CMakeFiles/blackboard.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/kareem/Desktop/Projects/blackboard/greeter-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable blackboard"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/blackboard.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/blackboard.dir/build: blackboard
.PHONY : CMakeFiles/blackboard.dir/build

CMakeFiles/blackboard.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/blackboard.dir/cmake_clean.cmake
.PHONY : CMakeFiles/blackboard.dir/clean

CMakeFiles/blackboard.dir/depend:
	cd /home/kareem/Desktop/Projects/blackboard/greeter-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kareem/Desktop/Projects/blackboard /home/kareem/Desktop/Projects/blackboard /home/kareem/Desktop/Projects/blackboard/greeter-build /home/kareem/Desktop/Projects/blackboard/greeter-build /home/kareem/Desktop/Projects/blackboard/greeter-build/CMakeFiles/blackboard.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/blackboard.dir/depend

