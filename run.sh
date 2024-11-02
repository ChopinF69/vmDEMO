#!/bin/bash

BINDIR="bin"
BUILDDIR="build"
EXEC="$BINDIR/birchvm"
TEST_EXEC="$BINDIR/test_runner"

function show_help() {
  echo "Usage: $0 [OPTION]"
  echo "Options:"
  echo "  build          Build the project"
  echo "  run            Run the main executable"
  echo "  test           Build and run tests"
  echo "  clean          Clean up build and bin directories"
  echo "  help           Display this help message"
}

function build() {
  echo "Building the project..."
  make all
  echo "Build completed."
}

function run() {
  if [[ ! -f "$EXEC" ]]; then
    echo "Executable not found. Building the project first..."
    build
  fi
  echo "Running the main executable..."
  "$EXEC" "$@"
}

function test() {
  echo "Building and running tests..."
  make test
  echo "Tests completed."
}

function clean() {
  echo "Cleaning build and bin directories..."
  make clean
  echo "Clean up completed."
}

case "$1" in
build)
  build
  ;;
run)
  shift
  run "$@"
  ;;
test)
  test
  ;;
clean)
  clean
  ;;
help | *)
  show_help
  ;;
esac
