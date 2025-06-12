cmake_minimum_required(VERSION 3.18)

project(blackboard LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LUA REQUIRED lua)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets DBus WaylandClient)
find_package(LayerShellQt REQUIRED)

option(BUILD_GREETER "build the greeter binary" OFF)

if (BUILD_GREETER)
  message("building greeter")
  set(SOURCES
    src/WidgetEngine.cpp
    src/LuaEngine.cpp
    greeter/greeter.cpp
  )

  set(HEADERS
    headers/WidgetEngine.h
    headers/LuaEngine.h
  )

  add_executable(blackboard ${SOURCES} ${HEADERS})

  message("Compiler ID: ${CMAKE_CXX_COMPILER_ID}")

  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    remove_definitions("-mno-direct-extern-access")
  endif()

  target_include_directories(blackboard PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${LUA_INCLUDE_DIRS})
  target_link_libraries(blackboard PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets LayerShellQt::Interface Qt6::DBus Qt6::WaylandClient ${LUA_LIBRARIES})
else()
  set(SOURCES
    src/main.cpp
    src/WidgetEngine.cpp
    src/LuaEngine.cpp
  )

  set(HEADERS
    headers/WidgetEngine.h
    headers/LuaEngine.h
  )

  add_executable(blackboard ${SOURCES} ${HEADERS})

  message("Compiler ID: ${CMAKE_CXX_COMPILER_ID}")

  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    remove_definitions("-mno-direct-extern-access")
  endif()

  target_include_directories(blackboard PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${LUA_INCLUDE_DIRS})
  target_link_libraries(blackboard PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets LayerShellQt::Interface Qt6::DBus Qt6::WaylandClient ${LUA_LIBRARIES})
endif()
