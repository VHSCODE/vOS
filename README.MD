# vOS : Kernel Simulator
A kernel simulator for my Operating Systems class at the University


## Build Instructions

### Linux

Prerequisites:
- CMake >= 3.20
- A C Compiler (Only tested with GCC)
- Git

Instructions:

1. Run `git clone https://github.com/VHSCODE/vOS`
2. Create a build folder and enter it `mkdir build && cd build`
3. Generate CMake files and build `cmake .. -DCMAKE_BUILD_TYPE=Release && cmake --build .`

### Windows

No support yet :(

## Usage

Place your specs.txt file, the vOS binary and the .elf files within the same directory and run `./vOS `

### Specs File
The specs.txt file describes the specifications of the machine the simulator will run on. An example specs.txt file
is provided for a quick start. Look for the comment in "src/main.c" for more info about the format.

