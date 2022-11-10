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
3. Generate CMake files and build `cmake ..-DCMAKE_BUILD_TYPE=Release && cmake --build .`

### Windows

No support yet :(

## Usage

`./vOS <scheduler_frequency> <process_generator_frequency`

Both of the arguments are optional.