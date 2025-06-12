#!/bin/bash

if [[ $1 == "clean" ]]; then
  rm -r build
  mkdir build
  cd build
  cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1
  make
else 
  if [ ! -d "build" ]; then
    mkdir build
    cd build
    cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1
    make
  else
    cd build
    make
  fi
fi

if [[ $1 == "run" || $2 == "run" ]]; then
  ./blackboard
fi
