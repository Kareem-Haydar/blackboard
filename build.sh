#!/bin/bash

BUILD_DIR="build"
BINARY="./blackboard"

# Parse args
DEBUG=false
CLEAN=false
RUN=false

for arg in "$@"; do
  case $arg in
    debug) DEBUG=true ;;
    clean) CLEAN=true ;;
    run)   RUN=true ;;
  esac
done

# Clean build
if $CLEAN; then
  rm -rf "$BUILD_DIR"
fi

# Configure if needed
if [ ! -d "$BUILD_DIR" ]; then
  mkdir "$BUILD_DIR"
  cd "$BUILD_DIR"

  if $DEBUG; then
    cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1
  else
    cmake .. -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=1
  fi
else
  cd "$BUILD_DIR"
fi

# Build
cmake --build . --parallel

# Run
if $RUN; then
  if [ -f "$BINARY" ]; then
    $BINARY
  else
    echo "❌ Binary not found: $BINARY"
    exit 1
  fi
fi

