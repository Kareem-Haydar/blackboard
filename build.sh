#!/bin/bash

if [[ $1 == "clean" || $2 == "clean" || $3 == "clean" ]]; then
  rm -r build
  mkdir build
  cd build

  if [[ $1 == "debug" || $2 == "debug" || $3 == "debug" ]]; then
    cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug
  else
    cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1
  fi

  make
else 
  if [ ! -d "build" ]; then
    mkdir build
    cd build

    if [[ $1 == "debug" || $2 == "debug" || $3 == "debug" ]]; then
      cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug
    else 
      cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1
    fi

    make
  else
    cd build
    make
  fi
fi

if [[ $1 == "run" || $2 == "run" || $3 == "run" ]]; then
  ./blackboard
fi
