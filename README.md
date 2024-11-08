# BirchVM

BirchVM is a simple, custom virtual machine (VM) project built in C. This project serves as a basic demonstration of how virtual machines can be structured, with an emphasis on modularity and testing.

## Table of Contents
- [Overview](#overview)
- [Project Structure](#project-structure)
- [Building and Running](#building-and-running)
- [Running the Tests](#running-the-tests)
- [Cleaning the Project](#cleaning-the-project)
- [Scripted Workflow](#scripted-workflow)
---

## Overview

The BirchVM project is designed to emulate basic VM functionality with core components modularized into separate source files. It is organized for flexibility and ease of testing, with clear directory structures for source files, headers, build files, and test cases. A `Makefile` is included for build automation, along with a `run.sh` script that provides commands for various build and test operations.

Registers : AX / BX / CX / DX / SP / IP and its derivatives ( on 8 bits and 32 bits for the first 4).

Operations : MOV / SUB / MUL / DIV / NOP / HLT.

## Project Structure

The project follows a professional layout to keep components organized:

```plaintext
.
├── bin/                     # Executable output files
│   └── birchvm              # Main executable
├── build/                   # Compiled object files
├── include/                 # Header files
│   ├── birchvm.h
│   └── utils.h
├── src/                     # Source code
│   ├── birchvm.c
│   └── utils.c
├── test/                    # Test files
│   ├── birchvm_test.c
│   └── birchvm_test.h
├── makefile                 # Makefile for building the project
└── run.sh                   # Script for managing builds, tests, and runs

    src/: Contains main source files for the virtual machine.
    include/: Holds all header files, providing interfaces for components in src/.
    build/: Stores compiled object files to keep the workspace clean.
    bin/: Contains the final executable output, birchvm, and any additional binaries.
    test/: Holds test code and files specific to the test suite.
```
## Building and Running

To build the project, navigate to the project root directory and use make or the provided run.sh script:

### Using Makefile directly
``` make ```

### Using the run.sh script
```./run.sh build```

### After building, you can execute birchvm with:

```./run.sh run```

The compiled executable will be located in the bin/ directory.
Running the Tests

## Running the tests

``` ./run.sh test ```

The run.sh script will compile the tests and execute the test runner, providing output on test results.

## Cleaning the Project

To remove all generated files, such as binaries and compiled objects, run:

``` ./run.sh clean ```

This will delete everything in the bin/ and build/ directories.

## Scripted Workflow

The run.sh script offers a convenient interface for managing the project, supporting the following commands:
```
build — Compile the project and create executables.
run — Run the main executable (birchvm), with optional arguments.
test — Build and execute tests.
clean — Remove all compiled files.
help — Show usage instructions.
```
